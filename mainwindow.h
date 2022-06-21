#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMovie>

#include "pentagramm.h"
#include "patcher.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent *event) override;
private:
    Ui::MainWindow *ui;

    Pentagramm *pentagramm;
    QWidget* catOnDuck;
    Patcher patcher;

    bool patch(QString filename);
private slots:
    void beginDrops();
    void cthuly();
};
#endif // MAINWINDOW_H
