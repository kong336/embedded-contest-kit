# Tools

This folder contains simple local helpers for using the installed Windows compilers.

- `use_compilers.ps1`: add `clang` and `gcc` to the current PowerShell session and print quick-use hints
- `open_msvc_shell.bat`: open a command prompt with MSVC `cl` environment already prepared
- `verify_portable_build.ps1`: compile all portable sources and examples with `gcc`, `clang`, or `cl`
- `check_easyeda_gateway.ps1`: probe the local EasyEDA bridge on ports `49620-49629` and print connected windows
