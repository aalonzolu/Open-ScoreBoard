#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>
#include <QApplication>
#include <QSettings>

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    enum Theme {
        Light,
        Dark,
        Auto  // Follow system theme
    };

    static ThemeManager* instance();
    
    void setTheme(Theme theme);
    Theme currentTheme() const;
    QString currentThemeName() const;
    
    void applyTheme();
    void loadThemeFromSettings();
    void saveThemeToSettings();
    
    QString getThemeStyleSheet(Theme theme) const;

signals:
    void themeChanged(Theme theme);

private:
    explicit ThemeManager(QObject *parent = nullptr);
    static ThemeManager* m_instance;
    
    Theme m_currentTheme;
    QSettings* m_settings;
    
    QString getLightThemeStyleSheet() const;
    QString getDarkThemeStyleSheet() const;
    bool isSystemDarkMode() const;
};

#endif // THEMEMANAGER_H