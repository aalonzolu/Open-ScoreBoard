#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <QObject>
#include <QTest>
#include <QApplication>
#include <QWidget>

class TestUtils : public QObject
{
    Q_OBJECT
    
public:
    explicit TestUtils(QObject *parent = nullptr);
    
    // Utility functions for tests
    static void waitForWindowShown(QWidget *widget);
    static void simulateKeySequence(QWidget *widget, const QString &keys);
    static void simulateMouseClick(QWidget *widget, Qt::MouseButton button = Qt::LeftButton);
    static bool compareImages(const QString &expected, const QString &actual, double threshold = 0.95);
    static QString generateTestData(const QString &type, int count = 10);
    
    // Test data helpers
    static QList<int> generateScoreSequence(int maxScore = 100);
    static QStringList generateTeamNames(int count = 2);
    
    // Timing utilities
    static void waitMs(int milliseconds);
    static bool waitForSignal(QObject *sender, const char *signal, int timeout = 5000);
    
    // Mock data
    struct GameData {
        QString homeTeam;
        QString awayTeam;
        int homeScore;
        int awayScore;
        int homeFouls;
        int awayFouls;
        int period;
        int timeMinutes;
        int timeSeconds;
    };
    
    static GameData createMockGameData();
};

#endif // TESTUTILS_H