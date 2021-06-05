#ifndef TRAININGDATA_H
#define TRAININGDATA_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>

class TrainingData
{
public:

    TrainingData() = delete;
    TrainingData(const QString &filename);
    ~TrainingData();

    void getTopology(QVector<int> &topology);
    bool isEof();
    int getNextInputs(QVector<double> &inputVals);
    int getTargetOutputs(QVector<double> &targetOutputVals);
    int parseFile();

public:
    QFile mTrainingDataFile;
    QTextStream mFileStream;
    QVector<int> mTopology;
    QVector<QVector<double>> mInputs;
    QVector<QVector<double>> mOutputs;
};
#endif // TRAININGDATA_H
