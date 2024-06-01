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
find_package(Qt6 REQUIRED COMPONENTS Network)

## other PACKAGES ##
find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)

## Qt project setup ##
qt_standard_project_setup()

## Qt ADD EXE ##
qt_add_executable("${PROJECT_NAME}"
    
    "include/main.h"
    "src/main.cpp"

    "include/ServerSettings.h"
    "src/ServerSettings.cpp"

    "include/DatabaseClient.h"
    "src/DatabaseClient.cpp"

    "include/JsonDoc.h"
    "src/JsonDoc.cpp"

    "include/GameManager.h"
    "src/GameManager.cpp"

    "include/TcpServer.h"
    "src/TcpServer.cpp"
    
)

target_link_libraries("${PROJECT_NAME}" PRIVATE Qt6::Core)
target_link_libraries("${PROJECT_NAME}" PRIVATE Qt6::Network)

target_link_libraries("${PROJECT_NAME}" PRIVATE mongo::bsoncxx_shared)
target_link_libraries("${PROJECT_NAME}" PRIVATE mongo::mongocxx_shared)