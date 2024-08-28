cmake_minimum_required(VERSION 3.5)

project(SNMP_cross LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SNMP_cross main.cpp processexecutor.cpp processexecutor.h)

install(TARGETS SNMP_cross
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
