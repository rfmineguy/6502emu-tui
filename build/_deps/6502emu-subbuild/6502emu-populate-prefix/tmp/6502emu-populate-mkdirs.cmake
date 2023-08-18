# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/rileyfischer/Documents/dev/cpu_emulators/6502emu_tui/build/_deps/6502emu-src"
  "/Users/rileyfischer/Documents/dev/cpu_emulators/6502emu_tui/build/_deps/6502emu-build"
  "/Users/rileyfischer/Documents/dev/cpu_emulators/6502emu_tui/build/_deps/6502emu-subbuild/6502emu-populate-prefix"
  "/Users/rileyfischer/Documents/dev/cpu_emulators/6502emu_tui/build/_deps/6502emu-subbuild/6502emu-populate-prefix/tmp"
  "/Users/rileyfischer/Documents/dev/cpu_emulators/6502emu_tui/build/_deps/6502emu-subbuild/6502emu-populate-prefix/src/6502emu-populate-stamp"
  "/Users/rileyfischer/Documents/dev/cpu_emulators/6502emu_tui/build/_deps/6502emu-subbuild/6502emu-populate-prefix/src"
  "/Users/rileyfischer/Documents/dev/cpu_emulators/6502emu_tui/build/_deps/6502emu-subbuild/6502emu-populate-prefix/src/6502emu-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/rileyfischer/Documents/dev/cpu_emulators/6502emu_tui/build/_deps/6502emu-subbuild/6502emu-populate-prefix/src/6502emu-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/rileyfischer/Documents/dev/cpu_emulators/6502emu_tui/build/_deps/6502emu-subbuild/6502emu-populate-prefix/src/6502emu-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
