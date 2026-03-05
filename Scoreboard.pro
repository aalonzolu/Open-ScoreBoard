#-------------------------------------------------
#
# Open ScoreBoard - Basketball Scoring Application
# Updated for Qt 6.5+ compatibility - 2026
#
#-------------------------------------------------

QT += core widgets multimedia

# Require Qt 6.5 or higher
lessThan(QT_MAJOR_VERSION, 6) {
    error("This application requires Qt 6.5 or higher")
}

# C++ standard
CONFIG += c++17

# Application settings
TARGET = Open-ScoreBoard
TEMPLATE = app
VERSION = 2.0.0

# Output directories
CONFIG(debug, debug|release) {
    DESTDIR = build/debug
    OBJECTS_DIR = build/debug/obj
    MOC_DIR = build/debug/moc
    UI_DIR = build/debug/ui
    RCC_DIR = build/debug/rcc
} else {
    DESTDIR = build/release
    OBJECTS_DIR = build/release/obj
    MOC_DIR = build/release/moc
    UI_DIR = build/release/ui
    RCC_DIR = build/release/rcc
}

# Platform-specific configurations
win32 {
    CONFIG += windows
    RC_ICONS = images/balon.ico
}

macx {
    ICON = images/balon.icns
    QMAKE_INFO_PLIST = Info.plist
}

unix:!macx {
    # Linux configuration
    target.path = /usr/local/bin
    INSTALLS += target
}

# Source files
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/boardw.cpp \
    src/thememanager.cpp

# Header files
HEADERS += \
    src/mainwindow.h \
    src/boardw.h \
    src/thememanager.h

# UI forms
FORMS += \
    src/mainwindow.ui \
    src/boardw.ui

# Resources
RESOURCES += \
    src/resources.qrc

# Additional compiler flags
QMAKE_CXXFLAGS += -Wall -Wextra

# Debug/Release specific settings
CONFIG(debug, debug|release) {
    DEFINES += DEBUG_BUILD
    QMAKE_CXXFLAGS += -g
} else {
    DEFINES += RELEASE_BUILD
    QMAKE_CXXFLAGS += -O2
}
