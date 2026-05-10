#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../../generic_c/include/contest_packet.h"
#include "../../generic_c/include/contest_periodic_task.h"
#include "../../generic_c/include/contest_ring_buffer.h"
#include "../../generic_c/include/contest_state_machine.h"
#include "../include/contest_hal_button.h"
#include "../include/contest_hal_status_led.h"
#include "../include/contest_hal_uart_packet.h"

#define APP_OUTPUT_LIMIT 1000
#define APP_RX_BUFFER_CAPACITY 128u

#define APP_CMD_START 0x01u
#define APP_CMD_STOP 0x02u
#define APP_CMD_SET_TARGET 0x10u
#define APP_CMD_ENTER_TUNE 0x11u
#define APP_TELEMETRY_CMD 0x81u

/*
 * Replace these macros with the real board resources from your CubeMX project.
 * The values below are only common starter choices on many STM32 boards.
 */
#define APP_MODE_BUTTON_PORT GPIOB
#define APP_MODE_BUTTON_PIN GPIO_PIN_12
#define APP_MODE_BUTTON_ACTIVE_LEVEL false
#define APP_STATUS_LED_PORT GPIOC
#define APP_STATUS_LED_PIN GPIO_PIN_13
#define APP_STATUS_LED_ACTIVE_LEVEL false

extern UART_HandleTypeDef huart1;

enum {
    APP_STATE_STOP = 0,
    APP_STATE_RUN = 1,
    APP_STATE_TUNE = 2,
    APP_STATE_FAULT = 3
};

typedef struct {
    bool emergency_stop;
    int16_t sensor_left;
    int16_t sensor_right;
    int16_t speed_feedback;
    uint16_t battery_mv;
} app_stm32_sensor_frame_t;

typedef struct {
    int16_t target_speed;
    int16_t target_turn;
    bool telemetry_enable;
} app_stm32_command_frame_t;

typedef struct {
    int16_t left_output;
    int16_t right_output;
    bool buzzer_on;
} app_stm32_output_frame_t;

static contest_hal_button_t g_mode_button;
static contest_hal_status_led_t g_status_led;
static contest_hal_uart_packet_t g_uart_packet;
static contest_ring_buffer_t g_uart_rx_buffer;
static contest_state_machine_t g_app_state;
static contest_periodic_task_t g_button_task;
static contest_periodic_task_t g_control_task;
static contest_periodic_task_t g_telemetry_task;
static contest_periodic_task_t g_status_task;
static uint8_t g_uart_rx_storage[APP_RX_BUFFER_CAPACITY];
static app_stm32_sensor_frame_t g_inputs;
static app_stm32_command_frame_t g_command;
static app_stm32_output_frame_t g_outputs;

static void app_stm32_contest_template_reset_drive(void);
static void app_stm32_contest_template_apply_state_entry(void);
static void app_stm32_contest_template_poll_button(void);
static void app_stm32_contest_template_process_uart(void);
static void app_stm32_contest_template_on_command_frame(const contest_packet_frame_t *frame);
static void app_stm32_contest_template_control_step(void);
static void app_stm32_contest_template_run_control(void);
static void app_stm32_contest_template_status_step(void);
static void app_stm32_contest_template_send_telemetry(void);
static int16_t app_stm32_contest_template_clamp_i16(int32_t value, int16_t minimum, int16_t maximum);
static int16_t app_stm32_contest_template_load_i16_le(const uint8_t *bytes);
static void app_stm32_contest_template_store_i16_le(uint8_t *bytes, int16_t value);

static void app_stm32_read_sensor_frame(app_stm32_sensor_frame_t *inputs);
static void app_stm32_write_motor_output(int16_t left_output, int16_t right_output);
static void app_stm32_write_buzzer(bool on);

