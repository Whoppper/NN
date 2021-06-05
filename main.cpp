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
    trainData.parseFile();

    Net myNet(trainData.mTopology);
    myNet.mOutputs = trainData.mOutputs;
    myNet.mInputs = trainData.mInputs;
    QVector<double> resultVals;
    int trainingPass = 0;
    qDebug() << "total data: " <<myNet.mInputs.size();
    /*for (int i = 0; i < myNet.mOutputs.size() ; i++)
    {
         showVectorVals("outputs should have been:", myNet.mOutputs[i]);
    }*/
    while (trainingPass < myNet.mInputs.size())
    {
        //qDebug()  << "Pass " << trainingPass;

        showVectorVals("Inputs:", myNet.mInputs[trainingPass]);
        myNet.feedForward(myNet.mInputs[trainingPass]);

        // Collect the net's actual output results:
        myNet.getResults(resultVals);
        showVectorVals("net output: ", resultVals);

        showVectorVals("outputs should have been:", myNet.mOutputs[trainingPass]);
        //assert(myNet.mOutputs[trainingPass].size() == myNet.mLayers.back().size());

        myNet.backProp(myNet.mOutputs[trainingPass]);

        // Report how well the training is working, average over recent samples:
        //qDebug()  << "Net recent average error: " << myNet.getRecentAverageError() << "\n";
        ++trainingPass;
    }

    qDebug() << "Done";
}


