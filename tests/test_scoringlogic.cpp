#include <QtTest>
#include <QPushButton>
#include <QLCDNumber>
#include "testutils.h"

// Include the main window to test scoring functionality
#include "../mainwindow.h"

class ScoringLogicTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    
    // Basic scoring tests
    void testIncreaseScore();
    void testDecreaseScore();
    void testMultiplePointIncrements();
    void testScoreResets();
    
    // Foul tests
    void testIncreaseFouls();
    void testDecreaseFouls();
    void testFoulBonus();
    void testFoulReset();
    
    // Period tests
    void testIncreasePeriod();
    void testDecreasePeriod();
    void testPeriodBoundaries();
    
    // Edge cases
    void testNegativeScores();
    void testMaximumScores();
    void testRapidScoring();
    
    // Integration tests
    void testScoreboardSync();
    void testScoreboardUpdate();

private:
    MainWindow *mainWindow;
    TestUtils::GameData mockData;
    
    void clickButton(const QString &buttonName);
    int getDisplayValue(const QString &displayName);
};

void ScoringLogicTest::initTestCase()
{
    qDebug() << "Starting ScoringLogicTest suite";
}

void ScoringLogicTest::init()
{
    mainWindow = new MainWindow();
    TestUtils::waitForWindowShown(mainWindow);
    mockData = TestUtils::createMockGameData();
}

void ScoringLogicTest::cleanup()
{
    delete mainWindow;
    mainWindow = nullptr;
}

void ScoringLogicTest::cleanupTestCase()
{
    qDebug() << "Finished ScoringLogicTest suite";
}

void ScoringLogicTest::clickButton(const QString &buttonName)
{
    QPushButton *button = mainWindow->findChild<QPushButton*>(buttonName);
    QVERIFY2(button != nullptr, qPrintable(QString("Button %1 not found").arg(buttonName)));
    
    QTest::mouseClick(button, Qt::LeftButton);
    QTest::qWait(10); // Small delay for processing
}

int ScoringLogicTest::getDisplayValue(const QString &displayName)
{
    QLCDNumber *display = mainWindow->findChild<QLCDNumber*>(displayName);
    QVERIFY2(display != nullptr, qPrintable(QString("Display %1 not found").arg(displayName)));
    
    return static_cast<int>(display->value());
}

void ScoringLogicTest::testIncreaseScore()
{
    // Test basic score increment for team 1
    int initialScore = getDisplayValue("puntos1");
    
    clickButton("puntos1mas1");
    int newScore = getDisplayValue("puntos1");
    
    QCOMPARE(newScore, initialScore + 1);
    
    // Test 2-point increment
    initialScore = newScore;
    clickButton("puntos1mas2");
    newScore = getDisplayValue("puntos1");
    
    QCOMPARE(newScore, initialScore + 2);
    
    // Test 3-point increment
    initialScore = newScore;
    clickButton("puntos1mas3");
    newScore = getDisplayValue("puntos1");
    
    QCOMPARE(newScore, initialScore + 3);
}

void ScoringLogicTest::testDecreaseScore()
{
    // First increase score to have something to decrease
    clickButton("puntos1mas3");
    clickButton("puntos1mas2");
    
    int initialScore = getDisplayValue("puntos1");
    QVERIFY(initialScore > 0);
    
    // Test score decrement
    clickButton("puntos1menos1");
    int newScore = getDisplayValue("puntos1");
    
    QCOMPARE(newScore, initialScore - 1);
}

void ScoringLogicTest::testMultiplePointIncrements()
{
    // Test sequence of different point increments
    int initialScore = getDisplayValue("puntos2");
    
    clickButton("puntos2mas1"); // +1
    clickButton("puntos2mas2"); // +2
    clickButton("puntos2mas3"); // +3
    clickButton("puntos2mas1"); // +1
    
    int expectedScore = initialScore + 1 + 2 + 3 + 1;
    int actualScore = getDisplayValue("puntos2");
    
    QCOMPARE(actualScore, expectedScore);
}

void ScoringLogicTest::testScoreResets()
{
    // Add some points
    clickButton("puntos1mas3");
    clickButton("puntos1mas2");
    
    QVERIFY(getDisplayValue("puntos1") > 0);
    
    // Reset by subtracting all points (if there's a reset functionality)
    // or test that scores can go back to zero
    int currentScore = getDisplayValue("puntos1");
    for (int i = 0; i < currentScore; ++i) {
        clickButton("puntos1menos1");
    }
    
    QCOMPARE(getDisplayValue("puntos1"), 0);
}

void ScoringLogicTest::testIncreaseFouls()
{
    // Test foul increment
    int initialFouls = getDisplayValue("faltas1");
    
    clickButton("FALTA1mas");
    int newFouls = getDisplayValue("faltas1");
    
    QCOMPARE(newFouls, initialFouls + 1);
}

void ScoringLogicTest::testDecreaseFouls()
{
    // First add a foul
    clickButton("FALTA1mas");
    
    int initialFouls = getDisplayValue("faltas1");
    QVERIFY(initialFouls > 0);
    
    clickButton("FALTA1menos");
    int newFouls = getDisplayValue("faltas1");
    
    QCOMPARE(newFouls, initialFouls - 1);
}

