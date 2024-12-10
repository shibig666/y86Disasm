#ifndef DISASM_H
#define DISASM_H

#include <stdint.h>

char *get_instruction_name(uint8_t num);
int get_instruction_len(uint8_t num);
void write_irmovl(uint8_t *code, char *buffer, int *label_addrs, int *len);
void write_rmmovl(uint8_t *code, char *buffer);
void write_mrmovl(uint8_t *code, char *buffer);
void write_opl_or_cmovxx(uint8_t *code, char *buffer);
void write_pp(uint8_t *code, char *buffer);
void write_one_ins(uint8_t *code, char *buffer);
void write_jxx_or_call(uint8_t *code, char *buffer, int *label_addrs, int *len);
void disasm(uint8_t *code, char *buffer, int len);

#endif 