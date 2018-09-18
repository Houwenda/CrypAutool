#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtWebEngineWidgets>
#include <QMainWindow>
#include <QtNetwork>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void printText(QString str,int k);
private slots:
    void on_pushButton_clicked();
    void replyFinished(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QWebEngineView view;
    bool writeQuipquip(QString str);
    bool requestQuipquip(void);
    void staticsAnalysis(QString input,int recurse_count);
    QSqlDatabase database;
};

#endif // MAINWINDOW_H
