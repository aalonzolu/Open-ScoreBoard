# Open-ScoreBoard Developer Documentation

## Architecture Overview

Open-ScoreBoard is a modern Qt 6.5+ based basketball scoreboard application with a clean separation between the control interface (MainWindow) and the display interface (BoardW).

### Core Components

#### 1. MainWindow (mainwindow.h/cpp)
The main control interface that handles:
- Game timer controls (start, pause, stop, reset)
- Score management for both teams
- Foul tracking and bonus system
- Period/quarter management  
- Team setup (names, logos)
- Theme switching via menu
- Keyboard shortcuts for live game control

**Key Responsibilities:**
- User input handling
- Game state management
- Communication with scoreboard display
- Settings persistence

#### 2. BoardW (boardw.h/cpp)
The scoreboard display window that shows:
- Current game time
- Team scores and fouls
- Period indicator
- Team logos and names
- Ball possession indicator
- Bonus/penalty status

**Key Responsibilities:**
- Visual game state display
- Multi-monitor support 
- Theme synchronization
- Responsive layout handling

#### 3. ThemeManager (thememanager.h/cpp)
Centralized theme management system providing:
- Light/Dark theme switching
- Auto theme based on system settings
- CSS stylesheet generation
- Settings persistence
- Application-wide theme synchronization

**Key Responsibilities:**
- Theme state management
- Stylesheet generation and application
- Settings persistence
- Signal emission for theme changes

### Communication Patterns

#### MainWindow ↔ BoardW
The MainWindow controls the BoardW through direct method calls:
- `updatePuntos(team1Score, team2Score)` - Update scores
- `updateFaltas(team1Fouls, team2Fouls)` - Update foul counts
- `updatePeriodo(period)` - Update current period
- `updateBonus(team)` - Set bonus/penalty status
- `setLCD(minutes, seconds)` - Update timer display
- `balonPOS(team)` - Set ball possession

#### Theme Synchronization
All components listen to ThemeManager signals:
```cpp
connect(ThemeManager::instance(), &ThemeManager::themeChanged,
        this, &ComponentClass::onThemeChanged);
```

#### Timer System
The timer operates on a 1-second QTimer that:
1. Decrements the game time
2. Updates displays in MainWindow
3. Synchronizes with BoardW through `setLCD()`
4. Triggers buzzer when time expires

### Game State Management

#### Core Game Variables (MainWindow)
```cpp
int PUNTOS1, PUNTOS2;    // Team scores
int FALTAS1, FALTAS2;    // Team fouls
int PERIODO;             // Current period/quarter
int STATUS;              // Timer status (0=stopped, 1=running, 2=paused)
QTime* TIME;            // Current game time
```

#### Status Values
- `STATUS = 0`: Timer stopped
- `STATUS = 1`: Timer running  
- `STATUS = 2`: Timer paused

#### Foul System
- 4 fouls triggers bonus for opposing team
- Fouls reset to 0 when timer starts (if at 4)
- Bonus status is synchronized between MainWindow and BoardW

### Keyboard Controls

The application provides comprehensive keyboard shortcuts for live game operation:

#### Team 1 Controls
- **W**: Increase score (+1)
- **S**: Decrease score (-1) 
- **D**: Increase fouls (+1)
- **A**: Decrease fouls (-1)

#### Team 2 Controls  
- **I**: Increase score (+1)
- **K**: Decrease score (-1)
- **L**: Increase fouls (+1)
- **J**: Decrease fouls (-1)

#### Game Controls
- **P**: Toggle play/pause timer
- **1**: Set ball possession to Team 1
- **2**: Set ball possession to Team 2
- **0**: Increase period (+1)
- **9**: Decrease period (-1)

### Theme System Details

#### Theme Types
```cpp
enum Theme {
    Light,  // Light theme with bright backgrounds
    Dark,   // Dark theme with dark backgrounds  
    Auto    // Follow system theme preference
};
```

#### CSS Generation
Themes are implemented via comprehensive CSS stylesheets that cover:
- Window and widget backgrounds
- Button styles (normal, hover, pressed, disabled)
- Line edit field styling
- LCD display colors
- Frame and border styling
- Typography and font settings

#### Theme Persistence
- Themes are saved to QSettings under "OpenScoreBoard/Themes"
- Settings are loaded on application startup
- Changes are saved immediately when theme is switched

### Build System

#### CMake (Primary)
Modern CMake-based build system with:
- Qt 6.5+ auto-detection
- Cross-platform support 
- Automatic MOC, UIC, RCC
- Test suite integration
- Install rules for packaging

**Key CMake Targets:**
- `OpenScoreBoard` - Main executable
- `run` - Custom target to run the application
- `run_all_tests` - Execute complete test suite

#### qmake (Legacy Support)
Updated .pro file maintaining compatibility for users preferring qmake:
- Qt 6 requirement enforcement
- C++17 standard specification
- Platform-specific configurations
- Organized build directories

### Testing Framework

#### Test Structure
Comprehensive QtTest-based testing with multiple suites:

