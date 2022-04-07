#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_fieldScene(new FieldScene)
    , m_view(new QGraphicsView)
{
    setFixedSize(840,560);

    timer =new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_pushButtonNext_clicked()));
    timer->setInterval(200);

    counter = 0;

    m_ui->setupUi(this);

    m_fieldScene->createEmptyFieldImage();
    m_ui->graphicsView->setScene(m_fieldScene);

    socket = new QTcpSocket();
    QObject::connect(socket, &QTcpSocket::readyRead, this, &::MainWindow::socket_readData);

    m_ui->lineEditIP->setText("127.0.0.1");
    m_ui->lineEditPort->setText("12345");

    m_ui->labelCounter->setStyleSheet("font-weight: bold; color: blue; font-size: 18px");
    m_ui->labelConnection->setStyleSheet("font-weight: bold; color: red; font-size: 20px");
    m_ui->pushButtonNext->setEnabled(false);
    m_ui->pushButtonAuto->setEnabled(false);
    m_ui->pushButtonNew->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete this->socket;
    delete m_ui;
    delete m_fieldScene;
}

void MainWindow::on_pushButtonNew_clicked()
{
    m_fieldScene->clear();
    m_fieldScene->createEmptyFieldImage();

    m_fieldScene->makeEmpty();
    counter = 0;
    m_ui->labelCounter->setText(QString::number(counter));

    timer->stop();

    m_ui->pushButtonNext->setEnabled(true);
    m_ui->pushButtonAuto->setEnabled(true);
}


void MainWindow::on_pushButtonConnect_clicked()
{
    QString my_ip = m_ui->lineEditIP->text();
    int my_port = m_ui ->lineEditPort->text().toInt();

    socket->abort();

    socket->connectToHost(my_ip, my_port);

    if(!socket->waitForConnected(3000))
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

    socket->write(stringField.toLatin1());
    socket->flush();
}

void MainWindow::socket_readData()
{
    QByteArray buffer;
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        m_fieldScene->clear();
        m_fieldScene->createFieldImage(buffer);

        m_ui->labelCounter->setStyleSheet("font-weight: bold; color: blue; font-size: 18px");
        m_ui->labelCounter->setText(QString::number(++counter));
    }
    if (m_fieldScene->checkEmpty())
    {
        m_ui->pushButtonNext->setEnabled(false);
        m_ui->pushButtonAuto->setEnabled(false);
        m_ui->labelCounter->setStyleSheet("font-weight: bold; color: red; font-size: 18px");
        m_ui->labelCounter->setText("End! : " + QString::number(counter));
        timer->stop();
    }
}


void MainWindow::on_pushButtonAuto_clicked()
{
    if (m_ui->pushButtonAuto->text() == "Auto")
    {
        m_ui->pushButtonNext->setEnabled(false);
        m_ui->pushButtonNew->setEnabled(false);
        m_ui->pushButtonAuto->setText("Stop");

        timer->start();
    }
    else
    {
        timer->stop();
        m_ui->pushButtonNext->setEnabled(true);
        m_ui->pushButtonNew->setEnabled(true);
        m_ui->pushButtonAuto->setText("Auto");
    }
}

