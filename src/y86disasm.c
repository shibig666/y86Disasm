#include "y86disasm.h"
#include "y86registor.h"
#include <stdint.h>
#include <stdio.h>
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

void add_label(label labels[], int *label_len, int32_t value, type_t type) {
  // 查找是否已经有该标签
  // printf("add label 0x%x\n", value);
  for (int i = 0; i < *label_len; i++) {
    if (labels[i].addr == (uint32_t)value) {
      return;
    }
  }
  printf("add label 0x%x\n", value);
  labels[*label_len].addr = (uint32_t)value;
  labels[*label_len].l = *label_len;
  labels[*label_len].type = type;
  labels[*label_len].is_label = 1;
  (*label_len)++;
}

void get_instructions(uint8_t *code, uint32_t len, instruction *ins,
                      number_data *data, int *ins_len, int *data_len,
                      label *labels, int *label_len) {
  int i = 0;
  *ins_len = 0;
  *data_len = 0;
  _Bool is_number = 0;
  while (i < len) {

    // 判断是否进入数据区域
    if (is_number == 0) {
      for (int k = 0; k < *ins_len; k++) {
        if (ins[k].ins == 0x30 && ins[k].value == i + 1 && code[i] == 0x00) {
          is_number = 1;
          i++;
          add_label(labels, label_len, i, DATA);
          break;
        }
      }
    }
    if (is_number) {
      data[*data_len].addr = i;
      data[*data_len].len = 4;
      data[*data_len].value = imm_to_num(code + i);
      (*data_len)++;
      i += 4;
      // if (i <= len && strcmp(get_instruction_name(code[i]), "Invalid") != 0 )
      // {
      //   is_number = 0;
      //   printf("%x\n",i);
      // }

      continue;
    }

    if (strcmp(get_instruction_name(code[i]), "Invalid") == 0) {
      printf("Invalid instruction 0x%x\n", i);
      i++;
      return;
    }

    ins[*ins_len].addr = i;
    ins[*ins_len].ins = code[i];
    ins[*ins_len].len = get_instruction_len(code[i]);
    ins[*ins_len].reg1 = 0;
    ins[*ins_len].reg2 = 0;
    ins[*ins_len].value = 0;

    if (ins[*ins_len].ins == 0x00 || ins[*ins_len].ins == 0x10 ||
        ins[*ins_len].ins == 0x90) {
      i += ins[*ins_len].len;
      (*ins_len)++;
      continue;
    } else if (ins[*ins_len].ins == 0x20) {
      ins[*ins_len].reg1 = code[i + 1] >> 4;
      ins[*ins_len].reg2 = code[i + 1] & 0xF;
    } else if (ins[*ins_len].ins == 0x30) {
      if (code[i + 1] / 16 != 0xF) {
        printf("Invalid instruction on irmovl\n");
      }
      ins[*ins_len].reg1 = code[i + 1] & 0xF;
      ins[*ins_len].value = imm_to_num(code + i + 2);
    } else if (ins[*ins_len].ins == 0x40) {
      ins[*ins_len].reg1 = code[i + 1] >> 4;
      ins[*ins_len].reg2 = code[i + 1] & 0xF;
      ins[*ins_len].value = imm_to_num(code + i + 2);
    } else if (ins[*ins_len].ins == 0x50) {
      ins[*ins_len].reg1 = code[i + 1] >> 4;
      ins[*ins_len].reg2 = code[i + 1] & 0xF;
      ins[*ins_len].value = imm_to_num(code + i + 2);
    } else if (ins[*ins_len].ins >= 0x60 && ins[*ins_len].ins <= 0x63) {
      ins[*ins_len].reg1 = code[i + 1] >> 4;
      ins[*ins_len].reg2 = code[i + 1] & 0xF;
    } else if (ins[*ins_len].ins >= 0x70 && ins[*ins_len].ins <= 0x76 ||
               ins[*ins_len].ins == 0x80) {
      ins[*ins_len].value = imm_to_num(code + i + 1);
      add_label(labels, label_len, ins[*ins_len].value, INS);
    } else if (ins[*ins_len].ins == 0xA0 || ins[*ins_len].ins == 0xB0) {
      if (code[i + 1] % 16 != 0xF) {
        printf("Invalid instruction on push or popl\n");
      }
      ins[*ins_len].reg1 = code[i + 1] >> 4;
    }
    i += ins[*ins_len].len;
    (*ins_len)++;
  }
}

