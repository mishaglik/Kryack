#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPropertyAnimation>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPalette>
#include <QBitmap>
#include <QSound>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , patcher("./patch")
{
    ui->setupUi(this);
    pentagramm = new Pentagramm(this);

    setFixedSize(400, 400);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
//    setStyleSheet("background:transparent;");
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background-color:fromRGBA(0, 0, 0, 0)");
    QRegion maskedRegion(0, 0, 400, 400, QRegion::Ellipse);
    setMask(maskedRegion);

    connect(pentagramm, SIGNAL(ready()), this, SLOT(beginDrops()));
    connect(pentagramm, SIGNAL(finished()), this, SLOT(cthuly()));

    catOnDuck = new QWidget(this);
    catOnDuck->setGeometry(0, 0, 400, 400);
    catOnDuck->setStyleSheet("background-image:url(./cat.jpg)");
    catOnDuck->hide();
//    pentagramm->hide();
    pentagramm->init();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pentagramm;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QString filename = event->mimeData()->urls().first().toLocalFile();
    if(patch(filename)){
        setAcceptDrops(false);
        pentagramm->start();
    }
}

bool MainWindow::patch(QString filename)
{
    QFile file(filename);

    switch(patcher.checkFile(file)){
    case Patcher::State::Unknown:
        ui->statusbar->showMessage("Unknown file");
        break;
    case Patcher::State::Patched:
        ui->statusbar->showMessage("Already patched");
        break;
    case Patcher::State::Original:
        patcher.patch(file);
        ui->statusbar->showMessage("Patched succesfull");
        return patcher.checkFile(file) == Patcher::State::Patched;
        break;
    }
    return false;
}

void MainWindow::beginDrops()
{
//    setAcceptDrops(true);
    pentagramm->start();
}

void MainWindow::cthuly()
{
    clearMask();
    pentagramm->hide();
    catOnDuck->show();
    QSound::play("./quak.wav");
    close();
}

