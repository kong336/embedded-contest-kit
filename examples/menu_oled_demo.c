#include <stddef.h>

#include "../generic_c/include/contest_menu.h"

static const char *g_menu_items[] = {
    "Start",
    "Speed",
    "Sensor",
    "Servo",
    "About"
};

static contest_menu_t g_menu;

void app_menu_demo_init(void)
{
    contest_menu_init(
        &g_menu,
        "Main Menu",
        g_menu_items,
        sizeof(g_menu_items) / sizeof(g_menu_items[0]),
        3u
    );
}

void app_menu_demo_on_key_next(void)
{
    contest_menu_move_next(&g_menu);
}

void app_menu_demo_on_key_prev(void)
{
    contest_menu_move_prev(&g_menu);
}

/*
 * Replace these prototypes with your actual OLED drawing functions.
 * The menu logic stays generic; only the renderer changes between boards.
 */
extern void oled_clear(void);
extern void oled_draw_text(int row, const char *text);

void app_menu_demo_render(void)
{
    size_t row;
    size_t item_index;
    size_t end_index;

    oled_clear();
    oled_draw_text(0, g_menu.title);

    end_index = contest_menu_visible_end(&g_menu);
    row = 1u;
    for (item_index = g_menu.top_index; item_index < end_index; ++item_index) {
        oled_draw_text((int)row, g_menu.items[item_index]);
        row += 1u;
    }
}
