#ifndef CONTEST_SERVO_MAP_H
#define CONTEST_SERVO_MAP_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    float angle_min_deg;
    float angle_max_deg;
    uint16_t compare_min;
    uint16_t compare_max;
} contest_servo_map_t;

bool contest_servo_map_init(
    contest_servo_map_t *map,
    float angle_min_deg,
    float angle_max_deg,
    uint16_t compare_min,
    uint16_t compare_max
);
uint16_t contest_servo_map_angle_to_compare(const contest_servo_map_t *map, float angle_deg);

#endif
