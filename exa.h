#include <stdbool.h>
#include <stdint.h>
#include "value.h"

#ifndef EXA_VM_H
#define EXA_VM_H

typedef enum {
  EXA_HALT,
  EXA_NOOP,
  EXA_COPY
} InstructionSet;

typedef enum {
  REG_X,
  REG_T,
  REG_M,
  REG_F
} Registers;

/*
 * Numbers are 2-byte (15-bit)
 * Text... byte streams
 * Registers: 1 byte values
 * Hardware Register, prefixed with # byte stream
 *
 * Type 1:
 * | Opcode | ArgType Flags |
 * | 5 bits | 3 bits        |
 *
 * | Opcode |    R   |
 * | 8 bits | 3 bits |

 * | Opcode |   R/N  |
 * | 8 bits |  |
 *
 * | Opcode |     N   |
 * | 8 bits | 16 bits |

 * | Opcode |  R\N   |   R   |
 * | 8 bits | 16 bit | byte |
 *
 * | Opcode |  R\N  |  R\N  |
 * | 8 bits |

 */

typedef struct {
  size_t length;
  uint8_t* data;
} EXA_FILE;

typedef struct {
  // Human-readable name for EXA.
  char name[9];
  // Instruction pointer (assuming a total of 255 instrunctions (final is always HALT)
  uint8_t* ip;
  Value registers[4];
  // X: General purpose register
  // T: General purpose register, stores result of TEST and criterion of TJMP/FJMP
  // M: Message-passing register, has additional semantics
  // F: Virtual register exists for file IO, Not usable for storage.

  uint8_t instructions[256];
  // If not NULL, points to the file "held" by the EXA.
  EXA_FILE file;
} EXA;

void EXA_init(EXA* exa, char* name);
bool EXA_tick(EXA* exa);
uint8_t EXA_fetch(EXA* exa);

#endif // EXA_VM_H
