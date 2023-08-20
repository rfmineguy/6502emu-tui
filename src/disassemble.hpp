#ifndef DISASSEMBLE_HPP
#define DISASSEMBLE_HPP
#include <6502emu/cpu.h>
#include <string>

class Disassembler {
  public:
    struct Instruction {
      int byte_size;
      std::string str_rep;
    };

    static Instruction Get(uint16_t, const cpu_t*);
};

#endif
