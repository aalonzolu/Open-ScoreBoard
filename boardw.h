#ifndef BOARDW_H
#define BOARDW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QResizeEvent>

namespace Ui {
class BoardW;
}

class BoardW : public QMainWindow
{
    Q_OBJECT

public:
    explicit BoardW(QWidget *parent = 0);
    ~BoardW();
    QString IMG0;
    QString IMG1;
    void SetPixmap(int team, QString img);
    int STATUS;

public slots:
    void setLCD(int minute,int second);
    void SHOW();
    void resizeEvent(QResizeEvent* event);
    void changename(QString name);
    void teamname(QString team1, QString team2);
    void balonPOS(int team);
    void keyPressEvent(QKeyEvent *event);
    int updatePuntos(int PUNTAJE1,int PUNTAJE2);
    int updateFaltas(int FaLTAS1, int FaLTAS2);
    int updatePeriodo(int PERIODOT);
    int updateBonus(int TEAM);
private:
    Ui::BoardW *ui;
};

#endif // BOARDW_H
