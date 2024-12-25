# Y86 Disassembler

**CFS2101-FNA-YEAR-2425 Assignment** by **Yuxin Shi**

## Introduction

This is a Y86 disassembler that converts Y86 binary programs into readable assembly code.

### Features

- Disassembles the Y86 instruction set
- Disassembles register operations
- Disassembles immediate values
- Generates labels for better readability
- Recognizes and differentiates data areas

## Usage

### Build

#### Using `make` to build

```bash
make
```

#### Using `CMake` to build

```bash
mkdir build && cd build
cmake ..
make
```

### Run

#### Disassembler Program `y86disassembler`

Usage:

```bash
Usage: ./y86disassembler <y86 input file> <y86 asm output file>
```

- `<y86 input file>`: The input Y86 binary file
- `<y86 asm output file>`: The output Y86 assembly file

#### Hexadecimal to Binary Converter `hex2bin`

Usage:

```bash
Usage: ./hex2bin <input file> <output file>
```

- `<input file>`: The input hexadecimal text file
- `<output file>`: The output binary file

## Author

**Yuxin Shi**

## License

This project is licensed under the MIT License. See the [LICENSE](https://chatgpt.com/c/LICENSE) file for more details.

------

# Y86 反汇编程序

**CFS2101-FNA-YEAR-2425 作业** 由 **Yuxin Shi** 完成

## 介绍

这是一个 Y86 反汇编工具，用于将 Y86 二进制格式的程序转换为可读的汇编代码。

### 功能

- 支持 Y86 指令集的反汇编
- 自动反汇编寄存器操作
- 支持立即数的反汇编
- 自动生成标签，提升代码可读性
- 能够识别并区分数据区域

## 使用说明

### 构建

#### 使用 `make` 构建

```bash
make
```

#### 使用 `CMake` 构建

```bash
mkdir build && cd build
cmake ..
make
```

### 运行

#### 反汇编程序 `y86disassembler`

用法：

```bash
Usage: ./y86disassembler <y86 input file> <y86 asm output file>
```

- `<y86 input file>`：输入的 Y86 二进制文件
- `<y86 asm output file>`：输出的 Y86 汇编文件

#### 十六进制转换工具 `hex2bin`

用法：

```bash
Usage: ./hex2bin <input file> <output file>
```

- `<input file>`：输入的十六进制文本文件
- `<output file>`：输出的二进制文件

## 作者

**Yuxin Shi**

## 许可

该项目采用 MIT 许可协议，详情请参阅 [LICENSE](https://chatgpt.com/c/LICENSE)。