#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scoreBoard = new BoardW();
    connect(ui->showButton,SIGNAL(clicked()),this,SLOT(showScoreboard()));
    connect(ui->actionSalir,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->timerPlay,SIGNAL(clicked()),this,SLOT(startTimer()));
    connect(ui->timerPause,SIGNAL(clicked()),this,SLOT(pauseTimer()));
    connect(ui->timerStop,SIGNAL(clicked()),this,SLOT(stopTimer()));
    connect(ui->timerRestart,SIGNAL(clicked()),this,SLOT(resetTimer()));
    connect(ui->LOGO1BUTTON,SIGNAL(clicked()),this,SLOT(setPixmap1()));
    connect(ui->LOGO2BUTTON,SIGNAL(clicked()),this,SLOT(setPixmap2()));

    connect(ui->TitleListoButton,SIGNAL(clicked()),this,SLOT(DisableTitle()));

    connect(ui->TITLE,SIGNAL(textEdited(QString)),this,SLOT(changename()));
    connect(ui->homeName,SIGNAL(textEdited(QString)),this,SLOT(teamname()));
    connect(ui->guestName,SIGNAL(textEdited(QString)),this,SLOT(teamname()));

    connect(ui->puntos1mas1,SIGNAL(clicked()),this,SLOT(Punteo()));
    connect(ui->puntos1mas2,SIGNAL(clicked()),this,SLOT(Punteo()));
    connect(ui->puntos1mas3,SIGNAL(clicked()),this,SLOT(Punteo()));
    connect(ui->puntos1menos1,SIGNAL(clicked()),this,SLOT(Punteo()));

    connect(ui->puntos2mas1,SIGNAL(clicked()),this,SLOT(Punteo()));
    connect(ui->puntos2mas2,SIGNAL(clicked()),this,SLOT(Punteo()));
    connect(ui->puntos2mas3,SIGNAL(clicked()),this,SLOT(Punteo()));
    connect(ui->puntos2menos1,SIGNAL(clicked()),this,SLOT(Punteo()));

    connect(ui->FALTA1mas,SIGNAL(clicked()),this,SLOT(Faltas()));
    connect(ui->FALTA1menos,SIGNAL(clicked()),this,SLOT(Faltas()));
    connect(ui->FALTA2mas,SIGNAL(clicked()),this,SLOT(Faltas()));
    connect(ui->FALTA2menos,SIGNAL(clicked()),this,SLOT(Faltas()));

    connect(ui->Periodomas,SIGNAL(clicked()),this,SLOT(Periodo()));
    connect(ui->Periodomenos,SIGNAL(clicked()),this,SLOT(Periodo()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(countdown()));
    timer->start(1000);

    TIME = new QTime();
    TIME->start();
    TIME->setHMS(ui->timeEdit->time().hour(), ui->timeEdit->time().minute(), ui->timeEdit->time().second());

    PUNTOS1 =0;PUNTOS2 =0;FALTAS1 =0;FALTAS2 =0;BONUS =0;POS =0;PERIODO =0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDisplay()
    {

    }

int MainWindow::countdown()
{
    if(STATUS==1)
    {
        if (TIME->minute() <=0 && TIME->second() <=0)
        {
            qDebug()<<"Suena la chicharra!";
            //   :/sounds/sounds/Brazzer.wav
            QSound::play(":/sounds/sounds/Brazzer.wav");
            stopTimer();
            return 0;
        }
            TIME->setHMS(0,TIME->addSecs(-1).minute(),TIME->addSecs(-1).second());
            qDebug()<<TIME->toString();
            ui->lcdTimerMM->display(this->TIME->minute());
            ui->lcdTimerSS->display(this->TIME->second());
            scoreBoard->setLCD(this->TIME->minute(),this->TIME->second());
    }
    else if (STATUS==2)
    {
        //do nothing
    }
    else if (STATUS==0)
    {
        //do nothing
    }
    return 0;
}


void MainWindow::countdown2()
{

}

void MainWindow::showScoreboard()
{
            scoreBoard->SHOW();
            ui->TITLE->setDisabled(true);
}

void MainWindow::startTimer()
{
    QPixmap mypix (":/images/images/gray.png");
    ui->bonus1->setPixmap(mypix);
    ui->bonus2->setPixmap(mypix);
    scoreBoard->updateBonus(0);
    if(FALTAS1==4)
    {
        FALTAS1=0;
        ui->faltas1->display(FALTAS1);
    }
    if(FALTAS2==4)
    {
        FALTAS2=0;
        ui->faltas2->display(FALTAS2);
    }
    scoreBoard->updateFaltas(FALTAS1,FALTAS2);
    if(STATUS!=2)
    {
        TIME->setHMS(ui->timeEdit->time().hour(), ui->timeEdit->time().minute(), ui->timeEdit->time().second());
    }
    STATUS = 1;
    scoreBoard->STATUS = 1;
    qDebug()<<"Timer started";

}
void MainWindow::pauseTimer()
{
    STATUS = 2;
    scoreBoard->STATUS = 2;
    //Just Pause the timer
}

void MainWindow::stopTimer()
{
    //stop the timer and set display to 0 minuts and 0 seconds.
    STATUS = 0;
    scoreBoard->STATUS = 0;
    TIME->setHMS(ui->timeEdit->time().hour(), ui->timeEdit->time().minute(), ui->timeEdit->time().second());
    ui->lcdTimerMM->display(00);
    ui->lcdTimerSS->display(00);
    scoreBoard->setLCD(0,0);
}

void MainWindow::resetTimer()
{
    //reset the time countdown
    STATUS = 1;
    TIME->setHMS(ui->timeEdit->time().hour(), ui->timeEdit->time().minute(), ui->timeEdit->time().second());
    ui->lcdTimerMM->display(00);
    ui->lcdTimerSS->display(00);
    scoreBoard->setLCD(0,0);
}
void MainWindow::setPixmap1()
{
    //Set picture of home team
    int w = ui->LOGO1->width();
    int h = ui->LOGO1->height();
    IMG0 = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "", tr("Archivo de Imagen (*.png *.jpg *.bmp)"));
    QPixmap mypix (IMG0);
    ui->LOGO1->setPixmap(mypix.scaled(w,h,Qt::KeepAspectRatio));
    scoreBoard->SetPixmap(1,IMG0);
}


