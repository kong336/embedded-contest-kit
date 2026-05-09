#include <stdint.h>

#include "../../generic_c/include/contest_state_machine.h"
#include "../include/contest_hal_button.h"

enum {
    APP_STATE_STOP = 0,
    APP_STATE_RUN = 1,
    APP_STATE_MENU = 2
};

static contest_hal_button_t g_mode_button;
static contest_state_machine_t g_app_state;

void app_stm32_button_init(void)
{
    contest_hal_button_init(&g_mode_button, GPIOB, GPIO_PIN_12, false, 2u, 80u);
    contest_state_machine_init(&g_app_state, APP_STATE_STOP);
}

void app_stm32_button_poll(void)
{
    contest_button_event_t event;

    event = contest_hal_button_poll(&g_mode_button);

    if (event & CONTEST_BUTTON_EVENT_CLICK) {
        if (g_app_state.current_state == APP_STATE_STOP) {
            contest_state_machine_transition(&g_app_state, APP_STATE_RUN);
        } else {
            contest_state_machine_transition(&g_app_state, APP_STATE_STOP);
        }
    }

    if (event & CONTEST_BUTTON_EVENT_LONG_PRESS) {
        contest_state_machine_transition(&g_app_state, APP_STATE_MENU);
    }

    if (contest_state_machine_entered(&g_app_state)) {
        /*
         * Put one-time mode entry work here, for example:
         * - clear alarms
         * - refresh OLED page
         * - re-arm a task timer
         */
    }

    contest_state_machine_tick(&g_app_state);
}
