#include "TUI_Util.hpp"
#include "ScrollPane.hpp"
#include <iostream>

namespace TUIUtil {
  using namespace ftxui;
  ftxui::Component CPUState(const cpu_t* cpu) {
    // ========================================================
    //  Pane that displays the current status of the 6502 registers
    // ========================================================
    auto cpu_state = Container::Vertical({});
    auto cpu_state_renderer = Renderer([&] {
      return vbox({
        text("Registers"),
        text(" A: " + std::to_string((int)cpu->regA)),
        text(" X: " + std::to_string((int)cpu->regX)),
        text(" Y: " + std::to_string((int)cpu->regY)),
        separator(),
        text("Status Flags"),
        text(" N: " + std::to_string((cpu->status_flags & CPU_STATUS_FLAGS_NEGATIVE) >> 7)),
        text(" O: " + std::to_string((cpu->status_flags & CPU_STATUS_FLAGS_OVERFLOW) >> 6)),
        text(" B: " + std::to_string((cpu->status_flags & CPU_STATUS_FLAGS_BREAK) >> 4)),
        text(" D: " + std::to_string((cpu->status_flags & CPU_STATUS_FLAGS_DECIMAL) >> 3)),
        text(" I: " + std::to_string((cpu->status_flags & CPU_STATUS_FLAGS_INTERRUPT_DISABLE) >> 2)),
        text(" Z: " + std::to_string((cpu->status_flags & CPU_STATUS_FLAGS_ZERO) >> 1)),
        text(" C: " + std::to_string((cpu->status_flags & CPU_STATUS_FLAGS_CARRY) >> 0)),
        separator(),
        text(" SP:" + std::to_string((int)cpu->sp)),
        text(" PC:" + std::to_string((int)cpu->pc)),
      });
    });
    cpu_state->Add(cpu_state_renderer | border | flex);

    return cpu_state;
  }

  Component Watch(const cpu_t* cpu, Component& watch_input) {
    auto watch = Container::Vertical({});
    auto watch_renderer = Renderer(
      Container::Vertical({ watch_input }),
      [&] {
        return vbox({
          text("Watch"),
          hbox({
            watch_input->Render() | border
          }),
        });
      }
    );
    watch->Add(watch_renderer | border | flex);
    return watch;
  }

  Component CodeScroll(cpu_t* cpu, cc65_dbginfo dbg, std::unordered_map<uint16_t, instruction_t>& disassembly) {
    constexpr int MEM_SIZE = 0xffff;
    // std::unordered_map<uint16_t, instruction_t> disassembly;
    // ========================================================
    // Disassembly section
    // ========================================================
    {

    int byte_size;
    for (int i = 0; i < MEM_SIZE;) {
      instruction_t ins = cpu_get_instruction(i, cpu);
      disassembly.emplace(ins.address, ins);

      if (ins.bytes == 0) {
        i ++;
      }
      else {
        i+=ins.bytes;
      }
    }
    std::cout << "Finished disassembly" << std::endl;
    }

    auto scrollpane = ScrollPane(disassembly, dbg);

    // std::cout << disassembly.size() << std::endl;
    auto code_window = Container::Vertical({});
    auto code_window_renderer = Renderer(scrollpane, [&]() {
      auto element = vbox({
        text("Code"),
        separator(),
        scrollpane->Render() | xflex,
      });
      return element;
    });
    code_window->Add(code_window_renderer | border | flex);
    return code_window;
  }
  
  Component HexMemoryView() {
  
  }
}