void app_stm32_contest_template_init(void)
{
    (void)contest_hal_button_init(
        &g_mode_button,
        APP_MODE_BUTTON_PORT,
        APP_MODE_BUTTON_PIN,
        APP_MODE_BUTTON_ACTIVE_LEVEL,
        2u,
        80u
    );
    (void)contest_hal_status_led_init(
        &g_status_led,
        APP_STATUS_LED_PORT,
        APP_STATUS_LED_PIN,
        APP_STATUS_LED_ACTIVE_LEVEL
    );
    (void)contest_ring_buffer_init(&g_uart_rx_buffer, g_uart_rx_storage, sizeof(g_uart_rx_storage));
    (void)contest_hal_uart_packet_init(&g_uart_packet, &huart1, &g_uart_rx_buffer);
    (void)contest_hal_uart_packet_start_rx_it(&g_uart_packet);

    contest_state_machine_init(&g_app_state, APP_STATE_STOP);
    contest_periodic_task_init(&g_button_task, 1u);
    contest_periodic_task_init(&g_control_task, 5u);
    contest_periodic_task_init(&g_telemetry_task, 20u);
    contest_periodic_task_init(&g_status_task, 100u);

    g_command.target_speed = 300;
    g_command.target_turn = 0;
    g_command.telemetry_enable = true;

    g_inputs.emergency_stop = false;
    g_inputs.sensor_left = 0;
    g_inputs.sensor_right = 0;
    g_inputs.speed_feedback = 0;
    g_inputs.battery_mv = 0u;

    app_stm32_contest_template_reset_drive();
    g_outputs.buzzer_on = false;
    app_stm32_contest_template_apply_state_entry();
}

void app_stm32_contest_template_tick_1ms(void)
{
    contest_periodic_task_tick(&g_button_task);
    contest_periodic_task_tick(&g_control_task);
    contest_periodic_task_tick(&g_telemetry_task);
    contest_periodic_task_tick(&g_status_task);
    contest_state_machine_tick(&g_app_state);
}

void app_stm32_contest_template_step(void)
{
    app_stm32_contest_template_process_uart();

    if (contest_periodic_task_consume(&g_button_task)) {
        app_stm32_contest_template_poll_button();
    }

    if (contest_periodic_task_consume(&g_control_task)) {
        app_stm32_contest_template_control_step();
    }

    if (contest_periodic_task_consume(&g_telemetry_task)) {
        app_stm32_contest_template_send_telemetry();
    }

    if (contest_periodic_task_consume(&g_status_task)) {
        app_stm32_contest_template_status_step();
    }

    if (contest_state_machine_entered(&g_app_state)) {
        app_stm32_contest_template_apply_state_entry();
    }
}

bool app_stm32_contest_template_on_uart_rx_complete(UART_HandleTypeDef *huart)
{
    return contest_hal_uart_packet_on_rx_complete(&g_uart_packet, huart);
}

void app_stm32_contest_template_set_command(int16_t target_speed, int16_t target_turn)
{
    g_command.target_speed = app_stm32_contest_template_clamp_i16(target_speed, -APP_OUTPUT_LIMIT, APP_OUTPUT_LIMIT);
    g_command.target_turn = app_stm32_contest_template_clamp_i16(target_turn, -APP_OUTPUT_LIMIT, APP_OUTPUT_LIMIT);
}

void app_stm32_contest_template_set_telemetry_enable(bool enabled)
{
    g_command.telemetry_enable = enabled;
}

void app_stm32_contest_template_clear_fault(void)
{
    if (g_app_state.current_state == APP_STATE_FAULT) {
        contest_state_machine_transition(&g_app_state, APP_STATE_STOP);
    }
}

static void app_stm32_contest_template_reset_drive(void)
{
    g_outputs.left_output = 0;
    g_outputs.right_output = 0;
}

