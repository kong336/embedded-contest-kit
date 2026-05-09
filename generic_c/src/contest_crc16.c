#include "contest_crc16.h"

uint16_t contest_crc16_modbus(const uint8_t *data, size_t length)
{
    size_t i;
    uint16_t crc = 0xFFFFu;

    for (i = 0; i < length; ++i) {
        uint8_t bit;
        crc ^= data[i];
        for (bit = 0; bit < 8u; ++bit) {
            if (crc & 0x0001u) {
                crc = (uint16_t)((crc >> 1u) ^ 0xA001u);
            } else {
                crc >>= 1u;
            }
        }
    }

    return crc;
}

uint16_t contest_crc16_ccitt_false(const uint8_t *data, size_t length)
{
    size_t i;
    uint16_t crc = 0xFFFFu;

    for (i = 0; i < length; ++i) {
        uint8_t bit;
        crc ^= (uint16_t)(data[i] << 8u);
        for (bit = 0; bit < 8u; ++bit) {
            if (crc & 0x8000u) {
                crc = (uint16_t)((crc << 1u) ^ 0x1021u);
            } else {
                crc <<= 1u;
            }
        }
    }

    return crc;
}
