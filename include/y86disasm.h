#ifndef DISASM_H
#define DISASM_H

#include <stdint.h>

char *get_instruction_name(uint8_t num);
int get_instruction_len(uint8_t num);
void disasm(uint8_t *code, char *buffer, uint32_t len);

typedef enum { INS, DATA } type_t;
typedef struct {
  uint32_t addr;
  uint8_t ins;
  int len;
  uint8_t reg1;
  uint8_t reg2;
  int32_t value;
} instruction;

typedef struct {
  uint32_t addr;
  int len;
  int32_t value;
} number_data;

typedef struct {
  uint32_t addr;
  int l;
  type_t type;
  _Bool is_label;
} label;

#endif 