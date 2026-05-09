#ifndef CONTEST_PACKET_H
#define CONTEST_PACKET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CONTEST_PACKET_HEAD1 0x55u
#define CONTEST_PACKET_HEAD2 0xAAu
#define CONTEST_PACKET_MAX_PAYLOAD 64u

typedef struct {
    uint8_t cmd;
    uint8_t length;
    uint8_t payload[CONTEST_PACKET_MAX_PAYLOAD];
} contest_packet_frame_t;

typedef enum {
    CONTEST_PACKET_WAIT_HEAD1 = 0,
    CONTEST_PACKET_WAIT_HEAD2,
    CONTEST_PACKET_WAIT_LENGTH,
    CONTEST_PACKET_WAIT_CMD,
    CONTEST_PACKET_WAIT_PAYLOAD,
    CONTEST_PACKET_WAIT_CRC_LOW,
    CONTEST_PACKET_WAIT_CRC_HIGH
} contest_packet_state_t;

typedef struct {
    contest_packet_state_t state;
    uint8_t length;
    uint8_t cmd;
    uint8_t index;
    uint8_t crc_low;
    uint8_t payload[CONTEST_PACKET_MAX_PAYLOAD];
} contest_packet_parser_t;

void contest_packet_parser_init(contest_packet_parser_t *parser);
void contest_packet_parser_reset(contest_packet_parser_t *parser);
bool contest_packet_encode(
    uint8_t cmd,
    const uint8_t *payload,
    uint8_t length,
    uint8_t *out_frame,
    size_t out_capacity,
    size_t *written_out
);
bool contest_packet_parser_feed(
    contest_packet_parser_t *parser,
    uint8_t byte,
    contest_packet_frame_t *frame_out
);

#endif
