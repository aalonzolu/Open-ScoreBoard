#include "thememanager.h"
#include <QStyleFactory>
#include <QPalette>
#include <QDir>

ThemeManager* ThemeManager::m_instance = nullptr;

ThemeManager* ThemeManager::instance()
{
    if (!m_instance) {
        m_instance = new ThemeManager();
    }
    return m_instance;
}

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
    , m_currentTheme(Light)
    , m_settings(new QSettings("OpenScoreBoard", "Themes", this))
{
    loadThemeFromSettings();
}

void ThemeManager::setTheme(Theme theme)
{
    if (m_currentTheme != theme) {
        m_currentTheme = theme;
        applyTheme();
        saveThemeToSettings();
        emit themeChanged(theme);
    }
}

ThemeManager::Theme ThemeManager::currentTheme() const
{
    if (m_currentTheme == Auto) {
        return isSystemDarkMode() ? Dark : Light;
    }
    return m_currentTheme;
}

QString ThemeManager::currentThemeName() const
{
    switch (currentTheme()) {
        case Dark: return "Dark";
        case Light: return "Light";
        case Auto: return "Auto";
    }
    return "Light";
}

void ThemeManager::applyTheme()
{
    QString styleSheet = getThemeStyleSheet(currentTheme());
    qApp->setStyleSheet(styleSheet);
}

void ThemeManager::loadThemeFromSettings()
{
    int themeValue = m_settings->value("theme", static_cast<int>(Light)).toInt();
    m_currentTheme = static_cast<Theme>(themeValue);
}

void ThemeManager::saveThemeToSettings()
{
    m_settings->setValue("theme", static_cast<int>(m_currentTheme));
    m_settings->sync();
}

QString ThemeManager::getThemeStyleSheet(Theme theme) const
{
    switch (theme) {
        case Dark: return getDarkThemeStyleSheet();
        case Light: return getLightThemeStyleSheet();
        case Auto: return getThemeStyleSheet(isSystemDarkMode() ? Dark : Light);
    }
    return getLightThemeStyleSheet();
}

QString ThemeManager::getLightThemeStyleSheet() const
{
    return R"(
        /* Light Theme */
        QMainWindow {
            background-color: #f5f5f5;
            color: #333333;
        }
        
        QWidget {
            background-color: #ffffff;
            color: #333333;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 9pt;
        }
        
        QPushButton {
            background-color: #2196F3;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
            min-height: 24px;
        }
        
        QPushButton:hover {
            background-color: #1976D2;
        }
        
        QPushButton:pressed {
            background-color: #0D47A1;
        }
        
        QPushButton:disabled {
            background-color: #cccccc;
            color: #666666;
        }
        
        QLineEdit {
            background-color: white;
            border: 2px solid #e0e0e0;
            border-radius: 4px;
            padding: 6px 12px;
            font-size: 10pt;
        }
        
        QLineEdit:focus {
            border-color: #2196F3;
        }
        
        QFrame {
            background-color: white;
            border: 1px solid #e0e0e0;
            border-radius: 8px;
        }
        
        QLabel {
            color: #333333;
        }
        
        QLCDNumber {
            background-color: #1a1a1a;
            color: #39ff14;
            border: 2px solid #4CAF50;
            border-radius: 4px;
        }
        
        QTimeEdit {
            background-color: white;
            border: 2px solid #e0e0e0;
            border-radius: 4px;
            padding: 4px 8px;
        }
        
        /* Scoring buttons */
        QPushButton[objectName*="puntos"] {
            background-color: #4CAF50;
        }
        
        QPushButton[objectName*="puntos"]:hover {
            background-color: #388E3C;
        }
        
        QPushButton[objectName*="menos"] {
            background-color: #F44336;
        }
        
        QPushButton[objectName*="menos"]:hover {
            background-color: #D32F2F;
        }
        
        /* Timer controls */
        QPushButton[objectName*="timer"] {
            background-color: #FF9800;
        }
        
        QPushButton[objectName*="timer"]:hover {
            background-color: #F57C00;
        }
    )";
}

QString ThemeManager::getDarkThemeStyleSheet() const
{
    return R"(
        /* Dark Theme */
        QMainWindow {
            background-color: #1a1a1a;
            color: #ffffff;
        }
        
        QWidget {
            background-color: #2d2d30;
            color: #ffffff;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 9pt;
        }
        
        QPushButton {
            background-color: #0078d4;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
            min-height: 24px;
        }
        
        QPushButton:hover {
            background-color: #106ebe;
        }
        
        QPushButton:pressed {
            background-color: #005a9e;
        }
        
        QPushButton:disabled {
            background-color: #404040;
            color: #808080;
        }
        
        QLineEdit {
            background-color: #3c3c3c;
            border: 2px solid #555555;
            border-radius: 4px;
            padding: 6px 12px;
            color: #ffffff;
            font-size: 10pt;
        }
        
        QLineEdit:focus {
            border-color: #0078d4;
        }
        
        QFrame {
            background-color: #2d2d30;
            border: 1px solid #555555;
            border-radius: 8px;
        }
        
        QLabel {
            color: #ffffff;
        }
        
        QLCDNumber {
            background-color: #000000;
            color: #00ff00;
            border: 2px solid #00aa00;
            border-radius: 4px;
        }
        
        QTimeEdit {
            background-color: #3c3c3c;
            border: 2px solid #555555;
            border-radius: 4px;
            padding: 4px 8px;
            color: #ffffff;
        }
        
        /* Scoring buttons */
        QPushButton[objectName*="puntos"] {
            background-color: #16a085;
        }
        
        QPushButton[objectName*="puntos"]:hover {
            background-color: #138d75;
        }
        
        QPushButton[objectName*="menos"] {
            background-color: #e74c3c;
        }
        
        QPushButton[objectName*="menos"]:hover {
            background-color: #c0392b;
        }
        
        /* Timer controls */
        QPushButton[objectName*="timer"] {
            background-color: #f39c12;
        }
        
        QPushButton[objectName*="timer"]:hover {
            background-color: #e67e22;
        }
    )";
}

bool ThemeManager::isSystemDarkMode() const
{
    // Check system palette to determine if dark mode is active
    QPalette palette = qApp->palette();
    QColor windowColor = palette.color(QPalette::Window);
    QColor textColor = palette.color(QPalette::WindowText);
    
    // If the window color is darker than the text color, assume dark mode
    return windowColor.lightness() < textColor.lightness();
}