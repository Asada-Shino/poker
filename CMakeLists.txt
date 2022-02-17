cmake_minimum_required (VERSION 2.8)

set(POKER_VERSION 0.1)

project(poker)

include_directories("include")

add_executable(poker src/poker.c src/main.c)