void MainWindow::setPixmap2()
{
    //Set picture of guest theam
    int w = ui->LOGO1->width();
    int h = ui->LOGO1->height();
    IMG1 = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "", tr("Archivo de Imagen (*.png *.jpg *.bmp)"));
    QPixmap mypix (IMG1);
    ui->LOGO2->setPixmap(mypix.scaled(w,h,Qt::KeepAspectRatio));
    scoreBoard->SetPixmap(2,IMG1);
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   // Your code here.
   qDebug()<<"Tamanho";
   int w = ui->LOGO1->width();
   int h = ui->LOGO1->height();
   QPixmap mypix0 (IMG0);
   ui->LOGO1->setPixmap(mypix0.scaled(w,h,Qt::KeepAspectRatio));
   QPixmap mypix1 (IMG1);
   ui->LOGO2->setPixmap(mypix1.scaled(w,h,Qt::KeepAspectRatio));
}

void MainWindow::changename()
{
    //Change the challengue name
    scoreBoard->changename(ui->TITLE->text());
}
void MainWindow::teamname()
{
    //update team names
    QString team1,team2;
    team1 = ui->homeName->text();
    team2 = ui->guestName->text();
    scoreBoard->teamname(team1,team2);
}

//void MainWindow::Score()
//{
//    //Chage the score
//}

void MainWindow::keyPressEvent(QKeyEvent *event)

{

        if( event->key() == Qt::Key_A )
        {

            qDebug()<<"Presionaste la tecla A";
            FALTAS1--;
            ui->faltas1->display(FALTAS1);
            scoreBoard->updateFaltas(FALTAS1,FALTAS2);
        }
        else if( event->key() == Qt::Key_D )
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
            FALTAS1++;
            ui->faltas1->display(FALTAS1);
            scoreBoard->updateFaltas(FALTAS1,FALTAS2);
            if (FALTAS1==4)
            {
                Bonus(1);
            }
        }
        else if( event->key() == Qt::Key_W )
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
            PUNTOS1++;
            ui->puntos1->display(PUNTOS1);
            scoreBoard->updatePuntos(PUNTOS1,PUNTOS2);

        }
        else if( event->key() == Qt::Key_S )
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
            PUNTOS1--;
            ui->puntos1->display(PUNTOS1);
            scoreBoard->updatePuntos(PUNTOS1,PUNTOS2);

        }
        else if( event->key() == Qt::Key_J )
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
            FALTAS2--;
            ui->faltas2->display(FALTAS2);
            scoreBoard->updateFaltas(FALTAS1,FALTAS2);

        }
        else if( event->key() == Qt::Key_L )
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
            FALTAS2++;
            ui->faltas2->display(FALTAS2);
            scoreBoard->updateFaltas(FALTAS1,FALTAS2);
            if (FALTAS2==4)
            {
                Bonus(2);
            }

        }
        else if( event->key() == Qt::Key_I )
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
            PUNTOS2++;
            ui->puntos2->display(PUNTOS2);
            scoreBoard->updatePuntos(PUNTOS1,PUNTOS2);

        }
        else if( event->key() == Qt::Key_K )
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
            PUNTOS2--;
            ui->puntos2->display(PUNTOS2);
            scoreBoard->updatePuntos(PUNTOS1,PUNTOS2);

        }
        else if( event->key() == Qt::Key_1 )
        {
            qDebug()<<"Presionaste el numeral "<<event->text();
            QPixmap mypix0 (":images/images/balon.png");
            ui->pos1->setPixmap(mypix0);
            QPixmap mypix1 (":images/images/balon-gray.png");
            ui->pos2->setPixmap(mypix1);
            scoreBoard->balonPOS(1);

        }
        else if( event->key() == Qt::Key_2 )
        {
            qDebug()<<"Presionaste el numeral "<<event->text();
            QPixmap mypix0 (":/images/images/balon-gray.png");
            ui->pos1->setPixmap(mypix0);
            QPixmap mypix1 (":/images/images/balon.png");
            ui->pos2->setPixmap(mypix1);
            scoreBoard->balonPOS(2);

        }
        else if( event->key() == Qt::Key_P )
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
            if(STATUS==1)
            {
                pauseTimer();
            }
            else
            {
                startTimer();
            }

        }
        else if( event->key() == Qt::Key_9 )
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
            PERIODO--;
            ui->PERIODO->display(PERIODO);
            scoreBoard->updatePeriodo(PERIODO);

        }
        else if( event->key() == Qt::Key_0 )
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
            PERIODO++;
            ui->PERIODO->display(PERIODO);
            scoreBoard->updatePeriodo(PERIODO);

        }
        else
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
        }
    }
