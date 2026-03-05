cmake_minimum_required(VERSION 3.16)

# Find Qt Test module 
find_package(Qt6 REQUIRED COMPONENTS Test)

# Enable automatic MOC for tests
set(CMAKE_AUTOMOC ON)

# Test utilities for shared functionality
add_library(TestUtils STATIC
    testutils.cpp
    testutils.h
)

target_link_libraries(TestUtils
    Qt6::Test
    Qt6::Core
)

# Function to create a test executable
function(add_qt_test test_name test_sources)
    add_executable(${test_name} ${test_sources})
    
    target_link_libraries(${test_name}
        Qt6::Test
        Qt6::Widgets
        Qt6::Multimedia
        TestUtils
    )
    
    # Add the sources from the main application
    target_sources(${test_name} PRIVATE
        ../src/mainwindow.cpp
        ../src/boardw.cpp  
        ../src/thememanager.cpp
    )
    
    target_include_directories(${test_name} PRIVATE
        ${CMAKE_SOURCE_DIR}
    )
    
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Unit Tests
add_qt_test(TimerLogicTest test_timerlogic.cpp)
add_qt_test(ScoringLogicTest test_scoringlogic.cpp)
add_qt_test(ThemeManagerTest test_thememanager.cpp)
add_qt_test(GameStateTest test_gamestate.cpp)

# Integration Tests
add_qt_test(MainWindowIntegrationTest test_mainwindow_integration.cpp)
add_qt_test(ScoreboardIntegrationTest test_scoreboard_integration.cpp)

# UI Tests (require X server or virtual display)
add_qt_test(UIInteractionTest test_ui_interaction.cpp)

# Performance Tests
add_qt_test(PerformanceTest test_performance.cpp)

# Test runner target
add_custom_target(run_all_tests
    COMMAND ctest --output-on-failure --verbose
    DEPENDS 
        TimerLogicTest
        ScoringLogicTest  
        ThemeManagerTest
        GameStateTest
        MainWindowIntegrationTest
        ScoreboardIntegrationTest
        UIInteractionTest
        PerformanceTest
    COMMENT "Running all tests"
)