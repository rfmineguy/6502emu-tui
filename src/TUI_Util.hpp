#ifndef TUI_UTIL_HPP
#define TUI_UTIL_HPP
#include <unordered_map>
#include <6502emu/dbginfo.h>
#include <6502emu/cpu.h> 
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/flexbox_config.hpp>

namespace TUIUtil {
  ftxui::Component CPUState(const cpu_t*);
  ftxui::Component Watch(const cpu_t*, ftxui::Component&);
  ftxui::Component CodeScroll(cpu_t*, cc65_dbginfo, std::unordered_map<uint16_t, instruction_t>&);
  ftxui::Component HexMemoryView();
}

#endif
