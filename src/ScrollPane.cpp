#include "ScrollPane.hpp"
#include <iostream>
#include <algorithm>
#include <fmt/core.h>
#include <ftxui/component/component.hpp>
#include <6502emu/cpu.h>

namespace ftxui {
  Component ScrollPane(const std::vector<instruction_t>& lines) {
    return Make<ScrollPaneBase>(lines);
  }
  
  ScrollPaneBase::ScrollPaneBase(const std::vector<instruction_t>& lines): m_lines(lines), m_scroll_delta(30), m_scroll_line(0) {}
  
  Element ScrollPaneBase::Render() {
    const bool active = Active();
    const bool focused = Focused();
    const bool focused_or_hover = focused || mouse_hover_;
  
    auto focus_management = focused ? focus : active ? select : nothing;

    const EntryState state = {
        "",
        false,
        active,
        focused_or_hover,
    };

    auto lines_renderer = Renderer([&] {
      instruction_t prev = {0};
      int height = box_.y_max - box_.y_min - 2;
      for (int i = 0; i < m_displayed_lines.size(); i++) {
        std::string line = "";
        try {
          instruction_t ins = m_lines.at(i + m_scroll_line);
          m_displayed_lines.at(i) = text({ fmt::format(" • {:x}", ins.address) + "  " +  ins.str });
        } catch (std::out_of_range const& exc) {
          m_displayed_lines.at(i) = text({ "    " +  std::string(exc.what()) });
        }
      }
      return vbox(std::vector<Element>(m_displayed_lines.begin(), m_displayed_lines.end()));
    });

    static std::vector<std::string> entries = { "0x01", "0xff" };
    auto selector = Radiobox(ConstStringListRef(&entries), &scroll_delta_selection_index, {});
    auto element = vbox({
      hbox({
        vbox({
          text("ScrollPane   : " + fmt::format("{:x}", m_scroll_line)),
          text("Hover        : " + std::to_string(mouse_hover_)),
          text("Height       : " + std::to_string(box_.y_max - box_.y_min)),
          text("width        : " + std::to_string(box_.x_max - box_.x_min)),
          text("lines        : " + std::to_string(m_lines.size())),
          text("scroll_delta : " + fmt::format("{:x}", m_scroll_delta)),
        }),
        // selector->Render(),
      }),
      separator(),
      lines_renderer->Render()
    }) | focus_management;

    return element | flex | reflect(box_);
  }
  
  bool ScrollPaneBase::OnEvent(Event event) {
    if (event.is_mouse()) {
      mouse_hover_ = box_.Contain(event.mouse().x, event.mouse().y) && CaptureMouse(event);
      if (!mouse_hover_) {
        return false;
      }
      if (event.mouse().button == Mouse::WheelUp) {
        m_scroll_line -= m_scroll_delta;
        if (m_scroll_line < 0) {
          m_scroll_line = 0;
        }
        return true;
      }
      if (event.mouse().button == Mouse::WheelDown) {
        m_scroll_line += m_scroll_delta;
        if (m_scroll_line > m_lines.size() - (box_.y_max - box_.y_min)) {
          m_scroll_line = m_lines.size() - (box_.y_max - box_.y_min);
        }
        return true;
      }
    }
    return false;
  }
}
