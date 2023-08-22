#include "TextComponent.hpp"
#include <ftxui/dom/elements.hpp>

namespace ftxui {
  Component Text(const std::string& text) {
    return Make<TextBase>(text);
  }

  TextBase::TextBase(): m_Text("") {}
  TextBase::TextBase(const std::string& str): m_Text(str) {}

  Element TextBase::Render() {
    return text(m_Text);
  }
}
