cmake_minimum_required(VERSION 3.16)

project("KolkoKrzyzyk" VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Qt ##
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


## Qt PACKAGES ##
find_package(Qt6 REQUIRED COMPONENTS Core)

## Qt project setup ##
qt_standard_project_setup()

## Qt ADD EXE ##
qt_add_executable("${PROJECT_NAME}"
    "src/main.cpp"
    "include/main.h"
)

target_link_libraries("${PROJECT_NAME}" PRIVATE Qt6::Core)