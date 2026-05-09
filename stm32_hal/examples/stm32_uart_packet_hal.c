#include <stdint.h>

#include "../../generic_c/include/contest_packet.h"
#include "../../generic_c/include/contest_ring_buffer.h"
#include "../include/contest_hal_uart_packet.h"

extern UART_HandleTypeDef huart1;

static uint8_t rx_storage[128];
static contest_ring_buffer_t rx_buffer;
static contest_hal_uart_packet_t uart_context;

void app_uart_packet_init(void)
{
    contest_ring_buffer_init(&rx_buffer, rx_storage, sizeof(rx_storage));
    contest_hal_uart_packet_init(&uart_context, &huart1, &rx_buffer);
    (void)contest_hal_uart_packet_start_rx_it(&uart_context);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Hand the received byte to the wrapper and immediately re-arm RX interrupt. */
    (void)contest_hal_uart_packet_on_rx_complete(&uart_context, huart);
}

void app_uart_packet_poll(void)
{
    contest_packet_frame_t frame;

    while (contest_hal_uart_packet_process_byte_stream(&uart_context, &frame)) {
        /*
         * Put your command dispatch here, for example:
         * if (frame.cmd == 0x10u) { ... }
         */
    }
}
