#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../generic_c/include/contest_command_table.h"

enum {
    APP_CMD_SET_SPEED = 0x10,
    APP_CMD_SET_MODE = 0x20
};

typedef struct {
    int target_speed;
    int mode;
} app_command_context_t;

static bool app_handle_set_speed(uint8_t cmd, const uint8_t *payload, size_t payload_len, void *user_context)
{
    app_command_context_t *context = (app_command_context_t *)user_context;

    (void)cmd;
    if (context == 0 || payload == 0 || payload_len < 1u) {
        return false;
    }

    context->target_speed = (int)payload[0];
    return true;
}

static bool app_handle_set_mode(uint8_t cmd, const uint8_t *payload, size_t payload_len, void *user_context)
{
    app_command_context_t *context = (app_command_context_t *)user_context;

    (void)cmd;
    if (context == 0 || payload == 0 || payload_len < 1u) {
        return false;
    }

    context->mode = (int)payload[0];
    return true;
}

static const contest_command_entry_t g_command_table[] = {
    {APP_CMD_SET_SPEED, app_handle_set_speed},
    {APP_CMD_SET_MODE, app_handle_set_mode}
};

bool app_command_dispatch_demo(uint8_t cmd, const uint8_t *payload, size_t payload_len, app_command_context_t *context)
{
    return contest_command_table_dispatch(
        g_command_table,
        sizeof(g_command_table) / sizeof(g_command_table[0]),
        cmd,
        payload,
        payload_len,
        context
    );
}
