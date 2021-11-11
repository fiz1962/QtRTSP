#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#ifdef ANDROID
    #include <QtAndroid>
#endif
#include <QDebug>
#include <QString>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <QStandardPaths>
#include <QFile>
#include <QDir>

using namespace std;
using namespace rapidjson;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef ANDROID
    auto  result = QtAndroid::checkPermission(QString("android.permission.INTERNET"));
    if(result == QtAndroid::PermissionResult::Denied){
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.INTERNET"}));
        if(resultHash["android.permission.INTERNET"] == QtAndroid::PermissionResult::Denied)
            ui->textEdit->append("INTERNET permission denied");
    }

    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if(r == QtAndroid::PermissionResult::Denied) {
        QtAndroid::requestPermissionsSync( QStringList() << "android.permission.WRITE_EXTERNAL_STORAGE" );
        r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
        if(r == QtAndroid::PermissionResult::Denied) {
            ui->textEdit->append("WRITE permission denied");
        }
    }

    r = QtAndroid::checkPermission("android.permission.READ_EXTERNAL_STORAGE");
    if(r == QtAndroid::PermissionResult::Denied) {
        QtAndroid::requestPermissionsSync( QStringList() << "android.permission.READ_EXTERNAL_STORAGE" );
        r = QtAndroid::checkPermission("android.permission.READ_EXTERNAL_STORAGE");
        if(r == QtAndroid::PermissionResult::Denied) {
            ui->textEdit->append("READ permission denied");
        }
    }

#endif

    views[0] = ui->graphicsView_1;
    views[1] = ui->graphicsView_2;
    views[2] = ui->graphicsView_3;
    views[3] = ui->graphicsView_4;
    views[4] = ui->graphicsView_5;
    views[5] = ui->graphicsView_6;

    for(int n=1; n<6; n++) {
        views[n]->hide();
        threads[n] = NULL;
    }

    avformat_network_init();

    ui->textEdit->append("QtRTSP v1.0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/QtRTSP";

    //ui->textEdit->append(path); // '/storage/emulated/0/Download'
    if(QFile::exists(path+"/QtRTSP.json")) {
        ui->textEdit->append(path+"/QtRTSP.json Exists");
        //ui->textEdit->append(path);
        QString jsonFile = path + "/QtRTSP.json";
        ifstream ifs(jsonFile.toStdString());
        IStreamWrapper isw(ifs);
        Document d;
        d.ParseStream(isw);
        for(int n=0; n<3; n++) {
            QString url = "URL" + QString::number(n+1);
            if( d.HasMember(url.toStdString().c_str()) ) {
                threads[n] = new RTSPThread;
                ui->textEdit->append("Opening " + QString(d[url.toStdString().c_str()].GetString()));
                //update();
                QApplication::processEvents();
                if (threads[n]->Init(views[n], d[url.toStdString().c_str()].GetString()) ) {
                    threads[n]->start();
                    views[n]->show();
                } else {
                    ui->textEdit->append("Could not open "+QString(d[url.toStdString().c_str()].GetString()));
                }
            }
        }
    } else {
        ui->textEdit->append(path+"/QtRTSP.json does not exist.  Creating default.");
        QDir dir = QDir::root();
        dir.mkdir(path);
        QFile testFile(path.append("/QtRTSP.json"));
        testFile.open(QFile::WriteOnly);

        QTextStream out(&testFile);
        out << "{\n    \"URL1\": \"rtsp://user:passwd@192.168.1.100:554/live/ch0\"\n}";
        testFile.close();
    }
}


void MainWindow::on_actionE_xit_triggered()
{
    for(int n=0; n<6; n++ ) {
        if( threads[n] && threads[n]->isRunning() ) {
            qDebug() << "Stopping thread";
            threads[n]->quit();
            //threads[0]->Close();
        }
    }
    QApplication::quit();
}

