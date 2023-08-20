cmake_minimum_required(VERSION 3.20)
project(6502emu_tui LANGUAGES C CXX)
project(6502emu_tui_testing LANGUAGES C CXX)

include (FetchContent)
FetchContent_Declare(ftxui GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git GIT_TAG v4.1.1)
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(6502lib GIT_REPOSITORY https://github.com/rfmineguy/6502emu.git)
FetchContent_MakeAvailable(6502lib)

add_executable(6502emu_tui src/main.cpp)
target_include_directories(6502emu_tui PRIVATE src ${6502lib_SOURCE_DIR}/include)

target_link_libraries(6502emu_tui
  PRIVATE 6502lib
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

install(TARGETS 6502emu_tui DESTINATION /usr/local/bin)

add_executable(6502_emu_tui_testing src/test_main.cpp)
target_include_directories(6502_emu_tui_testing PRIVATE src ${6502lib_SOURCE_DIR}/include)

target_link_libraries(6502_emu_tui_testing
  PRIVATE 6502lib
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
