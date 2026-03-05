#include <QtTest>
#include <QApplication>
#include <QSignalSpy>
#include "testutils.h"

// Include the theme manager
#include "../thememanager.h"

class ThemeManagerTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    
    // Basic theme tests
    void testSingletonPattern();
    void testDefaultTheme();
    void testSetLightTheme();
    void testSetDarkTheme();
    void testSetAutoTheme();
    
    // Theme switching tests
    void testThemeChanged();
    void testThemeChangedSignal();
    void testThemeNames();
    
    // Persistence tests
    void testSaveLoadSettings();
    void testSettingsPersistence();
    
    // Stylesheet tests
    void testLightStyleSheet();
    void testDarkStyleSheet();
    void testStyleSheetApplication();
    
    // Performance tests
    void testRapidThemeChanges();
    void testMemoryUsage();

private:
    ThemeManager *themeManager;
};

void ThemeManagerTest::initTestCase()
{
    qDebug() << "Starting ThemeManagerTest suite";
    // Initialize application if not already done
    if (!QApplication::instance()) {
        int argc = 0;
        char **argv = nullptr;
        new QApplication(argc, argv);
    }
}

void ThemeManagerTest::init()
{
    themeManager = ThemeManager::instance();
    QVERIFY(themeManager != nullptr);
}

void ThemeManagerTest::cleanup()
{
    // Reset to light theme for consistency
    themeManager->setTheme(ThemeManager::Light);
}

void ThemeManagerTest::cleanupTestCase()
{
    qDebug() << "Finished ThemeManagerTest suite";
}

void ThemeManagerTest::testSingletonPattern()
{
    // Test that ThemeManager is a proper singleton
    ThemeManager *instance1 = ThemeManager::instance();
    ThemeManager *instance2 = ThemeManager::instance();
    
    QVERIFY(instance1 != nullptr);
    QVERIFY(instance2 != nullptr);
    QCOMPARE(instance1, instance2);
    
    // They should be the same object
    QVERIFY(instance1 == instance2);
}

void ThemeManagerTest::testDefaultTheme()
{
    // Test that default theme is reasonable
    ThemeManager::Theme currentTheme = themeManager->currentTheme();
    
    // Should be one of the valid themes
    QVERIFY(currentTheme == ThemeManager::Light || 
            currentTheme == ThemeManager::Dark || 
            currentTheme == ThemeManager::Auto);
    
    // Default name should be valid
    QString themeName = themeManager->currentThemeName();
    QVERIFY(!themeName.isEmpty());
    QVERIFY(themeName == "Light" || themeName == "Dark" || themeName == "Auto");
}

void ThemeManagerTest::testSetLightTheme()
{
    // Test setting light theme
    themeManager->setTheme(ThemeManager::Light);
    
    QCOMPARE(themeManager->currentTheme(), ThemeManager::Light);
    QCOMPARE(themeManager->currentThemeName(), QString("Light"));
}

void ThemeManagerTest::testSetDarkTheme()
{
    // Test setting dark theme
    themeManager->setTheme(ThemeManager::Dark);
    
    QCOMPARE(themeManager->currentTheme(), ThemeManager::Dark);
    QCOMPARE(themeManager->currentThemeName(), QString("Dark"));
}

void ThemeManagerTest::testSetAutoTheme()
{
    // Test setting auto theme
    themeManager->setTheme(ThemeManager::Auto);
    
    // The stored theme should be Auto
    // But currentTheme() might return Light or Dark based on system
    QString themeName = themeManager->currentThemeName();
    QCOMPARE(themeName, QString("Auto"));
}

void ThemeManagerTest::testThemeChanged()
{
    // Test that theme changes are detected correctly
    ThemeManager::Theme initialTheme = themeManager->currentTheme();
    
    // Change to different theme
    ThemeManager::Theme newTheme = (initialTheme == ThemeManager::Light) ? 
                                  ThemeManager::Dark : ThemeManager::Light;
    
    themeManager->setTheme(newTheme);
    
    QCOMPARE(themeManager->currentTheme(), newTheme);
    QVERIFY(themeManager->currentTheme() != initialTheme);
}

void ThemeManagerTest::testThemeChangedSignal()
{
    // Test that themeChanged signal is emitted
    QSignalSpy spy(themeManager, &ThemeManager::themeChanged);
    
    // Change theme
    themeManager->setTheme(ThemeManager::Dark);
    
    // Should have emitted signal
    QCOMPARE(spy.count(), 1);
    
    // Check signal argument
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).value<ThemeManager::Theme>(), ThemeManager::Dark);
    
    // Changing to same theme should not emit signal
    spy.clear();
    themeManager->setTheme(ThemeManager::Dark);
    QCOMPARE(spy.count(), 0);
}

void ThemeManagerTest::testThemeNames()
{
    // Test all theme names
    themeManager->setTheme(ThemeManager::Light);
    QCOMPARE(themeManager->currentThemeName(), QString("Light"));
    
    themeManager->setTheme(ThemeManager::Dark);
    QCOMPARE(themeManager->currentThemeName(), QString("Dark"));
    
    themeManager->setTheme(ThemeManager::Auto);
    QCOMPARE(themeManager->currentThemeName(), QString("Auto"));
}

