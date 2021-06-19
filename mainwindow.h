#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include "neuralnetview.h"

class Net;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createMenus();

    Net *net() const;
    void setNet(Net *newNet);

signals:

    void netReady(QSharedPointer<Net> net);

private slots:

    void newNet();
    void startTraining();
    void editNet();
    void saveNet();
    void loadNet();

private:
    QSharedPointer<Net> mNet;
    NeuralNetView *mNetView;

};
#endif // MAINWINDOW_H
