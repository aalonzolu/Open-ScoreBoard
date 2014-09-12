#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QSound>
#include "boardw.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer* timer;
    QTime* TIME;
    QTime*  timeValue;
    BoardW* scoreBoard;
    int STATUS;
    QString IMG0;
    QString IMG1;
    int PUNTOS1, PUNTOS2,FALTAS1,FALTAS2,BONUS,POS,PERIODO;
public slots:
    void showScoreboard();
    void setDisplay();
    int countdown();
    void countdown2();
    void startTimer();
    void pauseTimer();
    void stopTimer();
    void resetTimer();
    void setPixmap1();
    void setPixmap2();
    void resizeEvent(QResizeEvent* event);
    void changename();
    void teamname();
    void keyPressEvent( QKeyEvent * event );
    void DisableTitle();
    int getStatus();
    int Punteo();
    int Faltas();
    int Periodo();
    int Bonus(int TEAM);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
