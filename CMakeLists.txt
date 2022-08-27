cmake_minimum_required(VERSION 3.5)

project(TevianTest)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets Network Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Gui)

file (GLOB_RECURSE PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/src/*)

set (CMAKE_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/src/)
add_executable(TevianTest
    ${PROJECT_SOURCES}
)

target_link_libraries(TevianTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Gui)

install(TARGETS TevianTest
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
