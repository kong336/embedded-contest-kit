#include "contest_menu.h"

bool contest_menu_init(
    contest_menu_t *menu,
    const char *title,
    const char *const *items,
    size_t item_count,
    size_t visible_rows
)
{
    if (menu == 0 || items == 0 || item_count == 0u || visible_rows == 0u) {
        return false;
    }

    menu->title = title;
    menu->items = items;
    menu->item_count = item_count;
    menu->visible_rows = visible_rows;
    menu->selected_index = 0u;
    menu->top_index = 0u;
    return true;
}

void contest_menu_move_next(contest_menu_t *menu)
{
    if (menu == 0 || menu->item_count == 0u) {
        return;
    }

    if (menu->selected_index + 1u < menu->item_count) {
        menu->selected_index += 1u;
    }

    if (menu->selected_index >= menu->top_index + menu->visible_rows) {
        menu->top_index = menu->selected_index + 1u - menu->visible_rows;
    }
}

void contest_menu_move_prev(contest_menu_t *menu)
{
    if (menu == 0 || menu->item_count == 0u) {
        return;
    }

    if (menu->selected_index > 0u) {
        menu->selected_index -= 1u;
    }

    if (menu->selected_index < menu->top_index) {
        menu->top_index = menu->selected_index;
    }
}

bool contest_menu_set_index(contest_menu_t *menu, size_t index)
{
    if (menu == 0 || index >= menu->item_count) {
        return false;
    }

    menu->selected_index = index;

    if (menu->selected_index < menu->top_index) {
        menu->top_index = menu->selected_index;
    } else if (menu->selected_index >= menu->top_index + menu->visible_rows) {
        menu->top_index = menu->selected_index + 1u - menu->visible_rows;
    }

    return true;
}

const char *contest_menu_current_label(const contest_menu_t *menu)
{
    if (menu == 0 || menu->items == 0 || menu->selected_index >= menu->item_count) {
        return "";
    }

    return menu->items[menu->selected_index];
}

size_t contest_menu_visible_end(const contest_menu_t *menu)
{
    size_t end_index;

    if (menu == 0) {
        return 0u;
    }

    end_index = menu->top_index + menu->visible_rows;
    if (end_index > menu->item_count) {
        end_index = menu->item_count;
    }

    return end_index;
}
