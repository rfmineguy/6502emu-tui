#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/flexbox_config.hpp>
#include <6502emu/cpu.h>
#include <iostream>

int main() {
  using namespace ftxui;
  cpu_t cpu;
  
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
  auto code_window = Container::Vertical({});
  {
  auto code_window_renderer = Renderer([&] {
    return vbox({
      text("Code")
    });
  });
  code_window->Add(code_window_renderer | border | flex);
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
  int code_split_size = 290;
  Component mainLayout = cpu_state;
  mainLayout = ResizableSplitBottom(watch, mainLayout, &watch_split_size);
  mainLayout = ResizableSplitBottom(controls_window, mainLayout, &controls_split_size);
  mainLayout = ResizableSplitRight(code_window, mainLayout, &code_split_size);

  screen.Loop(mainLayout);
  return EXIT_SUCCESS;
}
