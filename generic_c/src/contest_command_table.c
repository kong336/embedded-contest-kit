#include "contest_command_table.h"

bool contest_command_table_dispatch(
    const contest_command_entry_t *table,
    size_t entry_count,
    uint8_t cmd,
    const uint8_t *payload,
    size_t payload_len,
    void *user_context
)
{
    size_t i;

    if (table == 0 || entry_count == 0u) {
        return false;
    }

    for (i = 0u; i < entry_count; ++i) {
        if (table[i].cmd != cmd || table[i].handler == 0) {
            continue;
        }

        /* Centralized dispatch keeps packet parsing separate from application command handling. */
        return table[i].handler(cmd, payload, payload_len, user_context);
    }

    return false;
}
