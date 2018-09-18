#include "mainwindow.h"
#include <QApplication>
#include <QtWebEngineWidgets>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    QWebEngineView *view = new QWebEngineView();
//    view->setUrl(QUrl("file:///C:/Users/houwd/Desktop/programming/Qt5/classic_cryptography/testEcharts.html"));
//    view->show();
    return a.exec();
}
