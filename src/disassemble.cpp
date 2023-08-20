#include "disassemble.hpp"

Disassembler::Instruction Disassembler::Get(uint16_t index, const cpu_t* cpu) {
  uint8_t byte = cpu->memory[index];
  switch (byte) {
  }
  return Disassembler::Instruction();
}
