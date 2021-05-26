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

private:
    QFile mTrainingDataFile;
    QTextStream mFileStream;
};
#endif // TRAININGDATA_H
