#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/flexbox_config.hpp>
#include <6502emu/cpu.h>
#include <6502emu/args.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>

int main(int argc, char** argv) {
  using namespace ftxui;
  cpu_t cpu;

  args_t args = args_parse(argc, argv);
  if (!args.input_file) {
    std::cerr << "Failed to open file " << std::endl;
    return 0;
  }
  if (!cpu_load_program(args.input_file, &cpu)) {
    std::cerr << "Failed to load program" << std::endl;
    return 0;
  }
  
  // ========================================================
  //  Screen that renders the tui interface
  // ========================================================
  auto screen = ScreenInteractive::Fullscreen();

  // ========================================================
  //  Pane that displays the current status of the 6502 registers
  // ========================================================
  auto cpu_state = Container::Vertical({});
  {
  auto cpu_state_renderer = Renderer([&] {
    return vbox({
      text("Registers"),
      text(" A: " + std::to_string((int)cpu.regA)),
      text(" X: " + std::to_string((int)cpu.regX)),
      text(" Y: " + std::to_string((int)cpu.regY)),
      separator(),
      text("Status Flags"),
      text(" N: " + std::to_string((cpu.status_flags & CPU_STATUS_FLAGS_NEGATIVE) >> 7)),
      text(" O: " + std::to_string((cpu.status_flags & CPU_STATUS_FLAGS_OVERFLOW) >> 6)),
      text(" B: " + std::to_string((cpu.status_flags & CPU_STATUS_FLAGS_BREAK) >> 4)),
      text(" D: " + std::to_string((cpu.status_flags & CPU_STATUS_FLAGS_DECIMAL) >> 3)),
      text(" I: " + std::to_string((cpu.status_flags & CPU_STATUS_FLAGS_INTERRUPT_DISABLE) >> 2)),
      text(" Z: " + std::to_string((cpu.status_flags & CPU_STATUS_FLAGS_ZERO) >> 1)),
      text(" C: " + std::to_string((cpu.status_flags & CPU_STATUS_FLAGS_CARRY) >> 0)),
      separator(),
      text(" SP:" + std::to_string((int)cpu.sp)),
      text(" PC:" + std::to_string((int)cpu.pc)),
    });
  });
  cpu_state->Add(cpu_state_renderer | border | flex);
  }

  // ========================================================
  //  Pane that displays the currently watched memory addresses
  // ========================================================
  std::string watch_input_str;
  Component watch_input = Input(&watch_input_str, "");
  auto watch = Container::Vertical({});
  {
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
  }

  // ========================================================
  //  Pane that displays the currently loaded code
  // ========================================================
  constexpr int MEM_SIZE = 0xffff;
  std::vector<Element> disassembly;
  std::vector<Element> addresses;
  auto sc_disassembly = Container::Vertical({});
  auto style = ButtonOption::Animated(Color::Default, Color::GrayDark,
                                      Color::Default, Color::White);

  char prev_buffer[255], curr_buffer[255];
  int byte_size;
  for (int i = 0; i < MEM_SIZE;) {
    std::stringstream instruction, address;
    int is_valid = cpu_get_str_rep(i, &cpu, curr_buffer, 255, &byte_size);
    // instruction << std::setbase(16) << std::showbase << i << "    " << curr_buffer;
    instruction << curr_buffer;
    address     << " " << std::setbase(16) << std::showbase << i;
    disassembly.push_back(text({instruction.str()}));
    addresses.push_back(text({address.str()}));

    if (strcmp(curr_buffer, prev_buffer) != 0) {
      sc_disassembly->Add(Button(instruction.str(), [&] {}, style));
      sc_addresses->Add();
    }
    // breakpoints.push_back(text(
    i += byte_size;
    strcpy(prev_buffer, curr_buffer);
  }

  std::cout << disassembly.size() << std::endl;
  auto code_window = Container::Vertical({});
  {
  // for (int i = 0; i < disassembly.size(); i++) {
  //   sc_disassembly->Add(disassembly.at(i));
  // }

  auto code_window_renderer = Renderer(sc_disassembly, [&]() {
    auto element = vbox({
      text("Code"),
      separator(),
      hbox({
        vbox({
          sc_disassembly->Render() | xflex | vscroll_indicator | frame,
        })
      }) | yframe | focus,
    });
    return element;
  });
  code_window->Add(code_window_renderer | border | flex);
  }

  // ========================================================
  //  Displays a memory viewer/editor
  // ========================================================
  std::vector<Element> hex_lines;
  std::stringstream line;
  for (int i = 0; i < MEM_SIZE; i++) {
    line << std::setbase(16) << std::showbase << cpu.memory[i] << " ";
    if (i % 0xf == 0) {
      hex_lines.push_back(text({ line.str() }));
      line.clear();
    }
  }

  std::cout << hex_lines.size() << std::endl;
  auto hex_window = Container::Vertical({});
  {
  auto hex_window_renderer = Renderer([&] {
    auto element = vbox(hex_lines) | border | flex | yframe;
    return element;
  });
  }

  // ========================================================
  //  Displays the main controls for the emulator, such as continue/step
  // ========================================================
  auto step_callback = [&] {};
  auto controls_window = Container::Horizontal({});
  controls_window->Add(Button("Step", step_callback, ButtonOption::Simple()) | border);
  
  // ========================================================
  //  Combines all the above panes into one master layout
  // ========================================================
  int watch_split_size = 20;
  int controls_split_size = 5;
  int code_split_size = 190;
  int hex_split_size = 30;
  Component mainLayout = cpu_state;
  mainLayout = ResizableSplitBottom(watch, mainLayout, &watch_split_size);
  mainLayout = ResizableSplitBottom(controls_window, mainLayout, &controls_split_size);
  mainLayout = ResizableSplitRight(code_window, mainLayout, &code_split_size);
  mainLayout = ResizableSplitRight(hex_window , mainLayout, &hex_split_size);

  screen.Loop(mainLayout);
  return EXIT_SUCCESS;
}
