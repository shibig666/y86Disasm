#include "y86disasm.h"
#include "y86registor.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_instruction_name(uint8_t num) {
  switch (num) {
  case 0x00:
    return "halt";
  case 0x10:
    return "nop";
  case 0x20:
    return "rrmovl";
  case 0x21:
    return "cmovle";
  case 0x22:
    return "cmovl";
  case 0x23:
    return "cmove";
  case 0x24:
    return "cmovne";
  case 0x25:
    return "cmovge";
  case 0x26:
    return "cmovg";
  case 0x30:
    return "irmovl";
  case 0x40:
    return "rmmovl";
  case 0x50:
    return "mrmovl";
  case 0x60:
    return "addl";
  case 0x61:
    return "subl";
  case 0x62:
    return "andl";
  case 0x63:
    return "xorl";
  case 0x70:
    return "jmp";
  case 0x71:
    return "jle";
  case 0x72:
    return "jl";
  case 0x73:
    return "je";
  case 0x74:
    return "jne";
  case 0x75:
    return "jge";
  case 0x76:
    return "jg";
  case 0x80:
    return "call";
  case 0x90:
    return "ret";
  case 0xA0:
    return "pushl";
  case 0xB0:
    return "popl";
  default:
    return "Invalid";
  }
}

int get_instruction_len(uint8_t num) {
  if (num == 0x00 || num == 0x10 || num == 0x90)
    return 1;
  else if (num == 0x30 || num == 0x40 || num == 0x50)
    return 6;
  else if (num >= 0x70 && num <= 0x76 || num == 0x80)
    return 5;
  else
    return 2;
}

void write_irmovl(uint8_t *code, char *buffer, int *label_addrs, int *len) {
  if (code[1] / 16 != 0xF) {
    sprintf(buffer, "Invalid");
    printf("Invalid instruction on irmovl\n");
  }
  char *reg = get_reg_val(code[1] & 0xF);
  int value = imm_to_num(code + 2);
  for (int i = 0; i < *len; i++) {
    if (label_addrs[i] == value) {
      sprintf(buffer, "irmovl L%d,%s", i, reg);
      return;
    }
  }
  sprintf(buffer, "irmovl $%d,%s", value, reg);
}

void write_mrmovl(uint8_t *code, char *buffer) {
  char *reg1 = get_reg_val(code[1] >> 4);
  char *reg2 = get_reg_val(code[1] & 0xF);
  int32_t value = imm_to_num(code + 2);
  char *op = get_instruction_name(code[0]);
  if (value == 0)
    sprintf(buffer, "%s (%s),%s", op, reg2, reg1);
  else
    sprintf(buffer, "%s %d(%s),%s", op, value, reg2, reg1);
}

void write_rrmovl(uint8_t *code, char *buffer) {
  char *reg1 = get_reg_val(code[1] >> 4);
  char *reg2 = get_reg_val(code[1] & 0xF);
  char *op = get_instruction_name(code[0]);
  sprintf(buffer, "%s %s,%s", op, reg1, reg2);
}

void write_rmmovl(uint8_t *code, char *buffer) {
  char *reg1 = get_reg_val(code[1] >> 4);
  char *reg2 = get_reg_val(code[1] & 0xF);
  int32_t value = imm_to_num(code + 2);
  char *op = get_instruction_name(code[0]);
  if (value == 0)
    sprintf(buffer, "%s (%s),%s", op, reg2, reg1);
  else
    sprintf(buffer, "%s %s,%d(%s)", op, reg1, value, reg2);
}

void write_opl_or_cmovxx(uint8_t *code, char *buffer) {
  char *reg1 = get_reg_val(code[1] >> 4);
  char *reg2 = get_reg_val(code[1] & 0xF);
  char *op = get_instruction_name(code[0]);
  sprintf(buffer, "%s %s,%s", op, reg1, reg2);
}

void write_pp(uint8_t *code, char *buffer) {
  if (code[1] % 16 != 0xF) {
    printf("Invalid instruction on pushl or popl\n");
  }
  char *reg = get_reg_val(code[1] >> 4);
  char *op = get_instruction_name(code[0]);
  sprintf(buffer, "%s %s", op, reg);
}

void write_one_ins(uint8_t *code, char *buffer) {
  char *op = get_instruction_name(code[0]);
  sprintf(buffer, "%s", op);
}

void write_jxx_or_call(uint8_t *code, char *buffer, int *label_addrs,
                       int *len) {
  int32_t value = imm_to_num(code + 1);
  char *op = get_instruction_name(code[0]);
  for (int i = 0; i < *len; i++) {
    if (label_addrs[i] == value) {
      sprintf(buffer, "%s L%d", op, i);
      return;
    }
  }
  sprintf(buffer, "%s $%d", op, value);
}

