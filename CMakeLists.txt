cmake_minimum_required(VERSION 3.21)
project(veyon-chat-plugin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt (same major as Veyon 4.9.x in Actions step below)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Use your current filenames as-is (root-level files in your repo)
set(CHAT_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/ChatPlugin.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/MasterChatDialog.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/ClientChatDialog.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/ChatPlugin.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/MasterChatDialog.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/ClientChatDialog.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/plugin.json"
)

# Bundle your existing resources.qrc (root-level)
qt6_add_resources(CHAT_QRC "${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc")

# Build a MODULE (plugin) so it drops as chat.dll on Windows
add_library(chat MODULE ${CHAT_SRC} ${CHAT_QRC})

target_include_directories(chat PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(chat PRIVATE QT_NO_KEYWORDS)
target_link_libraries(chat PRIVATE Qt6::Widgets)

# If we are inside the Veyon build, these targets exist — link to them.
foreach(tgt IN ITEMS veyon-core veyon::core VeyonCore)
  if(TARGET ${tgt})
    target_link_libraries(chat PRIVATE ${tgt})
    set(_VEYON_CORE_FOUND ON)
    break()
  endif()
endforeach()
foreach(tgt IN ITEMS veyon-master veyon::master VeyonMaster)
  if(TARGET ${tgt})
    target_link_libraries(chat PRIVATE ${tgt})
    set(_VEYON_MASTER_FOUND ON)
    break()
  endif()
endforeach()

if(NOT _VEYON_CORE_FOUND OR NOT _VEYON_MASTER_FOUND)
  message(STATUS "Note: Veyon core/master targets not found. (Fine when configuring out-of-tree, but not used in Option A.)")
endif()

# Install next to other Veyon plugins
install(TARGETS chat RUNTIME DESTINATION "plugins" LIBRARY DESTINATION "plugins")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/plugin.json" DESTINATION "plugins/VeyonChat")