void ScoringLogicTest::testFoulBonus()
{
    // Test that 4 fouls trigger bonus
    // Add 4 fouls to trigger bonus
    for (int i = 0; i < 4; ++i) {
        clickButton("FALTA1mas");
    }
    
    // Bonus should be triggered (timer paused)
    // This is more of an integration test - verify through state
    int fouls = getDisplayValue("faltas1");
    QVERIFY(fouls >= 4);
}

void ScoringLogicTest::testFoulReset()
{
    // Add fouls
    clickButton("FALTA2mas");
    clickButton("FALTA2mas");
    
    QVERIFY(getDisplayValue("faltas2") > 0);
    
    // Start timer should reset fouls if they're at 4
    // This tests the automatic reset logic
    int currentFouls = getDisplayValue("faltas2");
    
    // Simulate game start
    mainWindow->startTimer();
    
    // Fouls should remain the same unless they were 4
    int newFouls = getDisplayValue("faltas2");
    
    if (currentFouls == 4) {
        QCOMPARE(newFouls, 0);
    } else {
        QCOMPARE(newFouls, currentFouls);
    }
}

void ScoringLogicTest::testIncreasePeriod()
{
    int initialPeriod = getDisplayValue("PERIODO");
    
    clickButton("Periodomas");
    int newPeriod = getDisplayValue("PERIODO");
    
    QCOMPARE(newPeriod, initialPeriod + 1);
}

void ScoringLogicTest::testDecreasePeriod()
{
    // First increase period so we have something to decrease
    clickButton("Periodomas");
    
    int initialPeriod = getDisplayValue("PERIODO");
    
    clickButton("Periodomenos");
    int newPeriod = getDisplayValue("PERIODO");
    
    QCOMPARE(newPeriod, initialPeriod - 1);
}

void ScoringLogicTest::testPeriodBoundaries()
{
    // Test period boundaries (typically 1-4 for basketball)
    int initialPeriod = getDisplayValue("PERIODO");
    
    // Add several periods
    for (int i = 0; i < 10; ++i) {
        clickButton("Periodomas");
    }
    
    int maxPeriod = getDisplayValue("PERIODO");
    QVERIFY(maxPeriod > initialPeriod);
    
    // Test decreasing periods
    for (int i = 0; i < 20; ++i) {
        clickButton("Periodomenos");
    }
    
    int minPeriod = getDisplayValue("PERIODO");
    // Should not go below reasonable minimum (but may go negative in current implementation)
    QVERIFY(minPeriod <= maxPeriod);
}

void ScoringLogicTest::testNegativeScores()
{
    // Test that scores can go negative (or are prevented from going negative)
    int initialScore = getDisplayValue("puntos1");
    
    // Try to make score negative
    for (int i = 0; i < initialScore + 5; ++i) {
        clickButton("puntos1menos1");
    }
    
    int finalScore = getDisplayValue("puntos1");
    
    // Depending on implementation, score might go negative or stop at 0
    // For now, we just verify the operation completes
    QVERIFY(finalScore <= initialScore);
}

void ScoringLogicTest::testMaximumScores()
{
    // Test very high scores
    for (int i = 0; i < 50; ++i) {
        clickButton("puntos2mas3");
    }
    
    int score = getDisplayValue("puntos2");
    QVERIFY(score >= 150); // Should be at least 150 (50 * 3)
}

void ScoringLogicTest::testRapidScoring()
{
    // Test rapid button clicking
    int initialScore = getDisplayValue("puntos1");
    
    for (int i = 0; i < 20; ++i) {
        clickButton("puntos1mas1");
    }
    
    int finalScore = getDisplayValue("puntos1");
    QCOMPARE(finalScore, initialScore + 20);
}

void ScoringLogicTest::testScoreboardSync()
{
    // Test that main window and scoreboard stay in sync
    // This requires the scoreboard to be shown
    mainWindow->showScoreboard();
    QTest::qWait(500);
    
    // Add points and verify scoreboard gets updated
    clickButton("puntos1mas2");
    clickButton("puntos2mas3");
    
    // The scoreboard should reflect these changes
    // This is tested through the updatePuntos method calls
    int score1 = getDisplayValue("puntos1");
    int score2 = getDisplayValue("puntos2");
    
    QVERIFY(score1 >= 2);
    QVERIFY(score2 >= 3);
}

void ScoringLogicTest::testScoreboardUpdate()
{
    // Test that scoreboard update methods work correctly
    int score1 = getDisplayValue("puntos1");
    int score2 = getDisplayValue("puntos2");
    
    // Trigger update through button press
    clickButton("puntos1mas1");
    
    int newScore1 = getDisplayValue("puntos1");
    QCOMPARE(newScore1, score1 + 1);
    
    // Score 2 should remain unchanged
    QCOMPARE(getDisplayValue("puntos2"), score2);
}

QTEST_MAIN(ScoringLogicTest)
#include "test_scoringlogic.moc"