void MainWindow::DisableTitle()
{
    //disable the title
    ui->TITLE->setEnabled(false);
}

int MainWindow::getStatus()
{
    return STATUS;
}
int MainWindow::Punteo()
{
    // set punteo
    QObject *button = sender();
    qDebug()<<button->objectName();
    if(button->objectName()=="puntos1mas1")
    {
        PUNTOS1++;
    }
    else if(button->objectName()=="puntos1mas2")
    {
        PUNTOS1++;
        PUNTOS1++;
    }
    else if(button->objectName()=="puntos1mas3")
    {
        PUNTOS1++;
        PUNTOS1++;
        PUNTOS1++;
    }
    else if(button->objectName()=="puntos1menos1")
    {
        PUNTOS1--;
    }
    else if(button->objectName()=="puntos2mas1")
    {
        PUNTOS2++;
    }
    else if(button->objectName()=="puntos2mas2")
    {
        PUNTOS2++;
        PUNTOS2++;
    }
    else if(button->objectName()=="puntos2mas3")
    {
        PUNTOS2++;
        PUNTOS2++;
        PUNTOS2++;
    }
    else if(button->objectName()=="puntos2menos1")
    {
        PUNTOS2--;
    }
    ui->puntos1->display(PUNTOS1);
    ui->puntos2->display(PUNTOS2);
    scoreBoard->updatePuntos(PUNTOS1,PUNTOS2);
    return 0;
}
int MainWindow::Faltas()
{
    //manejador de faltas
    QObject *button = sender();

        if (button->objectName() =="FALTA1mas")
        {
            FALTAS1++;
            if (FALTAS1==4)
            {
                Bonus(1);
            }
        }
        else if (button->objectName() =="FALTA1menos")
        {
            FALTAS1--;
        }
        else if (button->objectName() =="FALTA2mas")
        {
            FALTAS2++;
            if (FALTAS1==2)
            {
                Bonus(2);
            }
        }
        else if (button->objectName() =="FALTA2menos")
        {
            FALTAS2--;
        }
    ui->faltas1->display(FALTAS1);
    ui->faltas2->display(FALTAS2);
    scoreBoard->updateFaltas(FALTAS1,FALTAS2);

    return 0;
}

int MainWindow::Periodo()
{
    //manejador de faltas
    QObject *button = sender();

        if (button->objectName() =="Periodomas")
        {
            PERIODO++;
        }
        else if (button->objectName() =="Periodomenos")
        {
            PERIODO--;
        }
    ui->PERIODO->display(PERIODO);
    scoreBoard->updatePeriodo(PERIODO);

    return 0;
}

int MainWindow::Bonus(int TEAM)
{
    //bonus
    QPixmap mypix1 (":/images/images/yellow.png");
    QPixmap mypix2 (":/images/images/gray.png");
    switch(TEAM)
    {
    case 1:
        ui->bonus2->setPixmap(mypix1);
        ui->bonus1->setPixmap(mypix2);
        scoreBoard->updateBonus(1);
        pauseTimer();
        break;
    case 2:
        ui->bonus1->setPixmap(mypix1);
        ui->bonus2->setPixmap(mypix2);
        scoreBoard->updateBonus(2);
        pauseTimer();
        break;

    }
    return 0;

}
