#ifndef CONTEST_MENU_H
#define CONTEST_MENU_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    const char *title;
    const char *const *items;
    size_t item_count;
    size_t visible_rows;
    size_t selected_index;
    size_t top_index;
} contest_menu_t;

bool contest_menu_init(
    contest_menu_t *menu,
    const char *title,
    const char *const *items,
    size_t item_count,
    size_t visible_rows
);
void contest_menu_move_next(contest_menu_t *menu);
void contest_menu_move_prev(contest_menu_t *menu);
bool contest_menu_set_index(contest_menu_t *menu, size_t index);
const char *contest_menu_current_label(const contest_menu_t *menu);
size_t contest_menu_visible_end(const contest_menu_t *menu);

#endif
