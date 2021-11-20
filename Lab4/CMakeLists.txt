cmake_minimum_required(VERSION 3.1.0)

project(Lab_4)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g" CACHE STRING "Comment" FORCE)
set(CMAKE_CXX_FLAGS_SANITIZER "${CMAKE_CXX_FLAGS} -Wpedantic -Wall -Wextra -Wformat=2 -fsanitize=address,undefined -g" CACHE STRING "Comment" FORCE)


find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(lab4
    EditDialog.cpp Figure.hpp lab4.hpp Slider.hpp
    EditDialog.hpp Figure.cpp lab4.cpp main.cpp Slider.cpp
)

target_link_libraries(lab4 Qt5::Widgets)
