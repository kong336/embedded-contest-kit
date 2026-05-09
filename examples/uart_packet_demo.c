#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../generic_c/include/contest_packet.h"
#include "../generic_c/include/contest_ring_buffer.h"

static uint8_t uart_rx_storage[128];
static contest_ring_buffer_t uart_rx_buffer;

static void board_uart_send_bytes(const uint8_t *data, size_t length)
{
    (void)data;
    (void)length;
}

static void application_handle_frame(const contest_packet_frame_t *frame)
{
    (void)frame;
}

/*
 * Call this from the actual UART RX interrupt or callback.
 */
void board_uart_rx_byte_isr(uint8_t byte)
{
    (void)contest_ring_buffer_push(&uart_rx_buffer, byte);
}

/*
 * Call this from your main loop.
 */
void process_uart_packets(void)
{
    contest_packet_parser_t parser;
    contest_packet_frame_t frame;
    uint8_t byte;

    contest_packet_parser_init(&parser);

    while (contest_ring_buffer_pop(&uart_rx_buffer, &byte)) {
        if (contest_packet_parser_feed(&parser, byte, &frame)) {
            application_handle_frame(&frame);
        }
    }
}

/*
 * Example of sending a frame.
 */
void send_status_frame(uint8_t status_code, uint16_t speed_rpm)
{
    uint8_t payload[3];
    uint8_t frame[2 + 1 + 1 + CONTEST_PACKET_MAX_PAYLOAD + 2];
    size_t written = 0u;

    payload[0] = status_code;
    payload[1] = (uint8_t)(speed_rpm & 0xFFu);
    payload[2] = (uint8_t)((speed_rpm >> 8u) & 0xFFu);

    if (contest_packet_encode(0x10u, payload, 3u, frame, sizeof(frame), &written)) {
        board_uart_send_bytes(frame, written);
    }
}

int main(void)
{
    contest_ring_buffer_init(&uart_rx_buffer, uart_rx_storage, sizeof(uart_rx_storage));

    for (;;) {
        process_uart_packets();
    }
}
