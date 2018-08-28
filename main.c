#include <stdio.h>
#include "exa.c"

uint8_t assembleOpcode(uint8_t opcode, uint8_t flags) {
  return (flags << 5) | opcode;
}

int main() {
  EXA exa;
  EXA_init(&exa, "XA");

  /*
  exa.regX = TEXT_VAL("WORD");
  exa.regT = NUMBER_VAL(42);
  */

  printf("%s\n", exa.name);
  printf("%d\n", AS_NUMBER(exa.registers[REG_X]));

  exa.instructions[0] = assembleOpcode(EXA_COPY, 1);
  exa.instructions[1] = 42;
  exa.instructions[2] = 0;
  exa.instructions[3] = REG_X;

  exa.instructions[4] = assembleOpcode(EXA_JUMP, 0);
  int16_t v = 7;
  printf("%x\n", (int16_t)v);
  exa.instructions[5] = (uint8_t)(v & 0xFF);
  exa.instructions[6] = (uint8_t)((v >> 8) & 0xFF);

  exa.instructions[7] = assembleOpcode(EXA_COPY, 0);
  exa.instructions[8] = REG_X;
  exa.instructions[9] = REG_T;

  exa.instructions[10] = assembleOpcode(EXA_NOOP, 1);


  printf("0x%x\n", exa.instructions[3]);
  printf("0x%x\n", exa.instructions[4]);
  printf("0x%x\n", exa.instructions[5]);


  bool alive = true;
  while (alive) {
    alive = EXA_tick(&exa);
  }


  printf("%s\n", exa.name);
  printf("X: %d\n", AS_NUMBER(exa.registers[REG_X]));
  printf("T: %d\n", AS_NUMBER(exa.registers[REG_T]));
  printf("M: %d\n", AS_NUMBER(exa.registers[REG_M]));
  printf("F: %d\n", AS_NUMBER(exa.registers[REG_F]));


  return 0;
}

