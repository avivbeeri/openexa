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
  exa.instructions[2] = REG_X;

  bool alive = true;
  while (alive) {
    alive = EXA_tick(&exa);
  }


  printf("%s\n", exa.name);
  printf("%d\n", AS_NUMBER(exa.registers[REG_X]));


  return 0;
}

