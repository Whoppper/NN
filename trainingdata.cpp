#include "trainingdata.h"

#include <sstream>
#include <string>
#include <QDebug>
TrainingData::TrainingData(const QString& filename) : mTrainingDataFile(filename), mFileStream(&mTrainingDataFile)
{
    if(mTrainingDataFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&mTrainingDataFile);
    }
}

TrainingData::~TrainingData()
{
    if(mTrainingDataFile.isOpen())
    {
        mTrainingDataFile.close();
    }
}

bool TrainingData::isEof()
{
    return mFileStream.atEnd();
}

void TrainingData::getTopology(QVector<int> &topology)
{
    QString label;
    QString topo = mFileStream.readLine();
    //qDebug() << "topo " << topo;
    QStringList topoLst = topo.split(' ');
    for (const QString &t : topoLst)
    {
        topology.push_back(t.toInt());
    }
    return;
}



int TrainingData::getNextInputs(QVector<double> &inputVals)
{
    inputVals.clear();

    QString topo = mFileStream.readLine();
    //qDebug() << "input " << topo;
    QStringList topoLst = topo.split(' ');
    for (const QString &t : topoLst)
    {
        inputVals.push_back(t.toInt());
    }

    return inputVals.size();
}

int TrainingData::getTargetOutputs(QVector<double> &targetOutputVals)
{
    targetOutputVals.clear();

    QString line;

    QString topo = mFileStream.readLine();
    //qDebug() << "output " << topo;
    QStringList topoLst = topo.split(' ');
    for (const QString &t : topoLst)
    {
        targetOutputVals.push_back(t.toDouble());
    }

    return targetOutputVals.size();
}
