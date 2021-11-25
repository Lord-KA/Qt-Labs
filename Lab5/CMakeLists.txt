cmake_minimum_required(VERSION 3.1.0)

project(Lab_5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -D EXTRA_VERBOSE" CACHE STRING "Comment" FORCE)
set(CMAKE_CXX_FLAGS_SANITIZER "${CMAKE_CXX_FLAGS} -Wpedantic -Wall -Wextra -Wformat=2 -fsanitize=address,undefined -g" CACHE STRING "Comment" FORCE)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include(FetchContent)

FetchContent_Declare(
  frdialog
  GIT_REPOSITORY https://github.com/lord-ka/QtFindReplaceDialog.git
)

FetchContent_GetProperties(frdialog)
if(NOT frdialog_POPULATED)
  FetchContent_Populate(frdialog)
  add_subdirectory(${frdialog_SOURCE_DIR}/dialogs)
endif()


add_executable( run
    main.cpp  
    MainWindow.cpp 
    TextEditor.cpp 
    MainWindow.hpp 
    TextEditor.hpp
    SyntaxHighlighter.hpp
    SyntaxHighlighter.cpp
)

target_link_libraries(run Qt5::Widgets)
target_link_libraries(run QtFindReplaceDialog)
