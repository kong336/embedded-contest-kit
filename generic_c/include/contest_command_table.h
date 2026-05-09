#ifndef CONTEST_COMMAND_TABLE_H
#define CONTEST_COMMAND_TABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef bool (*contest_command_handler_t)(
    uint8_t cmd,
    const uint8_t *payload,
    size_t payload_len,
    void *user_context
);

typedef struct {
    uint8_t cmd;
    contest_command_handler_t handler;
} contest_command_entry_t;

bool contest_command_table_dispatch(
    const contest_command_entry_t *table,
    size_t entry_count,
    uint8_t cmd,
    const uint8_t *payload,
    size_t payload_len,
    void *user_context
);

#endif
