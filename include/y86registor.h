#ifndef Y86REGISTOR_H
#define Y86REGISTOR_H

#include <stdint.h>

char *get_reg_val(uint8_t reg_num);
int32_t imm_to_num(uint8_t *imm);

#endif 