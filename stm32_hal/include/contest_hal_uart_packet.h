#ifndef CONTEST_HAL_UART_PACKET_H
#define CONTEST_HAL_UART_PACKET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../../generic_c/include/contest_packet.h"
#include "../../generic_c/include/contest_ring_buffer.h"
#include "contest_stm32_hal.h"

typedef struct {
    UART_HandleTypeDef *huart;
    contest_ring_buffer_t *rx_buffer;
    uint8_t rx_byte;
    contest_packet_parser_t parser;
} contest_hal_uart_packet_t;

bool contest_hal_uart_packet_init(
    contest_hal_uart_packet_t *context,
    UART_HandleTypeDef *huart,
    contest_ring_buffer_t *rx_buffer
);
bool contest_hal_uart_packet_start_rx_it(contest_hal_uart_packet_t *context);
bool contest_hal_uart_packet_on_rx_complete(contest_hal_uart_packet_t *context, UART_HandleTypeDef *huart);
bool contest_hal_uart_packet_process_byte_stream(
    contest_hal_uart_packet_t *context,
    contest_packet_frame_t *frame_out
);

#endif
