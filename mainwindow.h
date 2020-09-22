#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mygraphicsview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include "rtspthread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    RTSPThread* threads[6];
    MyGraphicsView* views[6];

private slots:
    void on_pushButton_clicked();

    void on_actionE_xit_triggered();

    void graphicsViewClicked(int val);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
