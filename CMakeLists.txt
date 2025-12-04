cmake_minimum_required(VERSION 3.16)
project(VeyonChat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_MIN_VERSION 5.12)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Gui Widgets Network)

if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 OPTIONAL_COMPONENTS Core5Compat)
endif()

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
    src/ChatPlugin.cpp
    src/MasterChatDialog.cpp
    src/ClientChatDialog.cpp
    resources/resources.qrc
)

set(QT_LINK_LIBRARIES
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

if(QT_VERSION_MAJOR EQUAL 6 AND TARGET Qt6::Core5Compat)
    list(APPEND QT_LINK_LIBRARIES Qt6::Core5Compat)
endif()

target_include_directories(VeyonChat
    PRIVATE
        "${VEYON_BUILD_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_definitions(VeyonChat
    PRIVATE
        VEYON_CHAT_PLUGIN_SHARED
)

if(QT_VERSION_MAJOR EQUAL 6 AND TARGET Qt6::Core5Compat)
    target_compile_definitions(VeyonChat
        PRIVATE
            QT_DISABLE_DEPRECATED_BEFORE=0x050F00
    )
endif()

target_link_libraries(VeyonChat
    PRIVATE
        ${VEYON_CORE_LIBRARY}
        ${VEYON_MASTER_LIBRARY}
        ${QT_LINK_LIBRARIES}
)

install(TARGETS VeyonChat DESTINATION VeyonChat)
install(FILES plugin.json DESTINATION VeyonChat)