void ThemeManagerTest::testSaveLoadSettings()
{
    // Test settings persistence
    ThemeManager::Theme testTheme = ThemeManager::Dark;
    
    // Set theme and save
    themeManager->setTheme(testTheme);
    themeManager->saveThemeToSettings();
    
    // Simulate loading settings (in real scenario would be on app restart)
    themeManager->loadThemeFromSettings();
    
    // Theme should be restored
    // Note: currentTheme() might differ from stored theme for Auto mode
    QCOMPARE(themeManager->currentThemeName(), QString("Dark"));
}

void ThemeManagerTest::testSettingsPersistence()
{
    // Test that settings persist across multiple saves/loads
    QList<ThemeManager::Theme> themes = {
        ThemeManager::Light,
        ThemeManager::Dark,
        ThemeManager::Auto,
        ThemeManager::Light
    };
    
    for (ThemeManager::Theme theme : themes) {
        themeManager->setTheme(theme);
        themeManager->saveThemeToSettings();
        themeManager->loadThemeFromSettings();
        
        // Verify theme is consistent
        QString expectedName;
        switch (theme) {
            case ThemeManager::Light: expectedName = "Light"; break;
            case ThemeManager::Dark: expectedName = "Dark"; break;
            case ThemeManager::Auto: expectedName = "Auto"; break;
        }
        
        QCOMPARE(themeManager->currentThemeName(), expectedName);
    }
}

void ThemeManagerTest::testLightStyleSheet()
{
    // Test light theme stylesheet
    QString lightStyleSheet = themeManager->getThemeStyleSheet(ThemeManager::Light);
    
    QVERIFY(!lightStyleSheet.isEmpty());
    QVERIFY(lightStyleSheet.contains("Light Theme"));
    QVERIFY(lightStyleSheet.contains("QPushButton"));
    QVERIFY(lightStyleSheet.contains("background-color"));
    
    // Should not contain dark theme markers
    QVERIFY(!lightStyleSheet.contains("Dark Theme"));
}

void ThemeManagerTest::testDarkStyleSheet()
{
    // Test dark theme stylesheet
    QString darkStyleSheet = themeManager->getThemeStyleSheet(ThemeManager::Dark);
    
    QVERIFY(!darkStyleSheet.isEmpty());
    QVERIFY(darkStyleSheet.contains("Dark Theme"));
    QVERIFY(darkStyleSheet.contains("QPushButton"));
    QVERIFY(darkStyleSheet.contains("background-color"));
    
    // Should not contain light theme markers
    QVERIFY(!darkStyleSheet.contains("Light Theme"));
    
    // Should have different colors than light theme
    QString lightStyleSheet = themeManager->getThemeStyleSheet(ThemeManager::Light);
    QVERIFY(darkStyleSheet != lightStyleSheet);
}

void ThemeManagerTest::testStyleSheetApplication()
{
    // Test that stylesheet is applied to application
    QString initialStyleSheet = qApp->styleSheet();
    
    // Apply light theme
    themeManager->setTheme(ThemeManager::Light);
    themeManager->applyTheme();
    
    QString lightAppStyleSheet = qApp->styleSheet();
    
    // Apply dark theme  
    themeManager->setTheme(ThemeManager::Dark);
    themeManager->applyTheme();
    
    QString darkAppStyleSheet = qApp->styleSheet();
    
    // Stylesheets should be different
    QVERIFY(lightAppStyleSheet != darkAppStyleSheet);
    
    // Both should be non-empty
    QVERIFY(!lightAppStyleSheet.isEmpty());
    QVERIFY(!darkAppStyleSheet.isEmpty());
}

void ThemeManagerTest::testRapidThemeChanges()
{
    // Test rapid theme switching doesn't cause issues
    QSignalSpy spy(themeManager, &ThemeManager::themeChanged);
    
    // Rapidly switch themes
    for (int i = 0; i < 20; ++i) {
        ThemeManager::Theme theme = (i % 2 == 0) ? ThemeManager::Light : ThemeManager::Dark;
        themeManager->setTheme(theme);
        
        // Small delay to simulate real usage
        QTest::qWait(1);
    }
    
    // Should have received appropriate number of signals
    QVERIFY(spy.count() > 0);
    QVERIFY(spy.count() <= 20); // Might be less due to duplicate theme sets
    
    // Final theme should be correct
    QVERIFY(themeManager->currentTheme() == ThemeManager::Light || 
            themeManager->currentTheme() == ThemeManager::Dark);
}

void ThemeManagerTest::testMemoryUsage()
{
    // Test that theme manager doesn't leak memory during operations
    QString initialStyleSheet = qApp->styleSheet();
    
    // Perform many theme operations
    for (int cycle = 0; cycle < 10; ++cycle) {
        for (int theme = 0; theme < 3; ++theme) {
            themeManager->setTheme(static_cast<ThemeManager::Theme>(theme));
            themeManager->applyTheme();
            themeManager->saveThemeToSettings();
            themeManager->loadThemeFromSettings();
        }
    }
    
    // Should still be functional
    themeManager->setTheme(ThemeManager::Light);
    QCOMPARE(themeManager->currentTheme(), ThemeManager::Light);
    
    // Memory usage test would require more sophisticated tooling
    // For now, we just verify basic functionality still works
    QString finalStyleSheet = qApp->styleSheet();
    QVERIFY(!finalStyleSheet.isEmpty());
}

QTEST_MAIN(ThemeManagerTest)
#include "test_thememanager.moc"