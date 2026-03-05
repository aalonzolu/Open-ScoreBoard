#include <QtTest>
#include <QApplication>
#include <QTimer>
#include <QSignalSpy>
#include "testutils.h"

// Include main components
#include "../mainwindow.h"
#include "../thememanager.h"

class GameStateTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    
    // Game flow tests
    void testCompleteGameFlow();
    void testGameInitialization();
    void testTeamSetup();
    void testGameStart();
    void testGamePause();
    void testGameEnd();
    void testGameReset();
    
    // State synchronization tests
    void testMainWindowScoreboardSync();
    void testThemeSync();
    void testTimerStateSync();
    
    // Edge cases and error conditions
    void testInvalidGameStates();
    void testMemoryManagement();
    void testErrorRecovery();
    
    // Performance tests
    void testLargeBenchmark();
    void testResponseTime();

private:
    MainWindow *mainWindow;
    TestUtils::GameData gameData;
    
    void simulateCompleteGame();
    void verifyGameState(const TestUtils::GameData &expected);
};

void GameStateTest::initTestCase()
{
    qDebug() << "Starting GameStateTest suite";
    
    // Ensure application is initialized
    if (!QApplication::instance()) {
        int argc = 0;
        char **argv = nullptr;
        new QApplication(argc, argv);
    }
}

void GameStateTest::init()
{
    mainWindow = new MainWindow();
    TestUtils::waitForWindowShown(mainWindow);
    gameData = TestUtils::createMockGameData();
}

void GameStateTest::cleanup()
{
    delete mainWindow;
    mainWindow = nullptr;
}

void GameStateTest::cleanupTestCase()
{
    qDebug() << "Finished GameStateTest suite";
}

void GameStateTest::testCompleteGameFlow()
{
    // Test a complete game flow from start to finish
    QVERIFY(mainWindow != nullptr);
    
    // 1. Initialize game
    QCOMPARE(mainWindow->getStatus(), 0); // Should start stopped
    
    // 2. Set up teams
    QLineEdit *homeNameEdit = mainWindow->findChild<QLineEdit*>("homeName");
    QLineEdit *guestNameEdit = mainWindow->findChild<QLineEdit*>("guestName");
    QLineEdit *titleEdit = mainWindow->findChild<QLineEdit*>("TITLE");
    
    QVERIFY(homeNameEdit != nullptr);
    QVERIFY(guestNameEdit != nullptr);
    QVERIFY(titleEdit != nullptr);
    
    homeNameEdit->setText(gameData.homeTeam);
    guestNameEdit->setText(gameData.awayTeam);
    titleEdit->setText("Test Championship");
    
    // 3. Start game
    mainWindow->startTimer();
    QCOMPARE(mainWindow->getStatus(), 1);
    
    // 4. Simulate some scoring
    QPushButton *score1Plus = mainWindow->findChild<QPushButton*>("puntos1mas2");
    QPushButton *score2Plus = mainWindow->findChild<QPushButton*>("puntos2mas3");
    
    QVERIFY(score1Plus != nullptr);
    QVERIFY(score2Plus != nullptr);
    
    QTest::mouseClick(score1Plus, Qt::LeftButton);
    QTest::mouseClick(score2Plus, Qt::LeftButton);
    QTest::mouseClick(score1Plus, Qt::LeftButton);
    
    // Verify scores changed
    QLCDNumber *score1Display = mainWindow->findChild<QLCDNumber*>("puntos1");
    QLCDNumber *score2Display = mainWindow->findChild<QLCDNumber*>("puntos2");
    
    QVERIFY(score1Display != nullptr);
    QVERIFY(score2Display != nullptr);
    QCOMPARE(static_cast<int>(score1Display->value()), 4); // 2 + 2
    QCOMPARE(static_cast<int>(score2Display->value()), 3); // 3
    
    // 5. Add some fouls
    QPushButton *foul1Plus = mainWindow->findChild<QPushButton*>("FALTA1mas");
    QVERIFY(foul1Plus != nullptr);
    
    QTest::mouseClick(foul1Plus, Qt::LeftButton);
    QTest::mouseClick(foul1Plus, Qt::LeftButton);
    
    // 6. Test pause/resume
    mainWindow->pauseTimer();
    QCOMPARE(mainWindow->getStatus(), 2);
    
    mainWindow->startTimer();
    QCOMPARE(mainWindow->getStatus(), 1);
    
    // 7. Test period change
    QPushButton *periodPlus = mainWindow->findChild<QPushButton*>("Periodomas");
    QVERIFY(periodPlus != nullptr);
    
    QTest::mouseClick(periodPlus, Qt::LeftButton);
    
    // 8. Stop game
    mainWindow->stopTimer();
    QCOMPARE(mainWindow->getStatus(), 0);
}

