# VeyonChat.pro
#
# This file is now configured to work with the GitHub Actions CI workflow.
# It uses environment variables ($$(VEYON_BUILD_DIR)) to locate Veyon's
# headers and libraries in the automated build environment.

QT       += core gui widgets network
TARGET   = VeyonChat
TEMPLATE = lib

DEFINES += VEYON_CHAT_PLUGIN_SHARED

# Get the Veyon build directory from an environment variable
VEYON_BUILD_DIR = $$(VEYON_BUILD_DIR)

# Check if the environment variable is set
isEmpty(VEYON_BUILD_DIR) {
    error("The VEYON_BUILD_DIR environment variable is not set. This project must be built in the GitHub Actions environment.")
}

# Use the environment variable to set include and library paths
INCLUDEPATH += $$VEYON_BUILD_DIR/include
LIBS += -L$$VEYON_BUILD_DIR/lib -lVeyonCore -lVeyonMaster

SOURCES += \
    ChatPlugin.cpp \
    MasterChatDialog.cpp \
    ClientChatDialog.cpp

HEADERS += \
    ChatPlugin.h \
    MasterChatDialog.h \
    ClientChatDialog.h

RESOURCES += \
    resources.qrc

# This part is for deployment within the GitHub Action
target.path = $$OUT_PWD/install/VeyonChat
INSTALLS += target