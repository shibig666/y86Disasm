#include "y86registor.h"

char* get_reg_val(uint8_t reg_num) {
  switch (reg_num) {
    case 0:
      return "\%eax";
    case 1:
      return "\%ecx";
    case 2:
      return "\%edx";
    case 3:
      return "\%ebx";
    case 4:
      return "\%esp";
    case 5:
      return "\%ebp";
    case 6:
      return "\%esi";
    case 7:
      return "\%edi";
    case 0xF:
      return "null";
    default:
      return "Invalid";
  }
}

int32_t imm_to_num(uint8_t *imm) {
  int32_t value = (imm[0] & 0xFF) | ((imm[1] & 0xFF) << 8) |
                  ((imm[2] & 0xFF) << 16) | ((imm[3] & 0xFF) << 24);
  return value;
}

