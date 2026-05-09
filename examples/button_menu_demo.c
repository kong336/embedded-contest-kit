#include <stdbool.h>
#include <stdint.h>

#include "../generic_c/include/contest_button.h"
#include "../generic_c/include/contest_state_machine.h"

enum {
    APP_STATE_IDLE = 0,
    APP_STATE_RUN = 1,
    APP_STATE_TUNE = 2
};

static contest_button_t g_mode_button;
static contest_state_machine_t g_app_state;

void app_button_menu_init(void)
{
    contest_button_init(&g_mode_button, true, 2u, 80u);
    contest_state_machine_init(&g_app_state, APP_STATE_IDLE);
}

void app_button_menu_step(bool raw_button_level)
{
    contest_button_event_t event;

    event = contest_button_update(&g_mode_button, raw_button_level);

    if (event & CONTEST_BUTTON_EVENT_CLICK) {
        if (g_app_state.current_state == APP_STATE_IDLE) {
            contest_state_machine_transition(&g_app_state, APP_STATE_RUN);
        } else {
            contest_state_machine_transition(&g_app_state, APP_STATE_IDLE);
        }
    }

    if (event & CONTEST_BUTTON_EVENT_LONG_PRESS) {
        contest_state_machine_transition(&g_app_state, APP_STATE_TUNE);
    }

    if (contest_state_machine_entered(&g_app_state)) {
        /*
         * Put one-time entry actions here, for example:
         * - clear screen
         * - beep once
         * - reset a parameter cursor
         */
    }

    contest_state_machine_tick(&g_app_state);
}
