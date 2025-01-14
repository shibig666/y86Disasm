# Y86 反汇编程序

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

**shibig666**

## 许可

该项目采用 MIT 许可协议，详情请参阅 [LICENSE](https://chatgpt.com/c/LICENSE)。
