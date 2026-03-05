#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QPushButton>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scoreBoard = new BoardW(this);  // Set parent for proper cleanup
    
    // Initialize buzzer sound
    buzzerSound = new QSoundEffect(this);
    buzzerSound->setSource(QUrl("qrc:/sounds/sounds/Brazzer.wav"));
    
    // Initialize theme system
    ThemeManager::instance()->applyTheme();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged, 
            this, &MainWindow::onThemeChanged);
    
    // Create theme menu
    setupThemeMenu();
    
    connect(ui->showButton, &QPushButton::clicked, this, &MainWindow::showScoreboard);
    connect(ui->actionSalir, &QAction::triggered, this, &MainWindow::close);
    connect(ui->timerPlay, &QPushButton::clicked, this, &MainWindow::startTimer);
    connect(ui->timerPause, &QPushButton::clicked, this, &MainWindow::pauseTimer);
    connect(ui->timerStop, &QPushButton::clicked, this, &MainWindow::stopTimer);
    connect(ui->timerRestart, &QPushButton::clicked, this, &MainWindow::resetTimer);
    connect(ui->LOGO1BUTTON, &QPushButton::clicked, this, &MainWindow::setPixmap1);
    connect(ui->LOGO2BUTTON, &QPushButton::clicked, this, &MainWindow::setPixmap2);

    connect(ui->TitleListoButton, &QPushButton::clicked, this, &MainWindow::DisableTitle);

    connect(ui->TITLE, &QLineEdit::textEdited, this, &MainWindow::changename);
    connect(ui->homeName, &QLineEdit::textEdited, this, &MainWindow::teamname);
    connect(ui->guestName, &QLineEdit::textEdited, this, &MainWindow::teamname);

    connect(ui->puntos1mas1, &QPushButton::clicked, this, &MainWindow::Punteo);
    connect(ui->puntos1mas2, &QPushButton::clicked, this, &MainWindow::Punteo);
    connect(ui->puntos1mas3, &QPushButton::clicked, this, &MainWindow::Punteo);
    connect(ui->puntos1menos1, &QPushButton::clicked, this, &MainWindow::Punteo);

    connect(ui->puntos2mas1, &QPushButton::clicked, this, &MainWindow::Punteo);
    connect(ui->puntos2mas2, &QPushButton::clicked, this, &MainWindow::Punteo);
    connect(ui->puntos2mas3, &QPushButton::clicked, this, &MainWindow::Punteo);
    connect(ui->puntos2menos1, &QPushButton::clicked, this, &MainWindow::Punteo);

    connect(ui->FALTA1mas, &QPushButton::clicked, this, &MainWindow::Faltas);
    connect(ui->FALTA1menos, &QPushButton::clicked, this, &MainWindow::Faltas);
    connect(ui->FALTA2mas, &QPushButton::clicked, this, &MainWindow::Faltas);
    connect(ui->FALTA2menos, &QPushButton::clicked, this, &MainWindow::Faltas);

    connect(ui->Periodomas, &QPushButton::clicked, this, &MainWindow::Periodo);
    connect(ui->Periodomenos, &QPushButton::clicked, this, &MainWindow::Periodo);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::countdown);
    timer->start(1000);

    // Inicializar cronómetro con valores del time edit
    timeMinutes = ui->timeEdit->time().minute();
    timeSeconds = ui->timeEdit->time().second();

    PUNTOS1 =0;PUNTOS2 =0;FALTAS1 =0;FALTAS2 =0;BONUS =0;POS =0;PERIODO =0;
}

MainWindow::~MainWindow()
{
    delete scoreBoard;
    delete ui;
}

void MainWindow::setDisplay()
    {

    }

int MainWindow::countdown()
{
    if(STATUS==1)
    {
        if (timeMinutes <= 0 && timeSeconds <= 0)
        {
            // Game time expired - play buzzer sound
            buzzerSound->play();
            stopTimer();
            return 0;
        }
        // Decrementar tiempo
        timeSeconds--;
        if (timeSeconds < 0) {
            timeSeconds = 59;
            timeMinutes--;
        }
        if (timeMinutes < 0) {
            timeMinutes = 0;
            timeSeconds = 0;
        }
        
        // Update timer displays
        ui->lcdTimerMM->display(timeMinutes);
        ui->lcdTimerSS->display(timeSeconds);
        scoreBoard->setLCD(timeMinutes, timeSeconds);
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
        timeMinutes = ui->timeEdit->time().minute();
        timeSeconds = ui->timeEdit->time().second();
    }
    STATUS = 1;
    scoreBoard->STATUS = 1;
    // Timer is now running

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
    timeMinutes = ui->timeEdit->time().minute();
    timeSeconds = ui->timeEdit->time().second();
    ui->lcdTimerMM->display(00);
    ui->lcdTimerSS->display(00);
    scoreBoard->setLCD(0,0);
}

