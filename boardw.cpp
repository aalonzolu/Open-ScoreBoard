#include "boardw.h"
#include "ui_boardw.h"
#include <QDesktopWidget>

BoardW::BoardW(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BoardW)
{
    ui->setupUi(this);

}

BoardW::~BoardW()
{
    delete ui;
}

void BoardW::setLCD(int minute,int second)
{
ui->lcdTimerMM->display(minute);
ui->lcdTimerSS->display(second);
qDebug()<<"Hola desde la segunda funcion";
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
    QDesktopWidget *desktop = QApplication::desktop();
    if ( 1==desktop->screenCount()  ) {
        // single monitor - use built in
        showFullScreen();
    } else {
        QRect rect = desktop->screenGeometry(1);
        move(rect.topLeft());
        setWindowState(Qt::WindowFullScreen);
        this->resize( QApplication::desktop()->size() );
    }
}

void BoardW::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   {
        //hokla
       qDebug()<<"Resize 2";
       int w = ui->LOGO1->width();
       int h = ui->LOGO1->height();
       QPixmap mypix0 (IMG0);
       ui->LOGO1->setPixmap(mypix0.scaled(w,h,Qt::KeepAspectRatio));
       QPixmap mypix1 (IMG1);
       ui->LOGO2->setPixmap(mypix1.scaled(w,h,Qt::KeepAspectRatio));
   }
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

        if( event->key() == Qt::Key_A )
        {
            // do your stuff here
            qDebug()<<"2: Presionaste la tecla A";
        }
        else if( event->key() == Qt::Key_D )
        {
            qDebug()<<"2: Presionaste la tecla "<<event->text();
        }
        else if( event->key() == Qt::Key_W )
        {
            qDebug()<<"2: Presionaste la tecla "<<event->text();

        }
        else if( event->key() == Qt::Key_S )
        {
            qDebug()<<"2: Presionaste la tecla "<<event->text();

        }
        else if( event->key() == Qt::Key_J )
        {
            qDebug()<<"2: Presionaste la tecla "<<event->text();

        }
        else if( event->key() == Qt::Key_1 )
        {
            qDebug()<<"2: Presionaste el numeral "<<event->text();
            QPixmap mypix0 (":images/images/balon.png");
            ui->pos1->setPixmap(mypix0);
            QPixmap mypix1 (":images/images/balon-gray.png");
            ui->pos2->setPixmap(mypix1);


        }
        else if( event->key() == Qt::Key_2 )
        {
            qDebug()<<"2: Presionaste el numeral "<<event->text();
            QPixmap mypix0 (":/images/images/balon-gray.png");
            ui->pos1->setPixmap(mypix0);
            QPixmap mypix1 (":/images/images/balon.png");
            ui->pos2->setPixmap(mypix1);
        }
        else if( event->key() == Qt::Key_P )
        {
            qDebug()<<"2: Presionaste la tecla "<<event->text();
        }
        else
        {
            qDebug()<<"Presionaste la tecla "<<event->text();
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
