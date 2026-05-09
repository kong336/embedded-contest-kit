#include <stddef.h>

#include "../../generic_c/include/contest_menu.h"
#include "../include/contest_hal_button.h"

/*
 * This example keeps the menu logic generic and assumes you already have
 * a small OLED driver such as SSD1306 or SH1106 in your project.
 */

extern void oled_clear(void);
extern void oled_draw_text(int row, const char *text);
extern void oled_update(void);

static const char *g_menu_items[] = {
    "Run",
    "Tune",
    "Servo",
    "Status",
    "About"
};

static contest_menu_t g_menu;
static contest_hal_button_t g_key_next;
static contest_hal_button_t g_key_prev;

void app_stm32_menu_init(void)
{
    contest_menu_init(&g_menu, "Main Menu", g_menu_items, sizeof(g_menu_items) / sizeof(g_menu_items[0]), 3u);
    contest_hal_button_init(&g_key_next, GPIOB, GPIO_PIN_12, false, 2u, 80u);
    contest_hal_button_init(&g_key_prev, GPIOB, GPIO_PIN_13, false, 2u, 80u);
}

void app_stm32_menu_poll_and_render(void)
{
    size_t row;
    size_t item_index;
    size_t end_index;

    if (contest_hal_button_poll(&g_key_next) & CONTEST_BUTTON_EVENT_CLICK) {
        contest_menu_move_next(&g_menu);
    }

    if (contest_hal_button_poll(&g_key_prev) & CONTEST_BUTTON_EVENT_CLICK) {
        contest_menu_move_prev(&g_menu);
    }

    oled_clear();
    oled_draw_text(0, g_menu.title);

    end_index = contest_menu_visible_end(&g_menu);
    row = 1u;
    for (item_index = g_menu.top_index; item_index < end_index; ++item_index) {
        oled_draw_text((int)row, g_menu.items[item_index]);
        row += 1u;
    }

    oled_update();
}
