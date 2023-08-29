#include "ScrollPane.hpp"
#include <iostream>
#include <algorithm>
#include <fmt/core.h>
#include <ftxui/component/component.hpp>
#include <6502emu/cpu.h>

namespace ftxui {
  Component ScrollPane(const std::unordered_map<uint16_t, instruction_t>& lines, cc65_dbginfo dbg) {
    return Make<ScrollPaneBase>(lines, dbg);
  }
  
  ScrollPaneBase::ScrollPaneBase(const std::unordered_map<uint16_t, instruction_t>& lines, cc65_dbginfo dbg): m_lines(lines), m_scroll_delta(1), m_scroll_addr(0x8000), m_dbg(dbg) {
    // fill up display buffer at the start
    for (int i = 0; i < 0xff; i++) {
      m_displayed_lines.at(i) = text({"..."});
    }
  }
  
  Element ScrollPaneBase::Render() {
    const bool active = Active();
    const bool focused = Focused();
    const bool focused_or_hover = focused || m_mouse_hover;
  
    auto focus_management = focused ? focus : active ? select : nothing;

    auto lines_renderer = Renderer([&] {
      const cc65_segmentinfo* segment_info = cc65_get_segmentlist(m_dbg);
      // for (int i = 0; i < segment_info->count; i++) {
      //   cc65_segmentdata seg = segment_info->data[i];
      //   cc65_addr start = seg.segment_start;
      //   cc65_size size = seg.segment_size;
      // }

      /* Ouput  (Example)
       * =====
       * ...    ...  ZEROPAGE
       * 450    lda #43
       * 452    sta $2095
       * 454    jmp $1000
       * ...    ...  CODE
       * 1000   cmp #23
       * 1002   bne $0450
       * ...    ...
       */
      int line = 0;
      int line_height = m_box.y_max - m_box.y_min;
      for (int i = 0; i < segment_info->count; i++) {
        cc65_segmentdata seg = segment_info->data[i];
        if (line > m_box.y_max - m_box.y_min)                       break; // if the current line is passed the end of the frame, ignore it
        if (m_scroll_addr + 0x20 < seg.segment_start)             break; // if none of the segment memory is in range
        if (m_scroll_addr > seg.segment_start + seg.segment_size) break; // if we've scrolled completely passed it, ignore it
    
        int start_addr = m_scroll_addr;
        int end_addr   = m_scroll_addr + 0x20;
        for (int j = start_addr; j < end_addr; j++) {
          try {
            instruction_t ins = m_lines.at(j);
            m_displayed_lines.at(line) = text(fmt::format(" {:x}  {}", ins.address, ins.str));
            j += ins.bytes;
            line++;
          } catch (std::out_of_range e) {
            // m_displayed_lines.at(line) = text(" INVALID INSTRUCTION ..........");
            // line++;
          }
        }
      }

      return vbox(std::vector<Element>(m_displayed_lines.begin(), m_displayed_lines.end()));
    });

    auto element = vbox({
      hbox({
        vbox({
        text("ScrollAddr     : " + fmt::format("{:x}", m_scroll_addr)),
        text("Hover          : " + std::to_string(m_mouse_hover)),
        text("Width          : " + std::to_string(m_box.x_max - m_box.x_min)),
        }),
        vbox({
        text("Height         : " + std::to_string(m_box.y_max - m_box.y_min)),
        text("m_last_click_x : " + fmt::format("{:x}", m_last_click_x)),
        text("m_last_click_y : " + fmt::format("{:x}", m_last_click_y)),
        })
      }) | flex_grow,
      separator(),
      lines_renderer->Render()
    }) | focus_management;

    return element | flex | reflect(m_box);
  }
  
  bool ScrollPaneBase::OnEvent(Event event) {
    if (event.is_mouse()) {
      m_mouse_hover = m_box.Contain(event.mouse().x, event.mouse().y) && CaptureMouse(event);
      if (!m_mouse_hover) {
        return false;
      }
      if (event.mouse().button == Mouse::WheelUp) {
        m_scroll_addr -= m_scroll_delta;
        if (m_scroll_addr < 0) {
          m_scroll_addr = 0;
        }
        return true;
      }
      if (event.mouse().button == Mouse::WheelDown) {
        m_scroll_addr += m_scroll_delta;
        if (m_scroll_addr > 0xffff - (m_box.y_max - m_box.y_min)) {
          m_scroll_addr = 0xffff - (m_box.y_max - m_box.y_min);
        }
        return true;
      }
      if (event.mouse().button == Mouse::Left) {
        m_last_click_x = event.mouse().x;
        m_last_click_y = event.mouse().y;
      }
    }
    return false;
  }
}
