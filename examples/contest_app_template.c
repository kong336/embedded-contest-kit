#include <stdbool.h>
#include <stdint.h>

#include "../generic_c/include/contest_button.h"
#include "../generic_c/include/contest_led_pattern.h"
#include "../generic_c/include/contest_periodic_task.h"
#include "../generic_c/include/contest_state_machine.h"

#define APP_OUTPUT_LIMIT 1000

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
} app_sensor_frame_t;

typedef struct {
    int16_t target_speed;
    int16_t target_turn;
    bool telemetry_enable;
} app_command_frame_t;

typedef struct {
    int16_t left_output;
    int16_t right_output;
    bool status_led_on;
    bool buzzer_on;
} app_output_frame_t;

static contest_button_t g_mode_button;
static contest_led_pattern_t g_status_pattern;
static contest_state_machine_t g_app_state;
static contest_periodic_task_t g_input_task;
static contest_periodic_task_t g_control_task;
static contest_periodic_task_t g_telemetry_task;
static contest_periodic_task_t g_status_task;
static app_sensor_frame_t g_inputs;
static app_command_frame_t g_command;
static app_output_frame_t g_outputs;

static void app_contest_template_reset_drive(void);
static void app_contest_template_apply_state_entry(void);
static void app_contest_template_poll_inputs(void);
static void app_contest_template_control_step(void);
static void app_contest_template_run_control(void);
static void app_contest_template_status_step(void);
static void app_contest_template_send_telemetry(void);
static int16_t app_contest_template_clamp_i16(int32_t value, int16_t minimum, int16_t maximum);

static bool app_hw_read_mode_button_level(void);
static bool app_hw_read_fault_line(void);
static void app_hw_sample_sensors(app_sensor_frame_t *inputs);
static void app_hw_apply_outputs(const app_output_frame_t *outputs);
static void app_hw_send_telemetry(
    uint8_t app_state,
    const app_sensor_frame_t *inputs,
    const app_output_frame_t *outputs
);

void app_contest_template_init(void)
{
    contest_button_init(&g_mode_button, true, 2u, 80u);
    contest_led_pattern_init(&g_status_pattern);
    contest_state_machine_init(&g_app_state, APP_STATE_STOP);

    /*
     * The whole template assumes a 1 ms outer tick.
     * Put input processing, control, telemetry, and status at different rates
     * so the application stays readable during a contest.
     */
    contest_periodic_task_init(&g_input_task, 1u);
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

    app_contest_template_reset_drive();
    g_outputs.status_led_on = false;
    g_outputs.buzzer_on = false;
    app_contest_template_apply_state_entry();
}

void app_contest_template_tick_1ms(void)
{
    contest_periodic_task_tick(&g_input_task);
    contest_periodic_task_tick(&g_control_task);
    contest_periodic_task_tick(&g_telemetry_task);
    contest_periodic_task_tick(&g_status_task);
    contest_state_machine_tick(&g_app_state);
}

void app_contest_template_step(void)
{
    if (contest_periodic_task_consume(&g_input_task)) {
        app_contest_template_poll_inputs();
    }

    if (contest_periodic_task_consume(&g_control_task)) {
        app_contest_template_control_step();
    }

    if (contest_periodic_task_consume(&g_telemetry_task)) {
        app_contest_template_send_telemetry();
    }

    if (contest_periodic_task_consume(&g_status_task)) {
        app_contest_template_status_step();
    }

    if (contest_state_machine_entered(&g_app_state)) {
        app_contest_template_apply_state_entry();
    }
}

void app_contest_template_set_command(int16_t target_speed, int16_t target_turn)
{
    g_command.target_speed = app_contest_template_clamp_i16(target_speed, -APP_OUTPUT_LIMIT, APP_OUTPUT_LIMIT);
    g_command.target_turn = app_contest_template_clamp_i16(target_turn, -APP_OUTPUT_LIMIT, APP_OUTPUT_LIMIT);
}

void app_contest_template_set_telemetry_enable(bool enabled)
{
    g_command.telemetry_enable = enabled;
}

void app_contest_template_clear_fault(void)
{
    if (g_app_state.current_state == APP_STATE_FAULT && !app_hw_read_fault_line()) {
        contest_state_machine_transition(&g_app_state, APP_STATE_STOP);
    }
}

uint8_t app_contest_template_state(void)
{
    return g_app_state.current_state;
}

const app_output_frame_t *app_contest_template_outputs(void)
{
    return &g_outputs;
}

static void app_contest_template_reset_drive(void)
{
    g_outputs.left_output = 0;
    g_outputs.right_output = 0;
}

