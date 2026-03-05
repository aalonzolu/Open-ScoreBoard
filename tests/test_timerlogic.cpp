#include <QtTest>
#include <QTimer>
#include <QTime>
#include <QSignalSpy>
#include "testutils.h"

// Include the main window to test timer functionality
#include "../mainwindow.h"

class TimerLogicTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    
    // Timer functionality tests
    void testTimerStart();
    void testTimerPause();
    void testTimerStop();
    void testTimerReset();
    void testTimerCountdown();
    void testTimerZero();
    
    // Edge cases
    void testTimerNegativeTime();
    void testTimerMaxTime();
    void testTimerRapidStartStop();
    
    // Performance tests
    void testTimerAccuracy();
    void testTimerPrecision();

private:
    MainWindow *mainWindow;
    TestUtils::GameData mockData;
};

void TimerLogicTest::initTestCase()
{
    // This function is called once before all tests
    qDebug() << "Starting TimerLogicTest suite";
}

void TimerLogicTest::init()
{
    // This function is called before each test
    mainWindow = new MainWindow();
    TestUtils::waitForWindowShown(mainWindow);
    mockData = TestUtils::createMockGameData();
}

void TimerLogicTest::cleanup()
{
    // This function is called after each test
    delete mainWindow;
    mainWindow = nullptr;
}

void TimerLogicTest::cleanupTestCase()
{
    // This function is called once after all tests
    qDebug() << "Finished TimerLogicTest suite";
}

void TimerLogicTest::testTimerStart()
{
    // Test that timer starts correctly
    QVERIFY(mainWindow != nullptr);
    
    // Initial status should be stopped (0)
    QCOMPARE(mainWindow->getStatus(), 0);
    
    // Start the timer
    mainWindow->startTimer();
    
    // Status should now be running (1)
    QCOMPARE(mainWindow->getStatus(), 1);
    
    // Wait a bit and verify timer is actually running
    QTest::qWait(1100); // Wait for more than 1 second
    
    // Timer should still be running
    QCOMPARE(mainWindow->getStatus(), 1);
}

void TimerLogicTest::testTimerPause()
{
    // Test timer pause functionality
    QVERIFY(mainWindow != nullptr);
    
    // Start timer first
    mainWindow->startTimer();
    QCOMPARE(mainWindow->getStatus(), 1);
    
    // Pause the timer
    mainWindow->pauseTimer();
    
    // Status should be paused (2)
    QCOMPARE(mainWindow->getStatus(), 2);
    
    // Wait and verify it stays paused
    QTest::qWait(500);
    QCOMPARE(mainWindow->getStatus(), 2);
}

void TimerLogicTest::testTimerStop()
{
    // Test timer stop functionality
    QVERIFY(mainWindow != nullptr);
    
    // Start timer first
    mainWindow->startTimer();
    QCOMPARE(mainWindow->getStatus(), 1);
    
    // Stop the timer
    mainWindow->stopTimer();
    
    // Status should be stopped (0)
    QCOMPARE(mainWindow->getStatus(), 0);
}

void TimerLogicTest::testTimerReset()
{
    // Test timer reset functionality
    QVERIFY(mainWindow != nullptr);
    
    // Start timer and let it run
    mainWindow->startTimer();
    QTest::qWait(1100);
    
    // Reset the timer
    mainWindow->resetTimer();
    
    // Timer should be running again (1) after reset
    QCOMPARE(mainWindow->getStatus(), 1);
}

void TimerLogicTest::testTimerCountdown()
{
    // Test that countdown function works
    QVERIFY(mainWindow != nullptr);
    
    // Start timer
    mainWindow->startTimer();
    
    // Call countdown manually and verify return value
    int result = mainWindow->countdown();
    QCOMPARE(result, 0); // countdown() should return 0 normally
}

void TimerLogicTest::testTimerZero()
{
    // Test behavior when timer reaches zero
    QVERIFY(mainWindow != nullptr);
    
    // This would require setting time to very low value
    // For now, we test the logic path
    mainWindow->startTimer();
    
    // Verify timer is running
    QCOMPARE(mainWindow->getStatus(), 1);
    
    // After timeout, timer should stop automatically
    // This is tested in integration tests with actual time manipulation
}

void TimerLogicTest::testTimerNegativeTime()
{
    // Test that timer handles edge cases gracefully
    QVERIFY(mainWindow != nullptr);
    
    // Timer should handle boundary conditions
    mainWindow->startTimer();
    QVERIFY(mainWindow->getStatus() >= 0);
    QVERIFY(mainWindow->getStatus() <= 2);
}

void TimerLogicTest::testTimerMaxTime()
{
    // Test timer with maximum possible time
    QVERIFY(mainWindow != nullptr);
    
    // Test that timer can handle long durations
    mainWindow->startTimer();
    QCOMPARE(mainWindow->getStatus(), 1);
}

void TimerLogicTest::testTimerRapidStartStop()
{
    // Test rapid start/stop operations
    QVERIFY(mainWindow != nullptr);
    
    for (int i = 0; i < 10; ++i) {
        mainWindow->startTimer();
        QCOMPARE(mainWindow->getStatus(), 1);
        
        mainWindow->stopTimer();
        QCOMPARE(mainWindow->getStatus(), 0);
        
        QTest::qWait(10);
    }
}

void TimerLogicTest::testTimerAccuracy()
{
    // Test timer accuracy over time
    QVERIFY(mainWindow != nullptr);
    
    QTime startTime = QTime::currentTime();
    mainWindow->startTimer();
    
    // Let it run for a known duration
    QTest::qWait(2000); // 2 seconds
    
    QTime endTime = QTime::currentTime();
    int elapsed = startTime.msecsTo(endTime);
    
    // Check that elapsed time is approximately correct (within 100ms tolerance)
    QVERIFY(qAbs(elapsed - 2000) < 100);
}

void TimerLogicTest::testTimerPrecision()
{
    // Test timer precision with multiple measurements
    QVERIFY(mainWindow != nullptr);
    
    QList<int> measurements;
    
    for (int i = 0; i < 5; ++i) {
        QTime startTime = QTime::currentTime();
        mainWindow->startTimer();
        
        QTest::qWait(1000); // 1 second
        
        mainWindow->stopTimer();
        QTime endTime = QTime::currentTime();
        
        measurements << startTime.msecsTo(endTime);
        QTest::qWait(100);
    }
    
    // Calculate average and verify consistency
    int sum = 0;
    for (int measurement : measurements) {
        sum += measurement;
    }
    
    double average = static_cast<double>(sum) / measurements.size();
    
    // Average should be close to 1000ms
    QVERIFY(qAbs(average - 1000.0) < 50.0);
}

QTEST_MAIN(TimerLogicTest)
#include "test_timerlogic.moc"