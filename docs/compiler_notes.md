# Compiler Notes

## gcc / clang / cl 是什么

### gcc

- 全称通常说 GNU C Compiler
- 最常见的开源 C/C++ 编译器之一
- 嵌入式、Linux、交叉编译、开源工具链里非常常见

你会在这些场景里经常见到它：

- `arm-none-eabi-gcc`
- `riscv-none-elf-gcc`
- 普通桌面环境下的 `gcc`

对你来说，`gcc` 的意义是：

- 很多嵌入式例程和 Makefile 都默认围着它写
- 以后学交叉编译和裸机工程时几乎绕不开

### clang

- LLVM 体系里的 C/C++ 编译器前端
- 诊断信息通常更友好
- 在现代工具链、静态分析、IDE 集成里很常见

对你来说，`clang` 的意义是：

- 写通用 C 代码时很好用
- 查警告、看代码规范问题时体验常常更好

### cl

- 微软 MSVC 的命令行编译器
- 主要在 Windows / Visual Studio 生态里使用

对你来说，`cl` 的意义是：

- 如果你以后写 Windows 本地程序、某些只支持 MSVC 的项目，就会碰到它
- 对纯 MCU 固件本身不是第一优先，但在 Windows 上做 C/C++ 开发时很有用

## 对电赛和嵌入式来说怎么排序

如果按你当前最实用的顺序：

1. `gcc`
2. `clang`
3. `cl`

原因很简单：

- 嵌入式和交叉编译最常见的是 GCC 系
- Clang 很适合做本地代码检查和通用 C 代码验证
- `cl` 更偏 Windows 本地开发工具链

## 为什么我还是建议三个都知道

- `gcc` 帮你接近嵌入式真实工程
- `clang` 帮你写出更干净的通用代码
- `cl` 帮你不怕 Windows 开发环境

知道这三个的定位后，你以后看教程就不会老被工具名吓到。

## 这台电脑上我已经帮你装了什么

- `clang`: LLVM 22.1.5
- `gcc`: WinLibs GCC 16.1.0
- `cl`: Visual Studio Build Tools 2022 的 MSVC 编译器

## 为什么现在终端里可能还直接敲不出来

`winget` 装完以后，当前已经开着的终端窗口不一定会自动刷新环境变量。

所以你现在有三种最省事的用法：

1. 重新开一个终端
2. 运行 `contest-kit/tools/use_compilers.ps1`
3. 双击或运行 `contest-kit/tools/open_msvc_shell.bat`

## 你现在的实际安装位置

### clang

- `C:\Program Files\LLVM\bin\clang.exe`

### gcc

- `C:\Users\allen\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\gcc.exe`

### cl

- `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat`
- `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe`

## 平时怎么选

- 写通用 C、小工具、算法验证：优先 `clang`
- 看嵌入式例程、Makefile、交叉编译思路：优先 `gcc`
- 做 Windows 本地 C/C++ 工程、MSVC 兼容项目：用 `cl`

## 对你现在最实用的一句话建议

如果你最近主要搞单片机、电赛、控制代码：

1. 先会用 `gcc`
2. 再拿 `clang` 做代码检查
3. `cl` 当成 Windows 生态补充工具