void add_label(int *label_addrs, int *len, uint8_t *code, int code_line) {
  int32_t value = imm_to_num(code + 1);
  if (value >= code_line) {
    return;
  }
  // 查找是否已经有该标签
  for (int i = 0; i < *len; i++) {
    if (label_addrs[i] == value) {
      return;
    }
  }
  label_addrs[*len] = value;
  // printf("%d\n", value);
  (*len)++;
}

void disasm(uint8_t *code, char *buffer, int len) {
  char buffer_[100];
  int i = 0;
  int *label_addrs = (int *)malloc(100 * sizeof(int));
  int *data_addrs = (int *)malloc(100 * sizeof(int));
  int label_len = 0;
  int data_len = 0;
  int label_capacity = 100;
  int data_capacity = 100;

  while (i < len) {
    uint8_t ins = code[i];
    int ins_len = get_instruction_len(ins);
    if ((ins >= 0x70 && ins <= 0x76) || ins == 0x80) {
      if (label_len >= label_capacity) {
        label_capacity *= 2;
        label_addrs = (int *)realloc(label_addrs, label_capacity * sizeof(int));
      }
      add_label(label_addrs, &label_len, code + i, len);
    } else if (ins == 0x30) {
      if ((strcmp(get_instruction_name(code[imm_to_num(code + i + 2)]),
                  "Invalid") == 0 &&
           code[imm_to_num(code + i + 2) - 1] == 0x00) &&
          imm_to_num(code + i + 2) < len) {
        if (label_len >= label_capacity) {
          label_capacity *= 2;
          label_addrs =
              (int *)realloc(label_addrs, label_capacity * sizeof(int));
        }
        add_label(label_addrs, &label_len, code + i + 1, len);
        if (data_len >= data_capacity) {
          data_capacity *= 2;
          data_addrs = (int *)realloc(data_addrs, data_capacity * sizeof(int));
        }
        data_addrs[data_len] = imm_to_num(code + i + 2);
        data_len++;
      }
    }
    i += ins_len;
  }

  i = 0;
  while (i < len) {
    uint8_t ins = code[i];
    int ins_len = get_instruction_len(ins);

    for (int j = 0; j < label_len; j++) {
      if (i == label_addrs[j]) {
        sprintf(buffer_, "L%d:\n", j);
        strcat(buffer, buffer_);
        break;
      }
    }

    for (int j = 0; j < data_len; j++) {
      if (i == data_addrs[j]) {
        int data_addr = data_addrs[j];
        uint8_t ins = code[data_addr];
        strcat(buffer, "\t.align 4\n");
        while ((strcmp(get_instruction_name(ins), "Invalid") == 0 ||
                ins == 0x00) &&
               data_addr < len) {
          sprintf(buffer_, "\t.long 0x%x\n", imm_to_num(code + data_addr));
          strcat(buffer, buffer_);
          data_addr += 4;
        }
        i = data_addr;
        if (i >= len) {
          free(label_addrs);
          free(data_addrs);
          return;
        }
        break;
      }
    }
    if (strcmp(get_instruction_name(ins), "Invalid") == 0) {
      printf("Invalid instruction 0x%x\n", i);
      sprintf(buffer_, "Invalid");
      strcat(buffer, buffer_);
      free(label_addrs);
      free(data_addrs);
      return;
    }

    if (ins == 0x30)
      write_irmovl(code + i, buffer_, label_addrs, &label_len); // 111
    else if (ins == 0x20)
      write_rrmovl(code + i, buffer_);
    else if (ins == 0x40)
      write_rmmovl(code + i, buffer_);
    else if (ins == 0x50)
      write_mrmovl(code + i, buffer_);
    else if (ins >= 0x60 && ins <= 0x63)
      write_opl_or_cmovxx(code + i, buffer_);
    else if (ins >= 0x70 && ins <= 0x76 || ins == 0x80)
      write_jxx_or_call(code + i, buffer_, label_addrs, &label_len);
    else if (ins == 0xA0 || ins == 0xB0)
      write_pp(code + i, buffer_);
    else if (ins == 0x00) {
      _Bool flag = 1;
      for (int j = 0; j < data_len; j++) {
        if (i + 1 == data_addrs[j]) {
          memset(buffer_, 0, sizeof(buffer_));
          flag = 0;
          break;
        }
      }
      if(flag)
        write_one_ins(code + i, buffer_);
    }
    else
      write_one_ins(code + i, buffer_);

    strcat(buffer, "\t");
    strcat(buffer, buffer_);
    strcat(buffer, "\n");
    i += ins_len;
  }

  free(label_addrs);
  free(data_addrs);
}