static void app_stm32_contest_template_apply_state_entry(void)
{
    contest_hal_status_led_stop(&g_status_led);

    switch (g_app_state.current_state) {
        case APP_STATE_STOP:
            app_stm32_contest_template_reset_drive();
            g_outputs.buzzer_on = false;
            (void)contest_hal_status_led_start_pattern(&g_status_led, 1u, 9u, 0u);
            break;

        case APP_STATE_RUN:
            g_outputs.buzzer_on = false;
            contest_hal_status_led_set(&g_status_led, true);
            break;

        case APP_STATE_TUNE:
            app_stm32_contest_template_reset_drive();
            g_outputs.buzzer_on = false;
            (void)contest_hal_status_led_start_pattern(&g_status_led, 2u, 2u, 0u);
            break;

        case APP_STATE_FAULT:
        default:
            app_stm32_contest_template_reset_drive();
            g_outputs.buzzer_on = true;
            (void)contest_hal_status_led_start_pattern(&g_status_led, 1u, 1u, 0u);
            break;
    }

    app_stm32_write_motor_output(g_outputs.left_output, g_outputs.right_output);
    app_stm32_write_buzzer(g_outputs.buzzer_on);
}

static void app_stm32_contest_template_poll_button(void)
{
    contest_button_event_t event;

    event = contest_hal_button_poll(&g_mode_button);

    if (event & CONTEST_BUTTON_EVENT_CLICK) {
        if (g_app_state.current_state == APP_STATE_STOP) {
            contest_state_machine_transition(&g_app_state, APP_STATE_RUN);
        } else if (g_app_state.current_state == APP_STATE_RUN || g_app_state.current_state == APP_STATE_TUNE) {
            contest_state_machine_transition(&g_app_state, APP_STATE_STOP);
        }
    }

    if ((event & CONTEST_BUTTON_EVENT_LONG_PRESS) && g_app_state.current_state != APP_STATE_FAULT) {
        contest_state_machine_transition(&g_app_state, APP_STATE_TUNE);
    }
}

static void app_stm32_contest_template_process_uart(void)
{
    contest_packet_frame_t frame;

    while (contest_hal_uart_packet_process_byte_stream(&g_uart_packet, &frame)) {
        app_stm32_contest_template_on_command_frame(&frame);
    }
}

static void app_stm32_contest_template_on_command_frame(const contest_packet_frame_t *frame)
{
    if (frame == 0) {
        return;
    }

    switch (frame->cmd) {
        case APP_CMD_START:
            contest_state_machine_transition(&g_app_state, APP_STATE_RUN);
            break;

        case APP_CMD_STOP:
            contest_state_machine_transition(&g_app_state, APP_STATE_STOP);
            break;

        case APP_CMD_SET_TARGET:
            if (frame->length >= 4u) {
                app_stm32_contest_template_set_command(
                    app_stm32_contest_template_load_i16_le(&frame->payload[0]),
                    app_stm32_contest_template_load_i16_le(&frame->payload[2])
                );
            }
            break;

        case APP_CMD_ENTER_TUNE:
            contest_state_machine_transition(&g_app_state, APP_STATE_TUNE);
            break;

        default:
            break;
    }
}

static void app_stm32_contest_template_control_step(void)
{
    app_stm32_read_sensor_frame(&g_inputs);

    if (g_inputs.emergency_stop) {
        contest_state_machine_transition(&g_app_state, APP_STATE_FAULT);
    }

    switch (g_app_state.current_state) {
        case APP_STATE_RUN:
            app_stm32_contest_template_run_control();
            g_outputs.buzzer_on = false;
            break;

        case APP_STATE_TUNE:
        case APP_STATE_STOP:
            app_stm32_contest_template_reset_drive();
            g_outputs.buzzer_on = false;
            break;

        case APP_STATE_FAULT:
        default:
            app_stm32_contest_template_reset_drive();
            g_outputs.buzzer_on = true;
            break;
    }

    app_stm32_write_motor_output(g_outputs.left_output, g_outputs.right_output);
    app_stm32_write_buzzer(g_outputs.buzzer_on);
}

