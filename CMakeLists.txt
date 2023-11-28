cmake_minimum_required(VERSION 3.5)

project(
    fake-railway-sys
    VERSION 0.1.0
    DESCRIPTION "A fake railway management system written with Qt6 for database course experiment."
    HOMEPAGE_URL "https://github.com/zymelaii/fake-railway-sys"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt NAMES Qt6)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

add_subdirectory(src)
