#include "testutils.h"
#include <QTimer>
#include <QEventLoop>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QPixmap>
#include <QImage>

TestUtils::TestUtils(QObject *parent)
    : QObject(parent)
{
}

void TestUtils::waitForWindowShown(QWidget *widget)
{
    if (!widget) return;
    
    widget->show();
    QTest::qWaitForWindowExposed(widget);
    QTest::qWait(100); // Give some time for rendering
}

void TestUtils::simulateKeySequence(QWidget *widget, const QString &keys)
{
    if (!widget) return;
    
    for (const QChar &ch : keys) {
        QTest::keyClick(widget, ch.toLatin1());
        QTest::qWait(10);
    }
}

void TestUtils::simulateMouseClick(QWidget *widget, Qt::MouseButton button)
{
    if (!widget) return;
    
    QTest::mouseClick(widget, button, Qt::NoModifier, widget->rect().center());
}

bool TestUtils::compareImages(const QString &expected, const QString &actual, double threshold)
{
    QImage img1(expected);
    QImage img2(actual);
    
    if (img1.size() != img2.size()) return false;
    
    int totalPixels = img1.width() * img1.height();
    int matchingPixels = 0;
    
    for (int y = 0; y < img1.height(); ++y) {
        for (int x = 0; x < img1.width(); ++x) {
            if (img1.pixel(x, y) == img2.pixel(x, y)) {
                matchingPixels++;
            }
        }
    }
    
    double similarity = static_cast<double>(matchingPixels) / totalPixels;
    return similarity >= threshold;
}

QString TestUtils::generateTestData(const QString &type, int count)
{
    QStringList data;
    
    if (type == "teams") {
        QStringList teamNames = {"Lakers", "Bulls", "Celtics", "Warriors", "Heat", "Spurs"};
        for (int i = 0; i < count; ++i) {
            data << teamNames.at(QRandomGenerator::global()->bounded(teamNames.size()));
        }
    } else if (type == "scores") {
        for (int i = 0; i < count; ++i) {
            data << QString::number(QRandomGenerator::global()->bounded(1, 101));
        }
    }
    
    return data.join(",");
}

QList<int> TestUtils::generateScoreSequence(int maxScore)
{
    QList<int> sequence;
    int currentScore = 0;
    
    while (currentScore < maxScore) {
        int increment = QRandomGenerator::global()->bounded(1, 4); // 1, 2, or 3 points
        currentScore += increment;
        sequence << currentScore;
    }
    
    return sequence;
}

QStringList TestUtils::generateTeamNames(int count)
{
    QStringList names = {"Lakers", "Bulls", "Celtics", "Warriors", "Heat", "Spurs", 
                        "Nets", "Knicks", "76ers", "Bucks", "Raptors", "Magic"};
    
    QStringList result;
    for (int i = 0; i < count; ++i) {
        result << names.at(i % names.size());
    }
    
    return result;
}

void TestUtils::waitMs(int milliseconds)
{
    QTest::qWait(milliseconds);
}

bool TestUtils::waitForSignal(QObject *sender, const char *signal, int timeout)
{
    if (!sender) return false;
    
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    bool signalReceived = false;
    
    QObject::connect(sender, signal, &loop, [&loop, &signalReceived]() {
        signalReceived = true;
        loop.quit();
    });
    
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    
    timer.start(timeout);
    loop.exec();
    
    return signalReceived;
}

TestUtils::GameData TestUtils::createMockGameData()
{
    GameData data;
    data.homeTeam = "Home Team";
    data.awayTeam = "Away Team";
    data.homeScore = QRandomGenerator::global()->bounded(50, 101);
    data.awayScore = QRandomGenerator::global()->bounded(50, 101);
    data.homeFouls = QRandomGenerator::global()->bounded(0, 6);
    data.awayFouls = QRandomGenerator::global()->bounded(0, 6);
    data.period = QRandomGenerator::global()->bounded(1, 5);
    data.timeMinutes = QRandomGenerator::global()->bounded(0, 13);
    data.timeSeconds = QRandomGenerator::global()->bounded(0, 60);
    
    return data;
}