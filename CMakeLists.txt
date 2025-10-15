cmake_minimum_required(VERSION 3.21)
project(veyon-chat-plugin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Sources
file(GLOB CHAT_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
qt6_add_resources(CHAT_QRC "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc")

# Build a plugin DLL
add_library(chat MODULE ${CHAT_SRC} ${CHAT_QRC} "${CMAKE_CURRENT_SOURCE_DIR}/plugin.json")

target_include_directories(chat PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(chat PRIVATE QT_NO_KEYWORDS)

target_link_libraries(chat PRIVATE Qt6::Widgets)

# --- Try to link against Veyon targets when we are inside Veyon build ---
# These names cover common target name variants across Veyon versions.
set(_VEYON_CORE_TGTS veyon-core veyon::core VeyonCore)
set(_VEYON_MASTER_TGTS veyon-master veyon::master VeyonMaster)
foreach(t ${_VEYON_CORE_TGTS})
  if(TARGET ${t})
    target_link_libraries(chat PRIVATE ${t})
    set(_VEYON_CORE_FOUND ON)
    break()
  endif()
endforeach()
foreach(t ${_VEYON_MASTER_TGTS})
  if(TARGET ${t})
    target_link_libraries(chat PRIVATE ${t})
    set(_VEYON_MASTER_FOUND ON)
    break()
  endif()
endforeach()

if(NOT _VEYON_CORE_FOUND OR NOT _VEYON_MASTER_FOUND)
  message(STATUS "Veyon core/master targets not found yet. If building out-of-tree, set include paths/libs accordingly.")
endif()

# Install next to Veyon plugins in the build/install
install(TARGETS chat RUNTIME DESTINATION "plugins" LIBRARY DESTINATION "plugins")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/plugin.json" DESTINATION "plugins/VeyonChat")