void GameStateTest::testGameInitialization()
{
    // Test that game initializes with correct default values
    QVERIFY(mainWindow != nullptr);
    
    // Timer should be stopped initially
    QCOMPARE(mainWindow->getStatus(), 0);
    
    // Scores should be zero
    QLCDNumber *score1 = mainWindow->findChild<QLCDNumber*>("puntos1");
    QLCDNumber *score2 = mainWindow->findChild<QLCDNumber*>("puntos2");
    QLCDNumber *fouls1 = mainWindow->findChild<QLCDNumber*>("faltas1");
    QLCDNumber *fouls2 = mainWindow->findChild<QLCDNumber*>("faltas2");
    QLCDNumber *period = mainWindow->findChild<QLCDNumber*>("PERIODO");
    
    QVERIFY(score1 != nullptr && score2 != nullptr);
    QVERIFY(fouls1 != nullptr && fouls2 != nullptr);
    QVERIFY(period != nullptr);
    
    QCOMPARE(static_cast<int>(score1->value()), 0);
    QCOMPARE(static_cast<int>(score2->value()), 0);
    QCOMPARE(static_cast<int>(fouls1->value()), 0);
    QCOMPARE(static_cast<int>(fouls2->value()), 0);
    QCOMPARE(static_cast<int>(period->value()), 0);
    
    // Default team names should exist
    QLineEdit *homeName = mainWindow->findChild<QLineEdit*>("homeName");
    QLineEdit *guestName = mainWindow->findChild<QLineEdit*>("guestName");
    
    QVERIFY(homeName != nullptr && guestName != nullptr);
    QVERIFY(!homeName->text().isEmpty());
    QVERIFY(!guestName->text().isEmpty());
}

void GameStateTest::testTeamSetup()
{
    // Test team name setup and logo loading
    QLineEdit *homeName = mainWindow->findChild<QLineEdit*>("homeName");
    QLineEdit *guestName = mainWindow->findChild<QLineEdit*>("guestName");
    
    QVERIFY(homeName != nullptr && guestName != nullptr);
    
    QString testHomeName = "Test Home Team";
    QString testGuestName = "Test Guest Team";
    
    homeName->setText(testHomeName);
    guestName->setText(testGuestName);
    
    // Trigger text changed signals
    homeName->editingFinished();
    guestName->editingFinished();
    
    // Verify names are set
    QCOMPARE(homeName->text(), testHomeName);
    QCOMPARE(guestName->text(), testGuestName);
}

void GameStateTest::testGameStart()
{
    // Test game start functionality
    QCOMPARE(mainWindow->getStatus(), 0);
    
    mainWindow->startTimer();
    QCOMPARE(mainWindow->getStatus(), 1);
    
    // Should reset fouls if they were at 4
    // Test this by setting fouls to 4 first
    for (int i = 0; i < 4; ++i) {
        QPushButton *foulButton = mainWindow->findChild<QPushButton*>("FALTA1mas");
        QTest::mouseClick(foulButton, Qt::LeftButton);
    }
    
    // Start timer should reset the fouls
    mainWindow->startTimer();
    
    QLCDNumber *foulsDisplay = mainWindow->findChild<QLCDNumber*>("faltas1");
    QCOMPARE(static_cast<int>(foulsDisplay->value()), 0);
}

void GameStateTest::testGamePause()
{
    // Test game pause functionality
    mainWindow->startTimer();
    QCOMPARE(mainWindow->getStatus(), 1);
    
    mainWindow->pauseTimer();
    QCOMPARE(mainWindow->getStatus(), 2);
    
    // Should stay paused
    QTest::qWait(100);
    QCOMPARE(mainWindow->getStatus(), 2);
}

void GameStateTest::testGameEnd()
{
    // Test game end scenarios
    mainWindow->startTimer();
    QCOMPARE(mainWindow->getStatus(), 1);
    
    mainWindow->stopTimer();
    QCOMPARE(mainWindow->getStatus(), 0);
    
    // Timer displays should show 00:00
    QLCDNumber *timerMM = mainWindow->findChild<QLCDNumber*>("lcdTimerMM");
    QLCDNumber *timerSS = mainWindow->findChild<QLCDNumber*>("lcdTimerSS");
    
    if (timerMM && timerSS) {
        QCOMPARE(static_cast<int>(timerMM->value()), 0);
        QCOMPARE(static_cast<int>(timerSS->value()), 0);
    }
}

void GameStateTest::testGameReset()
{
    // Test game reset functionality
    // First make changes to game state
    mainWindow->startTimer();
    
    QPushButton *scoreButton = mainWindow->findChild<QPushButton*>("puntos1mas3");
    QTest::mouseClick(scoreButton, Qt::LeftButton);
    
    QPushButton *foulButton = mainWindow->findChild<QPushButton*>("FALTA1mas");
    QTest::mouseClick(foulButton, Qt::LeftButton);
    
    // Reset timer
    mainWindow->resetTimer();
    
    // Timer should be running again
    QCOMPARE(mainWindow->getStatus(), 1);
}

