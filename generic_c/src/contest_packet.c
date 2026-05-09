#include "contest_packet.h"

#include "contest_crc16.h"

void contest_packet_parser_init(contest_packet_parser_t *parser)
{
    contest_packet_parser_reset(parser);
}

void contest_packet_parser_reset(contest_packet_parser_t *parser)
{
    if (parser == 0) {
        return;
    }

    parser->state = CONTEST_PACKET_WAIT_HEAD1;
    parser->length = 0u;
    parser->cmd = 0u;
    parser->index = 0u;
    parser->crc_low = 0u;
}

bool contest_packet_encode(
    uint8_t cmd,
    const uint8_t *payload,
    uint8_t length,
    uint8_t *out_frame,
    size_t out_capacity,
    size_t *written_out
)
{
    size_t body_length;
    size_t frame_length;
    uint16_t crc;

    if (out_frame == 0 || written_out == 0 || length > CONTEST_PACKET_MAX_PAYLOAD) {
        return false;
    }
    if (length > 0u && payload == 0) {
        return false;
    }

    body_length = (size_t)length + 2u;
    frame_length = body_length + 4u;
    if (out_capacity < frame_length) {
        return false;
    }

    out_frame[0] = CONTEST_PACKET_HEAD1;
    out_frame[1] = CONTEST_PACKET_HEAD2;
    out_frame[2] = length;
    out_frame[3] = cmd;

    if (length > 0u) {
        size_t i;
        for (i = 0u; i < length; ++i) {
            out_frame[4u + i] = payload[i];
        }
    }

    /* Protect length + command + payload with CRC for safer transport. */
    crc = contest_crc16_modbus(&out_frame[2], body_length);
    out_frame[4u + length] = (uint8_t)(crc & 0xFFu);
    out_frame[5u + length] = (uint8_t)((crc >> 8u) & 0xFFu);
    *written_out = frame_length;
    return true;
}

bool contest_packet_parser_feed(
    contest_packet_parser_t *parser,
    uint8_t byte,
    contest_packet_frame_t *frame_out
)
{
    if (parser == 0 || frame_out == 0) {
        return false;
    }

    switch (parser->state) {
        case CONTEST_PACKET_WAIT_HEAD1:
            if (byte == CONTEST_PACKET_HEAD1) {
                parser->state = CONTEST_PACKET_WAIT_HEAD2;
            }
            break;

        case CONTEST_PACKET_WAIT_HEAD2:
            if (byte == CONTEST_PACKET_HEAD2) {
                parser->state = CONTEST_PACKET_WAIT_LENGTH;
            } else {
                parser->state = CONTEST_PACKET_WAIT_HEAD1;
            }
            break;

        case CONTEST_PACKET_WAIT_LENGTH:
            if (byte > CONTEST_PACKET_MAX_PAYLOAD) {
                parser->state = CONTEST_PACKET_WAIT_HEAD1;
            } else {
                /* Length is trusted only after header bytes were matched. */
                parser->length = byte;
                parser->index = 0u;
                parser->state = CONTEST_PACKET_WAIT_CMD;
            }
            break;

        case CONTEST_PACKET_WAIT_CMD:
            parser->cmd = byte;
            parser->state = parser->length == 0u ? CONTEST_PACKET_WAIT_CRC_LOW : CONTEST_PACKET_WAIT_PAYLOAD;
            break;

        case CONTEST_PACKET_WAIT_PAYLOAD:
            parser->payload[parser->index] = byte;
            parser->index += 1u;
            if (parser->index >= parser->length) {
                parser->state = CONTEST_PACKET_WAIT_CRC_LOW;
            }
            break;

        case CONTEST_PACKET_WAIT_CRC_LOW:
            parser->crc_low = byte;
            parser->state = CONTEST_PACKET_WAIT_CRC_HIGH;
            break;

        case CONTEST_PACKET_WAIT_CRC_HIGH:
        {
            uint8_t crc_buffer[2u + CONTEST_PACKET_MAX_PAYLOAD];
            uint16_t crc;
            size_t i;

            crc_buffer[0] = parser->length;
            crc_buffer[1] = parser->cmd;
            for (i = 0u; i < parser->length; ++i) {
                crc_buffer[2u + i] = parser->payload[i];
            }

            /* Compare transported CRC with locally recomputed CRC. */
            crc = contest_crc16_modbus(crc_buffer, (size_t)parser->length + 2u);
            if (parser->crc_low == (uint8_t)(crc & 0xFFu) && byte == (uint8_t)((crc >> 8u) & 0xFFu)) {
                frame_out->cmd = parser->cmd;
                frame_out->length = parser->length;
                for (i = 0u; i < parser->length; ++i) {
                    frame_out->payload[i] = parser->payload[i];
                }
                contest_packet_parser_reset(parser);
                return true;
            }

            contest_packet_parser_reset(parser);
            break;
        }

        default:
            contest_packet_parser_reset(parser);
            break;
    }

    return false;
}
