#ifndef TEXT_COMPONENT_HPP
#define TEXT_COMPONENT_HPP

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component.hpp>
#include <string>

namespace ftxui {
  Component Text(const std::string& m_Text);

  class TextBase : public ComponentBase {
    public:
      TextBase();
      TextBase(const std::string&);

      Element Render() override;

    private:
      std::string m_Text;
  };
}

#endif
