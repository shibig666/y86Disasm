/*
 * 作者: Shibig666
 * 日期: 2024年12月11日
 * 说明: Y86反汇编器。
 */

#include "y86disasm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 错误退出
void exit_with_error(const char *message, FILE *file1, FILE *file2,
                     uint8_t *buffer) {
  if (file1)
    fclose(file1);
  if (file2)
    fclose(file2);
  if (buffer)
    free(buffer);
  fprintf(stderr, "%s\n", message);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <y86 input file> <y86 asm output file>\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  const char *input_filename = argv[1];
  const char *output_filename = argv[2];

  // const char *input_filename = "../test/prog3.o";
  // const char *output_filename = "../test/prog3.txt";

  // 打开输入文件
  FILE *input_file = fopen(input_filename, "rb");
  if (!input_file) {
    fprintf(stderr, "Error: Cannot open input file %s\n", input_filename);
    return EXIT_FAILURE;
  }

  // 打开输出文件
  FILE *output_file = fopen(output_filename, "w");
  if (!output_file) {
    exit_with_error("Error: Cannot open output file", input_file, NULL, NULL);
  }

  // 获取文件大小
  fseek(input_file, 0, SEEK_END);
  long file_size = ftell(input_file);
  if (file_size < 0) {
    exit_with_error("Error: Failed to get file size", input_file, output_file,
                    NULL);
  }
  fseek(input_file, 0, SEEK_SET);

  // 分配内存
  uint8_t *code = (uint8_t *)malloc(file_size);
  if (!code) {
    exit_with_error("Error: Memory allocation failed", input_file, output_file,
                    NULL);
  }

  // 读取文件内容
  if (fread(code, 1, file_size, input_file) != (size_t)file_size) {
    exit_with_error("Error: Failed to read input file", input_file, output_file,
                    code);
  }
  fclose(input_file);

  // 动态分配用于保存反汇编结果的缓冲区
  char *buffer = (char *)malloc(file_size * 10); // 假设每字节最多需要 10 字节
  if (!buffer) {
    exit_with_error("Error: Buffer allocation failed", NULL, output_file, code);
  }
  memset(buffer, 0, file_size * 10);
  // 反汇编
  disasm(code, buffer, file_size);

  // 写入到输出文件
  if (fprintf(output_file, "%s", buffer) < 0) {
    exit_with_error("Error: Failed to write to output file", NULL, output_file,
                    code);
  }

  // 释放资源
  fclose(output_file);
  free(code);
  free(buffer);

  return EXIT_SUCCESS;
}
