#include <QCoreApplication>
#include <QApplication>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

#include "mainwindow.h"

#include <QDebug>
#include "net.h"
#include "connection.h"

void showVectorVals(QString  label, QVector<double> &v)
{
    QString str;
    for (int i = 0; i < v.size(); ++i)
    {
        str +=  QString::number(v[i]) + " ";
    }

    qDebug() << label << str;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


