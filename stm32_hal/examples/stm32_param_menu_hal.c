#include <stdbool.h>
#include <stdio.h>

#include "../../generic_c/include/contest_button.h"
#include "../../generic_c/include/contest_menu.h"
#include "../../generic_c/include/contest_value_tuner.h"
#include "../include/contest_hal_button.h"

extern void oled_clear(void);
extern void oled_draw_text(int row, const char *text);
extern void oled_update(void);

static const char *g_menu_items[] = {
    "kp",
    "ki",
    "speed"
};

static contest_menu_t g_menu;
static contest_value_tuner_t g_kp_tuner;
static contest_value_tuner_t g_ki_tuner;
static contest_value_tuner_t g_speed_tuner;
static contest_hal_button_t g_key_next;
static contest_hal_button_t g_key_prev;
static contest_hal_button_t g_key_inc;
static contest_hal_button_t g_key_dec;
static bool g_fine_mode = false;

static contest_value_tuner_t *app_current_tuner(void)
{
    if (g_menu.selected_index == 0u) {
        return &g_kp_tuner;
    }

    if (g_menu.selected_index == 1u) {
        return &g_ki_tuner;
    }

    return &g_speed_tuner;
}

void app_stm32_param_menu_init(void)
{
    contest_menu_init(&g_menu, "Tune", g_menu_items, sizeof(g_menu_items) / sizeof(g_menu_items[0]), 3u);
    contest_value_tuner_init(&g_kp_tuner, "kp", 20.0f, 0.0f, 80.0f, 1.0f, 0.1f);
    contest_value_tuner_init(&g_ki_tuner, "ki", 0.20f, 0.0f, 3.0f, 0.10f, 0.01f);
    contest_value_tuner_init(&g_speed_tuner, "spd", 180.0f, 0.0f, 500.0f, 10.0f, 1.0f);

    contest_hal_button_init(&g_key_next, GPIOB, GPIO_PIN_12, false, 2u, 80u);
    contest_hal_button_init(&g_key_prev, GPIOB, GPIO_PIN_13, false, 2u, 80u);
    contest_hal_button_init(&g_key_inc, GPIOB, GPIO_PIN_14, false, 2u, 80u);
    contest_hal_button_init(&g_key_dec, GPIOB, GPIO_PIN_15, false, 2u, 80u);
}

void app_stm32_param_menu_poll_and_render(void)
{
    char line_buffer[32];
    contest_button_event_t event;
    contest_value_tuner_t *tuner;

    event = contest_hal_button_poll(&g_key_next);
    if (event & CONTEST_BUTTON_EVENT_CLICK) {
        contest_menu_move_next(&g_menu);
    }
    if (event & CONTEST_BUTTON_EVENT_LONG_PRESS) {
        g_fine_mode = !g_fine_mode;
    }

    if (contest_hal_button_poll(&g_key_prev) & CONTEST_BUTTON_EVENT_CLICK) {
        contest_menu_move_prev(&g_menu);
    }

    tuner = app_current_tuner();
    if (contest_hal_button_poll(&g_key_inc) & CONTEST_BUTTON_EVENT_CLICK) {
        (void)contest_value_tuner_adjust(tuner, 1, g_fine_mode);
    }

    if (contest_hal_button_poll(&g_key_dec) & CONTEST_BUTTON_EVENT_CLICK) {
        (void)contest_value_tuner_adjust(tuner, -1, g_fine_mode);
    }

    oled_clear();
    oled_draw_text(0, g_menu.title);

    (void)snprintf(line_buffer, sizeof(line_buffer), "kp %.2f", (double)g_kp_tuner.value);
    oled_draw_text(1, line_buffer);
    (void)snprintf(line_buffer, sizeof(line_buffer), "ki %.2f", (double)g_ki_tuner.value);
    oled_draw_text(2, line_buffer);
    (void)snprintf(line_buffer, sizeof(line_buffer), "spd %.1f", (double)g_speed_tuner.value);
    oled_draw_text(3, line_buffer);

    oled_update();
}
