# rjsj_test

本项目使用 CMake 构建，测试框架为 Boost.Test。Boost 依赖通过 vcpkg 提供。普通构建可在 Windows、macOS 和 Linux 上使用；覆盖率构建使用 `--coverage` 和 `llvm-cov gcov`，因此要求 GCC 或 GNU-style Clang/AppleClang。

## 环境要求

- CMake 3.21 或更新版本
- 支持 C++20 的编译器
- vcpkg 中已安装 Boost.Test
- 需要生成覆盖率报告时，环境中需要可用的 `llvm-cov`

如果设置了 `VCPKG_ROOT` 环境变量，vcpkg toolchain 路径通常为：

```bash
$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

在 PowerShell 中可写为：

```powershell
$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake
```

如果没有设置 `VCPKG_ROOT`，请将下方命令中的占位符替换为本机 vcpkg 安装目录。

## 普通构建

普通构建不会插装覆盖率信息，适合日常编译和运行测试。

```bash
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

cmake --build build
ctest --test-dir build --output-on-failure
```

PowerShell 版本：

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"

cmake --build build
ctest --test-dir build --output-on-failure
```

## 覆盖率构建

覆盖率构建会为项目目标添加 `--coverage` 编译和链接参数，并提供 `coverage` 构建目标。该目标会先运行测试，再调用 `llvm-cov gcov` 生成覆盖率报告。

```bash
cmake -S . -B build-coverage \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -DRJSJ_ENABLE_COVERAGE=ON

cmake --build build-coverage --target coverage
```

PowerShell 版本：

```powershell
cmake -S . -B build-coverage `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake" `
  -DRJSJ_ENABLE_COVERAGE=ON

cmake --build build-coverage --target coverage
```

也可以使用等价目标：

```bash
cmake --build build-coverage --target llvm-cov-report
```

## 覆盖率报告位置

覆盖率报告生成在覆盖率构建目录下：

```text
build-coverage/coverage/
```

汇总文件为：

```text
build-coverage/coverage/llvm-cov-gcov-summary.txt
```

每个源文件或头文件的详细覆盖率会生成对应的 `.gcov` 文件。例如：

```text
build-coverage/coverage/#...#src#exercise_wb.cpp.gcov
build-coverage/coverage/#...#include#exercise.h.gcov
```