static void app_stm32_contest_template_run_control(void)
{
    int32_t drive_term;
    int32_t line_correction;
    int32_t left_output;
    int32_t right_output;

    /*
     * This placeholder control law gives you a safe place to plug in the
     * actual topic logic without rewriting the surrounding HAL structure.
     */
    drive_term = (int32_t)g_command.target_speed - (int32_t)g_inputs.speed_feedback;
    line_correction = (int32_t)g_inputs.sensor_left - (int32_t)g_inputs.sensor_right;

    left_output = drive_term - (int32_t)g_command.target_turn - line_correction;
    right_output = drive_term + (int32_t)g_command.target_turn + line_correction;

    g_outputs.left_output = app_stm32_contest_template_clamp_i16(left_output, -APP_OUTPUT_LIMIT, APP_OUTPUT_LIMIT);
    g_outputs.right_output = app_stm32_contest_template_clamp_i16(right_output, -APP_OUTPUT_LIMIT, APP_OUTPUT_LIMIT);
}

static void app_stm32_contest_template_status_step(void)
{
    if (g_app_state.current_state != APP_STATE_RUN) {
        contest_hal_status_led_tick(&g_status_led);
    }
}

static void app_stm32_contest_template_send_telemetry(void)
{
    uint8_t payload[10];
    uint8_t frame[16];
    size_t written;

    if (!g_command.telemetry_enable) {
        return;
    }

    payload[0] = g_app_state.current_state;
    payload[1] = g_inputs.emergency_stop ? 1u : 0u;
    app_stm32_contest_template_store_i16_le(&payload[2], g_inputs.speed_feedback);
    app_stm32_contest_template_store_i16_le(&payload[4], (int16_t)g_inputs.battery_mv);
    app_stm32_contest_template_store_i16_le(&payload[6], g_outputs.left_output);
    app_stm32_contest_template_store_i16_le(&payload[8], g_outputs.right_output);

    if (contest_packet_encode(APP_TELEMETRY_CMD, payload, sizeof(payload), frame, sizeof(frame), &written)) {
        (void)HAL_UART_Transmit(&huart1, frame, (uint16_t)written, 10u);
    }
}

static int16_t app_stm32_contest_template_clamp_i16(int32_t value, int16_t minimum, int16_t maximum)
{
    if (value < (int32_t)minimum) {
        return minimum;
    }

    if (value > (int32_t)maximum) {
        return maximum;
    }

    return (int16_t)value;
}

static int16_t app_stm32_contest_template_load_i16_le(const uint8_t *bytes)
{
    uint16_t low;
    uint16_t high;

    if (bytes == 0) {
        return 0;
    }

    low = bytes[0];
    high = (uint16_t)bytes[1] << 8;
    return (int16_t)(low | high);
}

static void app_stm32_contest_template_store_i16_le(uint8_t *bytes, int16_t value)
{
    uint16_t raw_value;

    if (bytes == 0) {
        return;
    }

    raw_value = (uint16_t)value;
    bytes[0] = (uint8_t)(raw_value & 0xFFu);
    bytes[1] = (uint8_t)(raw_value >> 8);
}

static void app_stm32_read_sensor_frame(app_stm32_sensor_frame_t *inputs)
{
    if (inputs == 0) {
        return;
    }

    /*
     * Replace with your board-facing acquisition path:
     * - ADC grayscale or analog feedback
     * - encoder speed
     * - IMU angle
     * - battery sampling
     * - driver fault inputs
     */
    inputs->emergency_stop = false;
    inputs->sensor_left = 0;
    inputs->sensor_right = 0;
    inputs->speed_feedback = 0;
    inputs->battery_mv = 12000u;
}

static void app_stm32_write_motor_output(int16_t left_output, int16_t right_output)
{
    /*
     * Replace with your real motor write-back:
     * - left and right PWM compare values
     * - direction GPIO polarity
     * - optional dead-zone handling
     */
    (void)left_output;
    (void)right_output;
}

static void app_stm32_write_buzzer(bool on)
{
    /*
     * Replace with your buzzer GPIO or timer output.
     * You can also forward this to the repository buzzer pattern helpers.
     */
    (void)on;
}
