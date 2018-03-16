cmake_minimum_required(VERSION 3.5)

project(painterPath)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOQRC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

FILE (GLOB QML
    main.qml
)

FILE (GLOB HDR
    SGPainterPathProxy.h
)

FILE (GLOB SRC
    main.cpp
    SGPainterPathProxy.cpp
    res.qrc
)

add_executable(${PROJECT_NAME} ${SRC} ${HDR} ${QML})

source_group("QML Files" FILES ${QML})
source_group("SRC" FILES ${SRC})
source_group("HDR" FILES ${HDR})

target_include_directories(${PROJECT_NAME} PRIVATE ${Qt5Core_PRIVATE_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${Qt5Gui_PRIVATE_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Qml
    Qt5::Gui
    Qt5::Quick
)
