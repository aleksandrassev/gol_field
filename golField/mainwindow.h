#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QTcpSocket>
#include <QTimer>

#include <fstream>
#include <iostream>

#include "fieldscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonNext_clicked();
    void on_pushButtonNew_clicked();
    void on_pushButtonConnect_clicked();
    void on_pushButtonAuto_clicked();
    void clientDisconnected();
    void socket_readData();   

private:
    Ui::MainWindow *m_ui;
    FieldScene *m_fieldScene;
    QGraphicsView *m_view;
    QTimer *m_timer;

    QTcpSocket *m_socket;
    int m_counter;

    void connectSocket(QString my_ip, int my_port);
    void disconnectSocket();
};
#endif // MAINWINDOW_H