1. **TimerLogicTest** - Timer functionality validation
2. **ScoringLogicTest** - Scoring system verification  
3. **ThemeManagerTest** - Theme system testing
4. **GameStateTest** - Integration testing

#### Test Utilities (TestUtils)
Shared testing utilities providing:
- Window management helpers
- Input simulation functions
- Mock data generation
- Timing and synchronization utilities
- Image comparison for UI testing

#### Running Tests
```bash
# Build and run all tests
cd build
ctest --output-on-failure --verbose

# Or run individual test suites
./TimerLogicTest
./ScoringLogicTest
./ThemeManagerTest  
./GameStateTest
```

### Memory Management

#### Qt Parent-Child System
The application leverages Qt's parent-child object management:
- UI objects have proper parent relationships
- QTimer, QSoundEffect have parent objects
- BoardW window has MainWindow as parent

#### Manual Cleanup
Critical objects requiring explicit cleanup:
- `QTime* TIME` - Deleted in MainWindow destructor
- `Ui::MainWindow* ui` - Deleted by Qt's destructor convention
- `Ui::BoardW* ui` - Deleted by Qt's destructor convention

### Error Handling

#### Input Validation
- Button clicks are handled gracefully
- Invalid keyboard input is passed to parent handlers
- File dialogs have proper error checking

#### State Consistency
- Timer state is validated on operations
- Score boundaries are enforced by UI constraints
- Theme changes are atomic and reversible

### Resource Management

#### Resource System
- Images and sounds embedded via Qt Resource System (.qrc)
- Path consistency using `:/` prefix for embedded resources
- Fallback handling for missing resources

#### Image Handling
- Logo scaling maintains aspect ratio
- Resize events properly update image displays
- Ball possession images are pre-loaded for performance

### Development Guidelines

#### Code Style
- Modern C++17 practices
- Qt naming conventions (camelCase for functions, PascalCase for classes)
- Comprehensive comments for public interfaces
- Consistent indentation (4 spaces)

#### Qt Best Practices
- Use modern connect() syntax with function pointers
- Leverage parent-child relationships for memory management
- Prefer Qt containers and algorithms
- Use Qt's meta-object system appropriately

#### Testing Guidelines
- Write tests for all public interfaces
- Include edge case testing
- Use mock data for consistency
- Performance test critical paths

### Deployment

#### Platform-Specific Notes

**Windows:**
- Use windeployqt for dependency bundling
- Executable + DLL distribution model
- MSVC runtime requirements

**Linux:**
- AppImage for portable distribution
- .desktop file for system integration
- Dependency handling via AppImage

**macOS:**
- .app bundle with macdeployqt
- .dmg creation for distribution
- Code signing considerations

#### CI/CD Integration
GitHub Actions workflow provides:
- Matrix builds for all platforms
- Automated testing on each commit
- Release artifact generation
- Code quality analysis

### Future Enhancements

#### Planned Features
- Network-based remote scoreboards
- Game statistics tracking
- Export functionality for game data
- Additional sports support
- Custom branding options

#### Technical Debt
- Consider migrating to Qt 6.8+ when stable
- Evaluate QML for more dynamic UI
- Real-time collaboration features
- Mobile companion app

### Troubleshooting

#### Common Issues

**Build Failures:**
- Ensure Qt 6.5+ is installed and in PATH
- Verify C++17 compiler support
- Check CMake version (3.16+ required)

**Runtime Issues:**
- Missing Qt libraries: Use deployment tools (windeployqt, etc.)
- Theme not applying: Check QSettings permissions
- Sound not working: Verify multimedia module installation

**Development Setup:**
- Qt Creator integration: Open CMakeLists.txt as project
- VS Code integration: Use Qt extension + CMake Tools
- Command line builds: Follow README instructions

### API Reference

#### MainWindow Public Interface
```cpp
// Timer Control
void startTimer();  
void pauseTimer();
void stopTimer();
void resetTimer();
int getStatus();

// Scoring
int Punteo();      // Handle score button clicks
int Faltas();      // Handle foul button clicks  
int Periodo();     // Handle period button clicks
int Bonus(int team);

// Display
void showScoreboard();
void setPixmap1();  // Set team 1 logo
void setPixmap2();  // Set team 2 logo
```

#### BoardW Public Interface  
```cpp
// Display Updates
void setLCD(int minute, int second);
int updatePuntos(int score1, int score2);
int updateFaltas(int fouls1, int fouls2);
int updatePeriodo(int period);  
int updateBonus(int team);
void balonPOS(int team);

// Visual Management
void SetPixmap(int team, QString imagePath);
void SHOW();  // Multi-monitor display
void applyCurrentTheme();
```

#### ThemeManager Public Interface
```cpp
// Theme Control
static ThemeManager* instance();
void setTheme(Theme theme);
Theme currentTheme() const;
QString currentThemeName() const;

// Application  
void applyTheme();
void loadThemeFromSettings();
void saveThemeToSettings();

// Style Generation
QString getThemeStyleSheet(Theme theme) const;
```

This documentation provides a comprehensive overview of the Open-ScoreBoard architecture, enabling developers to understand, maintain, and extend the application effectively.