CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(y86disassembler)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
add_library(y86libs src/y86registor.c src/y86disasm.c)
add_executable(y86disassembler src/main.c)
target_link_libraries(y86disassembler y86libs)
