#ifndef CONTEST_BUTTON_H
#define CONTEST_BUTTON_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    CONTEST_BUTTON_EVENT_NONE = 0u,
    CONTEST_BUTTON_EVENT_PRESS = 1u << 0,
    CONTEST_BUTTON_EVENT_RELEASE = 1u << 1,
    CONTEST_BUTTON_EVENT_CLICK = 1u << 2,
    CONTEST_BUTTON_EVENT_LONG_PRESS = 1u << 3
} contest_button_event_t;

typedef struct {
    bool active_level;
    bool stable_level;
    bool last_raw_level;
    bool pressed;
    bool long_reported;
    uint16_t debounce_ticks;
    uint16_t long_press_ticks;
    uint16_t raw_stable_ticks;
    uint16_t pressed_ticks;
} contest_button_t;

void contest_button_init(
    contest_button_t *button,
    bool active_level,
    uint16_t debounce_ticks,
    uint16_t long_press_ticks
);
void contest_button_reset(contest_button_t *button);
contest_button_event_t contest_button_update(contest_button_t *button, bool raw_level);
bool contest_button_is_pressed(const contest_button_t *button);

#endif
