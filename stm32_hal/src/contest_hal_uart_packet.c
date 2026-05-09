#include "../include/contest_hal_uart_packet.h"

bool contest_hal_uart_packet_init(
    contest_hal_uart_packet_t *context,
    UART_HandleTypeDef *huart,
    contest_ring_buffer_t *rx_buffer
)
{
    if (context == 0 || huart == 0 || rx_buffer == 0) {
        return false;
    }

    /* The parser state lives here so the application loop can stay byte-stream oriented. */
    context->huart = huart;
    context->rx_buffer = rx_buffer;
    context->rx_byte = 0u;
    contest_packet_parser_init(&context->parser);
    return true;
}

bool contest_hal_uart_packet_start_rx_it(contest_hal_uart_packet_t *context)
{
    if (context == 0 || context->huart == 0) {
        return false;
    }

    return HAL_UART_Receive_IT(context->huart, &context->rx_byte, 1u) == HAL_OK;
}

bool contest_hal_uart_packet_on_rx_complete(contest_hal_uart_packet_t *context, UART_HandleTypeDef *huart)
{
    if (context == 0 || context->huart == 0 || huart != context->huart) {
        return false;
    }

    /* Move the just-received byte into the software FIFO, then arm the next RX. */
    (void)contest_ring_buffer_push(context->rx_buffer, context->rx_byte);
    return HAL_UART_Receive_IT(context->huart, &context->rx_byte, 1u) == HAL_OK;
}

bool contest_hal_uart_packet_process_byte_stream(
    contest_hal_uart_packet_t *context,
    contest_packet_frame_t *frame_out
)
{
    uint8_t byte;

    if (context == 0 || context->rx_buffer == 0 || frame_out == 0) {
        return false;
    }

    while (contest_ring_buffer_pop(context->rx_buffer, &byte)) {
        /* Feed bytes one by one until a complete valid frame is reconstructed. */
        if (contest_packet_parser_feed(&context->parser, byte, frame_out)) {
            return true;
        }
    }

    return false;
}
