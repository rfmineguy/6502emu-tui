#ifndef SCROLL_PANE_HPP
#define SCROLL_PANE_HPP
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <array>
#include <unordered_map>
#include "TextComponent.hpp"
#include <6502emu/cpu.h>
#include <6502emu/dbginfo.h>

namespace ftxui {
  Component ScrollPane(const std::unordered_map<uint16_t, instruction_t>&, cc65_dbginfo);

class ScrollPaneBase : public ComponentBase {
  public:
    ScrollPaneBase(const std::unordered_map<uint16_t, instruction_t>& lines, cc65_dbginfo);

    Element Render() override;
    bool OnEvent(Event event) override;

  private:
    int scroll_delta_selection_index;
    int m_scroll_addr, m_scroll_delta;
    std::unordered_map<uint16_t, instruction_t> m_lines;
    std::array<Element, 0xff> m_displayed_lines;
    std::array<instruction_t, 0xff> m_displayed_instructions;

  private:
    cc65_dbginfo m_dbg;
    bool m_mouse_hover;
    int m_last_click_x, m_last_click_y;
    Box m_box;
};
}

#endif
