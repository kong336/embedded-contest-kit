#ifndef CONTEST_CRC16_H
#define CONTEST_CRC16_H

#include <stddef.h>
#include <stdint.h>

uint16_t contest_crc16_modbus(const uint8_t *data, size_t length);
uint16_t contest_crc16_ccitt_false(const uint8_t *data, size_t length);

#endif
