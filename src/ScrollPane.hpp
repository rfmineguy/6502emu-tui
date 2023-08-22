#ifndef SCROLL_PANE_HPP
#define SCROLL_PANE_HPP
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <array>
#include "TextComponent.hpp"
#include <6502emu/cpu.h>

namespace ftxui {
  Component ScrollPane(const std::vector<instruction_t>&);

class ScrollPaneBase : public ComponentBase {
  public:
    ScrollPaneBase(const std::vector<instruction_t>& lines);

    Element Render() override;
    bool OnEvent(Event event) override;

  private:
    int scroll_delta_selection_index;
    int m_scroll_line, m_scroll_delta;
    std::vector<instruction_t> m_lines;
    std::array<Element, 0xff> m_displayed_lines;

  private:
    bool mouse_hover_;
    Box box_;
};
}

#endif
