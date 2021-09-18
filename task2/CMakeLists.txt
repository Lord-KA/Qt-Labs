cmake_minimum_required(VERSION 3.1.0)

project(GPaint)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(./QSidePanel)

add_executable(
    GPaint
    main.cpp
    window.hpp
)

add_executable(
    Sidebars
    ./QSidePanel/
    main2.cpp
    sidebars.hpp
    sidebars.cpp
)

target_link_libraries(GPaint Qt5::Widgets)
target_link_libraries(Sidebars Qt5::Widgets)
