cmake_minimum_required(VERSION 3.19)
project(QtGSI LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network WebSockets)

qt_standard_project_setup()

add_subdirectory(src)
