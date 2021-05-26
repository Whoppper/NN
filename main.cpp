#include <QCoreApplication>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

#include <QDebug>
#include "trainingdata.h"
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
    QCoreApplication a(argc, argv);
    TrainingData trainData("./trainingData.txt");

    // e.g., { 3, 2, 1 }
    QVector<int> topology;
    trainData.getTopology(topology);

    Net myNet(topology);

    QVector<double> inputVals, targetVals, resultVals;
    int trainingPass = 0;

    while (!trainData.isEof())
    {
        ++trainingPass;
        qDebug()  << "Pass " << trainingPass;

        // Get new input data and feed it forward:
        if (trainData.getNextInputs(inputVals) != topology[0])
        {
            break;
        }
        showVectorVals("Inputs:", inputVals);
        myNet.feedForward(inputVals);

        // Collect the net's actual output results:
        myNet.getResults(resultVals);
        showVectorVals("net output: ", resultVals);

        // Train the net what the outputs should have been:
        trainData.getTargetOutputs(targetVals);
        showVectorVals("outputs should have been:", targetVals);
        qDebug() << targetVals.size() << " " << topology.back();
        assert(targetVals.size() == topology.back());

        myNet.backProp(targetVals);

        // Report how well the training is working, average over recent samples:
        qDebug()  << "Net recent average error: " << myNet.getRecentAverageError() << "\n";
    }

    qDebug() << "Done";
}


