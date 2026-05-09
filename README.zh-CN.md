# Embedded Contest Kit

[English](README.md)

一个面向电赛、嵌入式练习和小车控制题的基础代码仓库，重点放在可复用 C 模块、STM32 HAL 适配层，以及能直接照着改的示例。

## 这个仓库适合什么场景

- 想先有一套干净的速度环基础代码
- 想把按键、菜单、蜂鸣器、串口包这些常用功能先搭起来
- 想把算法层和板级 HAL 层分开，不想每次换板都重写

## 目录结构

- `generic_c/`：可移植的通用 C 模块
- `stm32_hal/`：面向 STM32 HAL 的适配层和示例
- `examples/`：通用示例，便于理解模块组合方式
- `docs/`：原理说明、项目配方、STM32 使用建议
- `tools/`：Windows 下的编译器辅助脚本和自检脚本

## 已包含的核心能力

### 控制与滤波

- `contest_pid`
- `contest_incremental_pi`
- `contest_moving_average`
- `contest_low_pass`
- `contest_complementary_angle`
- `contest_ramp`

### 传感与运动

- `contest_encoder`
- `contest_line_sensor`
- `contest_servo_map`
- `contest_ultrasonic`

### 交互与应用流转

- `contest_button`
- `contest_debounce`
- `contest_buzzer_pattern`
- `contest_menu`
- `contest_state_machine`
- `contest_periodic_task`
- `contest_soft_timer`

### 通信与缓冲

- `contest_command_table`
- `contest_ring_buffer`
- `contest_packet`
- `contest_crc16`

### STM32 HAL 适配

- `contest_hal_adc_scan`
- `contest_hal_motor`
- `contest_hal_uart_packet`
- `contest_hal_button`
- `contest_hal_encoder_timer`
- `contest_hal_buzzer`
- `contest_hal_servo`

## 直接可抄的组合

- 循迹小车：`contest_line_sensor` + `contest_pid` + `contest_ramp` + `contest_hal_adc_scan` + `contest_hal_motor`
- 编码器速度环：`contest_encoder` + `contest_low_pass` + `contest_incremental_pi` + `contest_hal_encoder_timer` + `contest_hal_motor`
- 按键菜单：`contest_button` + `contest_menu` + `contest_state_machine` + `contest_hal_button`
- 舵机扫描：`contest_servo_map` + `contest_ramp` + `contest_hal_servo`
- 超声测距：`contest_ultrasonic` + `contest_low_pass`
- 平衡车骨架：`contest_complementary_angle` + `contest_pid` + `contest_incremental_pi` + `contest_periodic_task`

更完整的组合建议在 [docs/project_recipes.md](docs/project_recipes.md)。

## 上手建议

1. 如果你用 STM32，先用 CubeMX 生成基础工程
2. 只拷你需要的模块，不要一股脑全塞进去
3. 把算法和协议逻辑放在 `generic_c/`
4. 把板级绑定和外设句柄放在 `stm32_hal/`
5. 把 `examples/` 当结构参考，不要当最终完整固件

## 本地验证

Windows 下可以这样快速检查可移植层：

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\verify_portable_build.ps1 -Compiler clang
powershell -ExecutionPolicy Bypass -File .\tools\verify_portable_build.ps1 -Compiler gcc
powershell -ExecutionPolicy Bypass -File .\tools\verify_portable_build.ps1 -Compiler cl
```

## 推荐先读

- [docs/module_principles.md](docs/module_principles.md)
- [docs/project_recipes.md](docs/project_recipes.md)
- [docs/stm32_hal_usage.md](docs/stm32_hal_usage.md)

## 故意不放进来的东西

- 每块板子完全不同的 CubeMX 初始化代码
- OLED 字库表
- MPU6050 或其他 IMU 的寄存器驱动
- 每一种板子都写一个完整大工程

这些部分强依赖板子和外设。这个仓库的目标，是把真正可复用的那层先沉淀下来。
