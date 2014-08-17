cmake_minimum_required(VERSION 2.6)
project(lsca)

add_executable(lsca main.cpp)

install(TARGETS lsca RUNTIME DESTINATION bin)
