#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Net;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createMenus();

private slots:

    void newNet();
    void startTraining();
    void editNet();
    void saveNet();
    void loadNet();

private:
    Net *mNet;

};
#endif // MAINWINDOW_H