static void app_contest_template_apply_state_entry(void)
{
    contest_led_pattern_stop(&g_status_pattern);

    switch (g_app_state.current_state) {
        case APP_STATE_STOP:
            app_contest_template_reset_drive();
            g_outputs.buzzer_on = false;
            g_outputs.status_led_on = false;
            (void)contest_led_pattern_start(&g_status_pattern, 1u, 9u, 0u);
            break;

        case APP_STATE_RUN:
            g_outputs.buzzer_on = false;
            g_outputs.status_led_on = true;
            break;

        case APP_STATE_TUNE:
            app_contest_template_reset_drive();
            g_outputs.buzzer_on = false;
            g_outputs.status_led_on = false;
            (void)contest_led_pattern_start(&g_status_pattern, 2u, 2u, 0u);
            break;

        case APP_STATE_FAULT:
        default:
            app_contest_template_reset_drive();
            g_outputs.buzzer_on = true;
            g_outputs.status_led_on = false;
            (void)contest_led_pattern_start(&g_status_pattern, 1u, 1u, 0u);
            break;
    }

    app_hw_apply_outputs(&g_outputs);
}

static void app_contest_template_poll_inputs(void)
{
    contest_button_event_t event;

    event = contest_button_update(&g_mode_button, app_hw_read_mode_button_level());

    if (app_hw_read_fault_line()) {
        contest_state_machine_transition(&g_app_state, APP_STATE_FAULT);
        return;
    }

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

static void app_contest_template_control_step(void)
{
    app_hw_sample_sensors(&g_inputs);

    if (g_inputs.emergency_stop) {
        contest_state_machine_transition(&g_app_state, APP_STATE_FAULT);
    }

    switch (g_app_state.current_state) {
        case APP_STATE_RUN:
            app_contest_template_run_control();
            g_outputs.buzzer_on = false;
            break;

        case APP_STATE_TUNE:
        case APP_STATE_STOP:
            app_contest_template_reset_drive();
            g_outputs.buzzer_on = false;
            break;

        case APP_STATE_FAULT:
        default:
            app_contest_template_reset_drive();
            g_outputs.buzzer_on = true;
            break;
    }

    app_hw_apply_outputs(&g_outputs);
}

static void app_contest_template_run_control(void)
{
    int32_t drive_term;
    int32_t line_correction;
    int32_t left_output;
    int32_t right_output;

    /*
     * This is only a placeholder control law.
     * Replace it with your actual speed loop, line-following, balance, or
     * sensor-fusion logic while keeping the surrounding application structure.
     */
    drive_term = (int32_t)g_command.target_speed - (int32_t)g_inputs.speed_feedback;
    line_correction = (int32_t)g_inputs.sensor_left - (int32_t)g_inputs.sensor_right;

    left_output = drive_term - (int32_t)g_command.target_turn - line_correction;
    right_output = drive_term + (int32_t)g_command.target_turn + line_correction;

    g_outputs.left_output = app_contest_template_clamp_i16(left_output, -APP_OUTPUT_LIMIT, APP_OUTPUT_LIMIT);
    g_outputs.right_output = app_contest_template_clamp_i16(right_output, -APP_OUTPUT_LIMIT, APP_OUTPUT_LIMIT);
}

static void app_contest_template_status_step(void)
{
    if (g_app_state.current_state == APP_STATE_RUN) {
        g_outputs.status_led_on = true;
    } else {
        (void)contest_led_pattern_tick(&g_status_pattern);
        g_outputs.status_led_on = contest_led_pattern_output(&g_status_pattern);
    }

    app_hw_apply_outputs(&g_outputs);
}

static void app_contest_template_send_telemetry(void)
{
    if (!g_command.telemetry_enable) {
        return;
    }

    app_hw_send_telemetry(g_app_state.current_state, &g_inputs, &g_outputs);
}

static int16_t app_contest_template_clamp_i16(int32_t value, int16_t minimum, int16_t maximum)
{
    if (value < (int32_t)minimum) {
        return minimum;
    }

    if (value > (int32_t)maximum) {
        return maximum;
    }

    return (int16_t)value;
}

static bool app_hw_read_mode_button_level(void)
{
    /* Replace with a real GPIO read or HAL button wrapper call. */
    return false;
}

static bool app_hw_read_fault_line(void)
{
    /* Replace with a stop switch, low-voltage lockout, or driver-fault input. */
    return false;
}

static void app_hw_sample_sensors(app_sensor_frame_t *inputs)
{
    if (inputs == 0) {
        return;
    }

    /*
     * Replace these placeholders with the real sensor path:
     * - ADC grayscale values
     * - encoder speed
     * - IMU angle
     * - battery voltage
     */
    inputs->emergency_stop = false;
    inputs->sensor_left = 0;
    inputs->sensor_right = 0;
    inputs->speed_feedback = 0;
    inputs->battery_mv = 12000u;
}

static void app_hw_apply_outputs(const app_output_frame_t *outputs)
{
    /*
     * Replace with your real actuator and indicator path:
     * - motor PWM write-back
     * - servo compare update
     * - status LED GPIO
     * - buzzer GPIO
     */
    (void)outputs;
}

static void app_hw_send_telemetry(
    uint8_t app_state,
    const app_sensor_frame_t *inputs,
    const app_output_frame_t *outputs
)
{
    /*
     * Replace with your preferred debug path:
     * - UART printf
     * - packet protocol
     * - upper-computer plotter
     */
    (void)app_state;
    (void)inputs;
    (void)outputs;
}
