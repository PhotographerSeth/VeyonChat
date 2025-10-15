cmake_minimum_required(VERSION 3.16)
project(VeyonChat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 5.12 REQUIRED COMPONENTS Core Gui Widgets Network)

if(NOT DEFINED ENV{VEYON_BUILD_DIR} OR "$ENV{VEYON_BUILD_DIR}" STREQUAL "")
    message(FATAL_ERROR "The VEYON_BUILD_DIR environment variable must be set to the root of a Veyon build tree.")
endif()

set(VEYON_BUILD_DIR "$ENV{VEYON_BUILD_DIR}")

find_library(VEYON_CORE_LIBRARY
    NAMES VeyonCore
    HINTS "${VEYON_BUILD_DIR}/lib" "${VEYON_BUILD_DIR}/lib64"
)

find_library(VEYON_MASTER_LIBRARY
    NAMES VeyonMaster
    HINTS "${VEYON_BUILD_DIR}/lib" "${VEYON_BUILD_DIR}/lib64"
)

if(NOT VEYON_CORE_LIBRARY)
    message(FATAL_ERROR "Could not find the VeyonCore library. Ensure VEYON_BUILD_DIR points to a compiled Veyon tree.")
endif()

if(NOT VEYON_MASTER_LIBRARY)
    message(FATAL_ERROR "Could not find the VeyonMaster library. Ensure VEYON_BUILD_DIR points to a compiled Veyon tree.")
endif()

add_library(VeyonChat SHARED
    ChatPlugin.cpp
    MasterChatDialog.cpp
    ClientChatDialog.cpp
    resources.qrc
)

target_include_directories(VeyonChat
    PRIVATE
        "${VEYON_BUILD_DIR}/include"
)

target_compile_definitions(VeyonChat
    PRIVATE
        VEYON_CHAT_PLUGIN_SHARED
)

target_link_libraries(VeyonChat
    PRIVATE
        ${VEYON_CORE_LIBRARY}
        ${VEYON_MASTER_LIBRARY}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
)

install(TARGETS VeyonChat DESTINATION VeyonChat)
install(FILES plugin.json DESTINATION VeyonChat)
