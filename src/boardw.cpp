#include "boardw.h"
#include "ui_boardw.h"
#include <QScreen>
#include <QGuiApplication>

BoardW::BoardW(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BoardW)
{
    ui->setupUi(this);
    
    // Apply current theme
    applyCurrentTheme();
    
    // Connect to theme changes
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &BoardW::applyCurrentTheme);
}

BoardW::~BoardW()
{
    delete ui;
}

void BoardW::setLCD(int minute,int second)
{
    // Update timer displays on scoreboard
    ui->lcdTimerMM->display(minute);
    ui->lcdTimerSS->display(second);
}

void BoardW::SetPixmap(int team,QString img)
{
    int w = ui->LOGO1->width();
    int h = ui->LOGO1->height();
    if (team==1)
    {
        IMG0 = img;
        QPixmap mypix (img);
        ui->LOGO1->setPixmap(mypix.scaled(w,h,Qt::KeepAspectRatio));
    }
    else
    {
        IMG1 = img;
        QPixmap mypix (img);
        ui->LOGO2->setPixmap(mypix.scaled(w,h,Qt::KeepAspectRatio));
    }


}

void BoardW::SHOW()
{
    QList<QScreen*> screens = QGuiApplication::screens();
    if (screens.count() == 1) {
        // single monitor - use built in
        showFullScreen();
    } else {
        // Multi-monitor setup - show on second screen
        if (screens.count() > 1) {
            QRect rect = screens.at(1)->geometry();
            move(rect.topLeft());
            setWindowState(Qt::WindowFullScreen);
            this->resize(rect.size());
        } else {
            showFullScreen();
        }
    }
}

void BoardW::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   
   // Update logo scaling after resize event
   int w = ui->LOGO1->width();
   int h = ui->LOGO1->height();
   
   QPixmap mypix0 (IMG0);
   ui->LOGO1->setPixmap(mypix0.scaled(w,h,Qt::KeepAspectRatio));
   QPixmap mypix1 (IMG1);
   ui->LOGO2->setPixmap(mypix1.scaled(w,h,Qt::KeepAspectRatio));
}

void BoardW::changename(QString name)
{
    ui->TITLE->setText(name);
}
void BoardW::teamname(QString team1, QString team2)
{
    ui->homeName->setText(team1);
    ui->guestName->setText(team2);
}

void BoardW::balonPOS(int team)
{
    //change de b pos
    if(team==1)
    {
        QPixmap mypix0 (":/images/images/balon.png");
        ui->pos1->setPixmap(mypix0);
        QPixmap mypix1 (":/images/images/balon-gray.png");
        ui->pos2->setPixmap(mypix1);
    }
    else if(team==2)
    {
        QPixmap mypix0 (":/images/images/balon-gray.png");
        ui->pos1->setPixmap(mypix0);
        QPixmap mypix1 (":/images/images/balon.png");
        ui->pos2->setPixmap(mypix1);
    }
}
void BoardW::keyPressEvent(QKeyEvent *event)
{
    // Handle keyboard input on scoreboard
    // Most game control happens in main window, this is for display-specific actions
    
    if( event->key() == Qt::Key_A )
    {
        // Reserved for future team 1 display actions
    }
    else if( event->key() == Qt::Key_D )
    {
        // Reserved for future team 1 display actions
    }
    else if( event->key() == Qt::Key_W )
    {
        // Reserved for future team 1 display actions
    }
    else if( event->key() == Qt::Key_S )
    {
        // Reserved for future team 1 display actions
    }
    else if( event->key() == Qt::Key_J )
    {
        // Reserved for future team 2 display actions
    }
        else if( event->key() == Qt::Key_1 )
        {
            // Set ball possession to Team 1 on scoreboard
            QPixmap mypix0 (":/images/images/balon.png");
            ui->pos1->setPixmap(mypix0);
            QPixmap mypix1 (":/images/images/balon-gray.png");
            ui->pos2->setPixmap(mypix1);
        }
        else if( event->key() == Qt::Key_2 )
        {
            // Set ball possession to Team 2 on scoreboard
            QPixmap mypix0 (":/images/images/balon-gray.png");
            ui->pos1->setPixmap(mypix0);
            QPixmap mypix1 (":/images/images/balon.png");
            ui->pos2->setPixmap(mypix1);
        }
        else
        {
            // Pass unhandled keys to parent
            QMainWindow::keyPressEvent(event);
        }
    }
int BoardW::updatePuntos(int PUNTAJE1, int PUNTAJE2)
{
    //cambiar puntaje en la segunta pantalla
    ui->PUNTOS1->display(PUNTAJE1);
    ui->PUNTOS2->display(PUNTAJE2);
    return 0;
}
int BoardW::updateFaltas(int FaLTAS1, int FaLTAS2)
{
    //update faltas
    ui->FALTAS1->display(FaLTAS1);
    ui->FALTAS2->display(FaLTAS2);
    return 0;
}
int BoardW::updatePeriodo(int PERIODOT)
{
    //update periodo
    ui->PERIODO->display(PERIODOT);
    return 0;
}
int BoardW::updateBonus(int TEAM)
{
    //update bonus
    //bonus
    QPixmap mypix1 (":/images/images/yellow.png");
    QPixmap mypix2 (":/images/images/gray.png");
    switch(TEAM)
    {
    case 1:
        ui->bonus2->setPixmap(mypix1);
        ui->bonus1->setPixmap(mypix2);
        break;
    case 2:
        ui->bonus1->setPixmap(mypix1);
        ui->bonus2->setPixmap(mypix2);
        break;
    case 0:
        ui->bonus1->setPixmap(mypix2);
        ui->bonus2->setPixmap(mypix2);
    }
    return 0;
}

void BoardW::applyCurrentTheme()
{
    // Apply current theme from ThemeManager
    ThemeManager::instance()->applyTheme();
    
    // Update window title color based on theme
    if (ThemeManager::instance()->currentTheme() == ThemeManager::Dark) {
        setWindowTitle("Open ScoreBoard - Dark Mode");
    } else {
        setWindowTitle("Open ScoreBoard");
    }
}