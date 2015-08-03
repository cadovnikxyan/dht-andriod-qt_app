#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
   // connect(ui->textinfo,SIGNAL(t_h_graf_data()),this,SLOT(t_h_data_change()));
    t_h_graf_paint();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_starting_clicked()
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));
    if (!tcpServer->listen(QHostAddress::Any, 33333) && server_status==0) {
        ui->textinfo->append(tcpServer->errorString());
    } else {
        server_status=1;
        ui->textinfo->append(QString::fromUtf8("Сервер запущен!"));

    }
}

void MainWindow::on_stoping_clicked()
{
    if(server_status==1){
        foreach(int i,SClients.keys()){
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        ui->textinfo->append(QString::fromUtf8("Сервер остановлен!"));
        server_status=0;
    }
}


void MainWindow::newuser()
{
    if(server_status==1){
        ui->textinfo->append(QString::fromUtf8("У нас новое соединение!"));
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
          qApp->processEvents();
    }
}

void MainWindow::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs=clientSocket->socketDescriptor();
    QTextStream os(clientSocket);
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.0 200 Ok\r\n"
          "Content-Type: text/html; charset=\"utf-8\"\r\n"
          "\r\n"
          "<h1>Nothing to see here</h1>\n";


    QString buf=clientSocket->readAll();
    QStringList list_buf=buf.split("|");
    h.append(list_buf.at(0));
    t.append(list_buf.at(1));
    emit t_h_graf_data();
    for(int i=0;i<list_buf.size();i++)
    {
        ui->textinfo->append(list_buf.at(i));
          qApp->processEvents();
    }
    // Если нужно закрыть сокет
    clientSocket->close();
    SClients.remove(idusersocs);
}

void MainWindow::t_h_graf_paint()
{

    ui->widget->xAxis->setRange(0,100);
    ui->widget->yAxis->setRange(0,100);
    ui->widget->yAxis->setLabel("Влажность, \n Температура");
    QPen pen(Qt::green);
    pen.setWidth(9);
    QFont font;
    font.setPointSize(8);
    ui->widget->yAxis->setBasePen(pen);
    ui->widget->yAxis->setSubTickPen(QPen(Qt::green));
    ui->widget->yAxis->setLabelFont(font);
    ui->widget->xAxis->setBasePen(pen);
    ui->widget->xAxis->setSubTickPen(QPen(Qt::green));


}

void MainWindow::on_graf_clicked()
{
    QVector <double>x,yh,yt;
  qApp->processEvents();
    for(int i=0;i<h.size();i++)
    {
        QString bufh=h.at(i);
        QString buft=t.at(i);
        yh.append(bufh.toDouble());
        yt.append(buft.toDouble());
        x.append(i);
    }
    QPen penh,pent;
    penh.setWidth(9);
    penh.setColor(Qt::blue);
    pent.setWidth(9);
    pent.setColor(Qt::red);
    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(penh);
    ui->widget->addGraph();
    ui->widget->graph(1)->setPen(pent);
    ui->widget->graph(0)->setData(x,yh);
    ui->widget->graph(0)->rescaleAxes();
    ui->widget->graph(1)->setData(x,yt);
    ui->widget->graph(1)->rescaleAxes();
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->yAxis2, SLOT(setRange(QCPRange)));

}

void MainWindow::t_h_data_change()
{

}
