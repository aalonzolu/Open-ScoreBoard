cmake_minimum_required(VERSION 3.16)
project(OpenScoreBoard VERSION 2.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 components
find_package(Qt6 6.4 REQUIRED COMPONENTS 
    Core
    Widgets 
    Multimedia
    Test
)

# Enable Qt6 automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/boardw.cpp
    src/thememanager.cpp
)

# Header files
set(HEADERS
    src/mainwindow.h
    src/boardw.h
    src/thememanager.h
)

# UI files
set(UI_FILES
    src/mainwindow.ui
    src/boardw.ui
)

# Resource files
set(RESOURCES
    src/resources.qrc
)

# Create executable
add_executable(OpenScoreBoard WIN32 MACOSX_BUNDLE
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCES}
)

# Link Qt6 libraries
target_link_libraries(OpenScoreBoard
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
)

# Set target properties
set_target_properties(OpenScoreBoard PROPERTIES
    OUTPUT_NAME "Open-ScoreBoard"
    MACOSX_BUNDLE_BUNDLE_NAME "Open ScoreBoard"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.openscoreboard.app"
    WIN32_EXECUTABLE TRUE
)

# Platform-specific configurations
if(WIN32)
    # Windows-specific settings
    set_target_properties(OpenScoreBoard PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
elseif(APPLE)
    # macOS-specific settings
    set_target_properties(OpenScoreBoard PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_ICON_FILE "balon.png"
    )
elseif(UNIX)
    # Linux-specific settings - nothing special needed
endif()

# Install rules
install(TARGETS OpenScoreBoard
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

# Custom target for running the application
add_custom_target(run
    COMMAND OpenScoreBoard
    DEPENDS OpenScoreBoard
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Testing support (optional - for future tests)
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests OPTIONAL)
endif()

# IDE support - organize files in folders
source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
source_group("UI Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RESOURCES})

# Print configuration summary
message(STATUS "Open-ScoreBoard configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}") 
message(STATUS "  Qt version: ${Qt6_VERSION}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")