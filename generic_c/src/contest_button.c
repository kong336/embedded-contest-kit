#include "contest_button.h"

#include <limits.h>

void contest_button_init(
    contest_button_t *button,
    bool active_level,
    uint16_t debounce_ticks,
    uint16_t long_press_ticks
)
{
    if (button == 0) {
        return;
    }

    button->active_level = active_level;
    button->stable_level = !active_level;
    button->last_raw_level = !active_level;
    button->pressed = false;
    button->long_reported = false;
    button->debounce_ticks = debounce_ticks;
    button->long_press_ticks = long_press_ticks;
    button->raw_stable_ticks = 0u;
    button->pressed_ticks = 0u;
}

void contest_button_reset(contest_button_t *button)
{
    if (button == 0) {
        return;
    }

    button->stable_level = !button->active_level;
    button->last_raw_level = !button->active_level;
    button->pressed = false;
    button->long_reported = false;
    button->raw_stable_ticks = 0u;
    button->pressed_ticks = 0u;
}

contest_button_event_t contest_button_update(contest_button_t *button, bool raw_level)
{
    contest_button_event_t event = CONTEST_BUTTON_EVENT_NONE;

    if (button == 0) {
        return CONTEST_BUTTON_EVENT_NONE;
    }

    if (raw_level != button->last_raw_level) {
        button->last_raw_level = raw_level;
        button->raw_stable_ticks = 0u;
    } else if (button->raw_stable_ticks < UINT16_MAX) {
        button->raw_stable_ticks += 1u;
    }

    if (button->stable_level != raw_level && button->raw_stable_ticks >= button->debounce_ticks) {
        button->stable_level = raw_level;

        if (raw_level == button->active_level) {
            button->pressed = true;
            button->pressed_ticks = 0u;
            button->long_reported = false;
            event = (contest_button_event_t)(event | CONTEST_BUTTON_EVENT_PRESS);
        } else {
            button->pressed = false;
            event = (contest_button_event_t)(event | CONTEST_BUTTON_EVENT_RELEASE);

            /* A short click is simply a release that happened before the long-press threshold. */
            if (!button->long_reported) {
                event = (contest_button_event_t)(event | CONTEST_BUTTON_EVENT_CLICK);
            }

            button->pressed_ticks = 0u;
            button->long_reported = false;
        }
    }

    if (button->pressed) {
        if (button->pressed_ticks < UINT16_MAX) {
            button->pressed_ticks += 1u;
        }

        if (!button->long_reported && button->pressed_ticks >= button->long_press_ticks) {
            button->long_reported = true;
            event = (contest_button_event_t)(event | CONTEST_BUTTON_EVENT_LONG_PRESS);
        }
    }

    return event;
}

bool contest_button_is_pressed(const contest_button_t *button)
{
    if (button == 0) {
        return false;
    }

    return button->pressed;
}