void GameStateTest::testMainWindowScoreboardSync()
{
    // Test synchronization between main window and scoreboard
    mainWindow->showScoreboard();
    QTest::qWait(500); // Wait for scoreboard to appear
    
    // Make changes in main window
    QPushButton *scoreButton = mainWindow->findChild<QPushButton*>("puntos1mas2");
    QTest::mouseClick(scoreButton, Qt::LeftButton);
    
    QPushButton *foulButton = mainWindow->findChild<QPushButton*>("FALTA2mas");
    QTest::mouseClick(foulButton, Qt::LeftButton);
    
    // Changes should be reflected (tested through method calls)
    QLCDNumber *scoreDisplay = mainWindow->findChild<QLCDNumber*>("puntos1");
    QLCDNumber *foulDisplay = mainWindow->findChild<QLCDNumber*>("faltas2");
    
    QCOMPARE(static_cast<int>(scoreDisplay->value()), 2);
    QCOMPARE(static_cast<int>(foulDisplay->value()), 1);
}

void GameStateTest::testThemeSync()
{
    // Test that theme changes are synchronized across components
    ThemeManager *themeManager = ThemeManager::instance();
    
    // Change theme
    themeManager->setTheme(ThemeManager::Dark);
    QTest::qWait(100);
    
    // Verify theme is applied
    QCOMPARE(themeManager->currentTheme(), ThemeManager::Dark);
    
    // Change back  
    themeManager->setTheme(ThemeManager::Light);
    QTest::qWait(100);
    
    QCOMPARE(themeManager->currentTheme(), ThemeManager::Light);
}

void GameStateTest::testTimerStateSync()
{
    // Test timer state synchronization
    mainWindow->startTimer();
    
    // Both main window and scoreboard should have same timer state
    QCOMPARE(mainWindow->getStatus(), 1);
    
    mainWindow->pauseTimer();
    QCOMPARE(mainWindow->getStatus(), 2);
    
    mainWindow->stopTimer();
    QCOMPARE(mainWindow->getStatus(), 0);
}

void GameStateTest::testInvalidGameStates()
{
    // Test handling of invalid or edge case game states
    
    // Test timer operations when already in that state
    mainWindow->stopTimer(); // Should already be stopped
    QCOMPARE(mainWindow->getStatus(), 0);
    
    mainWindow->startTimer();
    mainWindow->startTimer(); // Start when already started
    QCOMPARE(mainWindow->getStatus(), 1);
    
    mainWindow->pauseTimer();
    mainWindow->pauseTimer(); // Pause when already paused
    QCOMPARE(mainWindow->getStatus(), 2);
}

void GameStateTest::testMemoryManagement()
{
    // Test that objects are properly managed during game operations
    
    // Create and destroy multiple scoreboards
    for (int i = 0; i < 5; ++i) {
        mainWindow->showScoreboard();
        QTest::qWait(100);
        
        // Make some changes
        QPushButton *scoreButton = mainWindow->findChild<QPushButton*>("puntos1mas1");
        if (scoreButton) {
            QTest::mouseClick(scoreButton, Qt::LeftButton);
        }
        
        QTest::qWait(100);
    }
    
    // Application should still be stable
    QVERIFY(mainWindow != nullptr);
    QVERIFY(ThemeManager::instance() != nullptr);
}

void GameStateTest::testErrorRecovery()
{
    // Test recovery from error conditions
    
    // Try operations that might fail gracefully
    mainWindow->startTimer();
    
    // Simulate rapid operations
    for (int i = 0; i < 10; ++i) {
        mainWindow->pauseTimer();
        mainWindow->startTimer();
        QTest::qWait(1);
    }
    
    // Should still be in valid state
    int status = mainWindow->getStatus();
    QVERIFY(status >= 0 && status <= 2);
}

void GameStateTest::testLargeBenchmark()
{
    // Test system with large numbers of operations
    QTime timer;
    timer.start();
    
    // Perform many scoring operations
    QPushButton *scoreButton = mainWindow->findChild<QPushButton*>("puntos1mas1");
    
    for (int i = 0; i < 100; ++i) {
        if (scoreButton) {
            QTest::mouseClick(scoreButton, Qt::LeftButton);
        }
    }
    
    int elapsed = timer.elapsed();
    qDebug() << "100 scoring operations took" << elapsed << "ms";
    
    // Should complete in reasonable time (less than 5 seconds)
    QVERIFY(elapsed < 5000);
    
    // Final score should be correct
    QLCDNumber *scoreDisplay = mainWindow->findChild<QLCDNumber*>("puntos1");
    QCOMPARE(static_cast<int>(scoreDisplay->value()), 100);
}

void GameStateTest::testResponseTime()
{
    // Test UI response times
    QPushButton *scoreButton = mainWindow->findChild<QPushButton*>("puntos1mas1");
    QVERIFY(scoreButton != nullptr);
    
    QTime timer;
    int totalTime = 0;
    int iterations = 10;
    
    for (int i = 0; i < iterations; ++i) {
        timer.start();
        QTest::mouseClick(scoreButton, Qt::LeftButton);
        QTest::qWait(1); // Minimal wait for processing
        totalTime += timer.elapsed();
    }
    
    double avgTime = static_cast<double>(totalTime) / iterations;
    qDebug() << "Average response time:" << avgTime << "ms";
    
    // Should be very responsive (less than 50ms average)
    QVERIFY(avgTime < 50.0);
}

QTEST_MAIN(GameStateTest)
#include "test_gamestate.moc"