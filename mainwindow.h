#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QGraphicsScene>
#include <qcustomplot.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void t_h_graf_paint();

public slots:
    void on_starting_clicked();
    void on_stoping_clicked();
    void newuser();
    void slotReadClient();
    void graphClicked(QCPAbstractPlottable *plottable);

private slots:
    void on_graf_clicked();



private:

    Ui::MainWindow *ui;
     QTcpServer *tcpServer;
     int server_status;
     QMap<int,QTcpSocket *> SClients;
     QStringList h,t;
     QVector <double>x,yh,yt;
};

#endif // MAINWINDOW_H