void MainWindow::resetTimer()
{
    //reset the time countdown
    STATUS = 1;
    timeMinutes = ui->timeEdit->time().minute();
    timeSeconds = ui->timeEdit->time().second();
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
   
   // Update logo displays after resize
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Handle keyboard shortcuts for game control
    switch(event->key()) {
        case Qt::Key_A:
            // Team 1: Decrease fouls
            FALTAS1--;
            ui->faltas1->display(FALTAS1);
            scoreBoard->updateFaltas(FALTAS1,FALTAS2);
            break;
            
        case Qt::Key_D:
            // Team 1: Increase fouls
            FALTAS1++;
            ui->faltas1->display(FALTAS1);
            scoreBoard->updateFaltas(FALTAS1,FALTAS2);
            if (FALTAS1==4) {
                Bonus(1);
            }
            break;
            
        case Qt::Key_W:
            // Team 1: Increase score
            PUNTOS1++;
            ui->puntos1->display(PUNTOS1);
            scoreBoard->updatePuntos(PUNTOS1,PUNTOS2);
            break;
            
        case Qt::Key_S:
            // Team 1: Decrease score
            PUNTOS1--;
            ui->puntos1->display(PUNTOS1);
            scoreBoard->updatePuntos(PUNTOS1,PUNTOS2);
            break;
        case Qt::Key_J:
            // Team 2: Decrease fouls
            FALTAS2--;
            ui->faltas2->display(FALTAS2);
            scoreBoard->updateFaltas(FALTAS1,FALTAS2);
            break;
            
        case Qt::Key_L:
            // Team 2: Increase fouls
            FALTAS2++;
            ui->faltas2->display(FALTAS2);
            scoreBoard->updateFaltas(FALTAS1,FALTAS2);
            if (FALTAS2==4) {
                Bonus(2);
            }
            break;
            
        case Qt::Key_I:
            // Team 2: Increase score
            PUNTOS2++;
            ui->puntos2->display(PUNTOS2);
            scoreBoard->updatePuntos(PUNTOS1,PUNTOS2);
            break;
            
        case Qt::Key_K:
            // Team 2: Decrease score
            PUNTOS2--;
            ui->puntos2->display(PUNTOS2);
            scoreBoard->updatePuntos(PUNTOS1,PUNTOS2);
            break;
        case Qt::Key_1:
            // Set ball possession to Team 1
            {
            QPixmap mypix0 (":/images/images/balon.png");
            ui->pos1->setPixmap(mypix0);
            QPixmap mypix1 (":/images/images/balon-gray.png");
            ui->pos2->setPixmap(mypix1);
            scoreBoard->balonPOS(1);
            }
            break;
            
        case Qt::Key_2:
            // Set ball possession to Team 2
            {
            QPixmap mypix0 (":/images/images/balon-gray.png");
            ui->pos1->setPixmap(mypix0);
            QPixmap mypix1 (":/images/images/balon.png");
            ui->pos2->setPixmap(mypix1);
            scoreBoard->balonPOS(2);
            }
            break;
        case Qt::Key_P:
            // Toggle play/pause timer
            if(STATUS==1) {
                pauseTimer();
            } else {
                startTimer();
            }
            break;
            
        case Qt::Key_9:
            // Decrease period
            PERIODO--;
            ui->PERIODO->display(PERIODO);
            scoreBoard->updatePeriodo(PERIODO);
            break;
            
        case Qt::Key_0:
            // Increase period
            PERIODO++;
            ui->PERIODO->display(PERIODO);
            scoreBoard->updatePeriodo(PERIODO);
            break;
            
        default:
            // Unhandled key - pass to parent
            QMainWindow::keyPressEvent(event);
            break;
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
    // Set punteo - handle scoring button clicks
    QObject *button = sender();
    
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

void MainWindow::setupThemeMenu()
{
    // Create theme menu in menu bar
    QMenu *themeMenu = menuBar()->addMenu("&Theme");
    
    QAction *lightAction = themeMenu->addAction("Light Theme");
    QAction *darkAction = themeMenu->addAction("Dark Theme");
    QAction *autoAction = themeMenu->addAction("Auto Theme");
    
    connect(lightAction, &QAction::triggered, [this]() {
        ThemeManager::instance()->setTheme(ThemeManager::Light);
    });
    
    connect(darkAction, &QAction::triggered, [this]() {
        ThemeManager::instance()->setTheme(ThemeManager::Dark);
    });
    
    connect(autoAction, &QAction::triggered, [this]() {
        ThemeManager::instance()->setTheme(ThemeManager::Auto);
    });
}

void MainWindow::onThemeChanged()
{
    // Handle theme changes - refresh UI elements if needed
    ThemeManager::instance()->applyTheme();
}

void MainWindow::showThemeMenu()
{
    // Reserved slot for showing theme menu
}