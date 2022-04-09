#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_fieldScene(new FieldScene)
    , m_view(new QGraphicsView)
{
    setFixedSize(840,560);

    m_timer =new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(on_pushButtonNext_clicked()));
    m_timer->setInterval(200);

    m_counter = 0;

    m_ui->setupUi(this);

    m_fieldScene->createEmptyFieldImage();
    m_ui->graphicsView->setScene(m_fieldScene);

    m_socket = new QTcpSocket();
    m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &::MainWindow::socket_readData);
    QObject::connect(m_socket, &QTcpSocket::disconnected, this, &MainWindow::clientDisconnected);

    m_ui->lineEditIP->setText("127.0.0.1");
    m_ui->lineEditPort->setText("12345");

    m_ui->labelCounter->setStyleSheet("font-weight: bold; color: blue; font-size: 18px");
    m_ui->labelConnection->setStyleSheet("font-weight: bold; color: red; font-size: 20px");
    m_ui->pushButtonNext->setEnabled(false);
    m_ui->pushButtonAuto->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete this->m_socket;
    delete m_timer;
    delete m_ui;
    delete m_fieldScene;
}

void MainWindow::on_pushButtonNew_clicked()
{
    m_fieldScene->clear();
    m_fieldScene->createEmptyFieldImage();

    m_fieldScene->makeEmpty();
    m_counter = 0;
    m_ui->labelCounter->setText(QString::number(m_counter));

    m_timer->stop();

    m_ui->pushButtonNext->setEnabled(true);
    m_ui->pushButtonAuto->setEnabled(true);
}


void MainWindow::on_pushButtonConnect_clicked()
{
    QString my_ip = m_ui->lineEditIP->text();
    int my_port = m_ui ->lineEditPort->text().toInt();

    if (m_ui->pushButtonConnect->text() == "Connect")
    {
        connectSocket(my_ip, my_port);
        m_ui->pushButtonConnect->setText("Disconnect");
    }
    else
    {
        disconnectSocket();
        m_ui->pushButtonConnect->setText("Connect");
    }

}

void MainWindow::on_pushButtonNext_clicked()
{
    auto field = m_fieldScene->get_field();
    QString stringField;

    for (int i = 0; i < field.size(); i++)
    {
        for (int j = 0; j <field[i].size(); j++)
        {
            stringField += QString::number(field[i][j]);
        }
        stringField += "\n";
    }

    m_socket->write(stringField.toLatin1());
    m_socket->flush();
}

void MainWindow::socket_readData()
{
    QByteArray buffer;
    buffer = m_socket->readAll();

    if(!buffer.isEmpty())
    {
        m_fieldScene->clear();
        m_fieldScene->createFieldImage(buffer);

        m_ui->labelCounter->setStyleSheet("font-weight: bold; color: blue; font-size: 18px");
        m_ui->labelCounter->setText(QString::number(++m_counter));
    }
    if (m_fieldScene->checkEmpty())
    {
        m_ui->pushButtonNext->setEnabled(false);
        m_ui->pushButtonAuto->setEnabled(false);
        m_ui->pushButtonNew->setEnabled(true);
        m_ui->labelCounter->setStyleSheet("font-weight: bold; color: red; font-size: 18px");
        m_ui->labelCounter->setText("End! : " + QString::number(m_counter));
        m_timer->stop();
    }
}


void MainWindow::on_pushButtonAuto_clicked()
{
    if (m_ui->pushButtonAuto->text() == "Auto")
    {
        m_ui->pushButtonNext->setEnabled(false);
        m_ui->pushButtonNew->setEnabled(false);
        m_ui->pushButtonAuto->setText("Stop");

        m_timer->start();
    }
    else
    {
        m_timer->stop();
        m_ui->pushButtonNext->setEnabled(true);
        m_ui->pushButtonNew->setEnabled(true);
        m_ui->pushButtonAuto->setText("Auto");
    }
}

void MainWindow::connectSocket(QString my_ip, int my_port)
{
    m_socket->abort();
    m_socket->connectToHost(my_ip, my_port);

    if(!m_socket->waitForConnected(3000))
    {
        m_ui->labelConnection->setText("Connection failed!");
        return;
    }
     m_ui->labelConnection->setStyleSheet("font-weight: bold; color: green; font-size: 20px");
     m_ui->labelConnection->setText("Connected!");
     m_ui->pushButtonNext->setEnabled(true);
     m_ui->pushButtonAuto->setEnabled(true);
     m_ui->pushButtonNew->setEnabled(true);
}

void MainWindow::disconnectSocket()
{
    m_socket->abort();
}

void MainWindow::clientDisconnected()
{
    m_ui->labelConnection->setStyleSheet("font-weight: bold; color: red; font-size: 20px");
    m_ui->labelConnection->setText("Disconnected!");
    m_ui->pushButtonConnect->setText("Connect");
    m_ui->pushButtonNext->setEnabled(false);
    m_ui->pushButtonAuto->setEnabled(false);
    m_ui->pushButtonNew->setEnabled(false);
}