void disasm(uint8_t *code, char *buffer, uint32_t len) {
  char buffer_[100];
  instruction ins[100];
  number_data data[100];
  label labels[100];
  int ins_len = 0;
  int data_len = 0;
  int label_len = 0;
  get_instructions(code, len, ins, data, &ins_len, &data_len, labels,
                   &label_len);
  for (int i = 0; i < ins_len; i++) {

    // 判断是否为标签
    for (int j = 0; j < label_len; j++) {
      if (ins[i].addr == labels[j].addr) {
        sprintf(buffer_, "L%d:\n", labels[j].l);
        strcat(buffer, buffer_);
        break;
      }
    }

    if (ins[i].ins == 0x30) {
      // 判断是否为数据地址
      _Bool is_data = 0;
      for (int j = 0; j < data_len; j++) {
        if ((uint32_t)ins[i].value == data[j].addr) {
          is_data = 1;
          break;
        }
      }
      if (is_data) {
        for (int j = 0; j < label_len; j++) {
          if (ins[i].value == labels[j].addr) {
            sprintf(buffer_, "\tirmovl $L%d,%s", labels[j].l,
                    get_reg_val(ins[i].reg1));
            break;
          }
        }
      } else {
        sprintf(buffer_, "\tirmovl $%d,%s", ins[i].value,
                get_reg_val(ins[i].reg1));
      }
    } else if (ins[i].ins >= 0x70 && ins[i].ins <= 0x76 || ins[i].ins == 0x80) {
      _Bool is_label = 0;
      for (int j = 0; j < ins_len; j++) {
        if ((uint32_t)ins[i].value == ins[j].addr) {
          is_label = 1;
          break;
        }
      }
      if (is_label) {
        for (int j = 0; j < label_len; j++) {
          if (ins[i].value == labels[j].addr) {
            sprintf(buffer_, "\t%s L%d", get_instruction_name(ins[i].ins),
                    labels[j].l);
            break;
          }
        }
      } else {
        sprintf(buffer_, "\t%s %d", get_instruction_name(ins[i].ins),
                ins[i].value);
      }
    } else if (ins[i].ins == 0x40) {
      if (ins[i].value == 0)
        sprintf(buffer_, "\trmmovl %s,(%s)", get_reg_val(ins[i].reg1),
                get_reg_val(ins[i].reg2));
      else
        sprintf(buffer_, "\trmmovl %s,%d(%s)", get_reg_val(ins[i].reg1),
                ins[i].value, get_reg_val(ins[i].reg2));
    } else if (ins[i].ins == 0x50) {
      if (ins[i].value == 0)
        sprintf(buffer_, "\tmrmovl (%s),%s", get_reg_val(ins[i].reg2),
                get_reg_val(ins[i].reg1));
      else
        sprintf(buffer_, "\tmrmovl %d(%s),%s", ins[i].value,
                get_reg_val(ins[i].reg2), get_reg_val(ins[i].reg1));
    } else if (ins[i].ins >= 0x60 && ins[i].ins <= 0x63) {
      sprintf(buffer_, "\t%s %s,%s", get_instruction_name(ins[i].ins),
              get_reg_val(ins[i].reg1), get_reg_val(ins[i].reg2));
    } else if (ins[i].ins == 0xA0 || ins[i].ins == 0xB0) {
      sprintf(buffer_, "\t%s %s", get_instruction_name(ins[i].ins),
              get_reg_val(ins[i].reg1));
    } else if (ins[i].ins == 0x20) {
      sprintf(buffer_, "\t%s %s,%s", get_instruction_name(ins[i].ins),
              get_reg_val(ins[i].reg1), get_reg_val(ins[i].reg2));
    } else {
      sprintf(buffer_, "\t%s", get_instruction_name(ins[i].ins));
    }
    strcat(buffer, buffer_);
    strcat(buffer, "\n");
  }

  for (int i = 0; i < data_len; i++) {
    for (int j = 0; j < label_len; j++) {
      if (data[i].addr == labels[j].addr) {
        sprintf(buffer_, "L%d:\n", labels[j].l);
        strcat(buffer, buffer_);
        strcat(buffer, "\t.align 4\n");
        break;
      }
    }
    sprintf(buffer_, "\t.long 0x%x\n", data[i].value);
    strcat(buffer, buffer_);
  }
}