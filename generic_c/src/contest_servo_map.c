#include "contest_servo_map.h"

#include "contest_common.h"

bool contest_servo_map_init(
    contest_servo_map_t *map,
    float angle_min_deg,
    float angle_max_deg,
    uint16_t compare_min,
    uint16_t compare_max
)
{
    if (map == 0 || angle_max_deg <= angle_min_deg || compare_max <= compare_min) {
        return false;
    }

    map->angle_min_deg = angle_min_deg;
    map->angle_max_deg = angle_max_deg;
    map->compare_min = compare_min;
    map->compare_max = compare_max;
    return true;
}

uint16_t contest_servo_map_angle_to_compare(const contest_servo_map_t *map, float angle_deg)
{
    float ratio;
    float compare_value;

    if (map == 0 || map->angle_max_deg <= map->angle_min_deg || map->compare_max <= map->compare_min) {
        return 0u;
    }

    angle_deg = contest_clampf(angle_deg, map->angle_min_deg, map->angle_max_deg);
    ratio = (angle_deg - map->angle_min_deg) / (map->angle_max_deg - map->angle_min_deg);
    compare_value = (float)map->compare_min + (ratio * (float)(map->compare_max - map->compare_min));
    return (uint16_t)compare_value;
}
