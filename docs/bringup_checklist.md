# Bring-up Checklist

This page is for the first hour after you finish soldering or wiring a board.

## Recommended bring-up order

1. Power only
2. LED and buzzer
3. Serial print
4. Buttons
5. OLED
6. Motors without load
7. Encoders
8. ADC sensors
9. Ultrasonic or other timing-sensitive sensors
10. IMU
11. Closed-loop control

## Power checks

- confirm board voltage with a multimeter before connecting motors
- confirm regulator output is stable
- confirm ground is shared across MCU, motor driver, and sensors
- check motor supply polarity twice

## Minimal first firmware

Your first firmware should only do:

- blink one LED
- print one boot line on UART
- beep once if a buzzer exists

Do not start by enabling every peripheral at once.

## Motor checks

- test left and right motor separately
- confirm forward direction matches your code expectation
- confirm PWM range is not inverted
- start with small duty values first

## Encoder checks

- rotate wheel slowly by hand
- check count direction for forward rotation
- confirm counts increase smoothly instead of jumping wildly

## IMU checks

- log raw gyro and accelerometer values first
- confirm the board orientation matches your axis assumption
- keep the board still and watch offset drift before running control loops

## Sensor checks

- capture raw grayscale or ADC values before normalization
- log ultrasonic raw echo timing before distance conversion
- confirm button active level and bounce behavior

## Before enabling PID

- clamp all outputs
- add a simple stop condition
- add a timeout or fail-safe path
- confirm one loop tick is actually fixed

## Before a competition run

- mark left and right motor polarity on the chassis
- keep one known-good firmware tag
- write down the last stable parameters
- carry a UART debug build and a quiet run build
