cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)

# 1行ずつ明示的に分けることで、CMakeに確実に認識させます
target_sources(app PRIVATE src/main.cpp)
target_sources(app PRIVATE src/sato_lcd.cpp)
target_compile_options(app PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-finput-charset=UTF-8 -fexec-charset=UTF-8>)