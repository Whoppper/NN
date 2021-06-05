#include "trainingdata.h"

#include <sstream>
#include <string>
#include <QDebug>
TrainingData::TrainingData(const QString& filename) : mTrainingDataFile(filename), mFileStream(&mTrainingDataFile)
{

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

int TrainingData::parseFile()
{
    if(mTrainingDataFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        while (!mTrainingDataFile.atEnd())
        {
            QByteArray bline = mTrainingDataFile.readLine();
            QString line(bline);
            QStringList topoLst = line.split(' ');
            for (const QString &t : topoLst)
            {
                mTopology.push_back(t.toInt());
            }
            while (!mTrainingDataFile.atEnd())
            {
                QVector<double> inputs;
                bline = mTrainingDataFile.readLine();
                QString inputLine(bline);
                QStringList inputLst = inputLine.split(' ');
                for (const QString &t : inputLst)
                {
                    inputs.push_back(t.toDouble());
                }
                mInputs.push_back(inputs);
                if (mTrainingDataFile.atEnd())
                {
                    qDebug() << "wrong number of lines";
                    Q_ASSERT(0);
                }
                QVector<double> outputs;
                bline = mTrainingDataFile.readLine();
                QString outputLine(bline);
                QStringList outputLst = outputLine.split(' ');
                for (const QString &t : outputLst)
                {
                    outputs.push_back(t.toDouble());
                }
                mOutputs.push_back(outputs);
            }
        }
    }
}
