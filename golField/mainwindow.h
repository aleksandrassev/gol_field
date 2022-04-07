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

    void socket_readData();   

private:
    Ui::MainWindow *m_ui;
    FieldScene *m_fieldScene;
    QGraphicsView *m_view;
    QTimer *timer;

    QTcpSocket *socket;
    int counter;
};
#endif // MAINWINDOW_H
