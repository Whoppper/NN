#ifndef NET_H
#define NET_H

#include <QObject>

#include <QVector>
#include "neuron.h"

class Net : public QObject
{
    Q_OBJECT
public:
    Net();
    ~Net();
    void feedForward(const QVector<double> &inputVals);
    void backProp(const QVector<double> &targetVals);
    void getResults(QVector<double> &resultVals);
    double getRecentAverageError(void) const { return mRecentAverageError; }

public slots:
    void reset();
    void createTopology(const QVector<int> &topology);
    void parseTrainingFile(const QString &filename);
    void createConnections(const QString &filename);
    void startTraining();

private:

    bool netIsValid();

private:
    QVector<Layer> mLayers;
    double mError;
    double mRecentAverageError;
    static double mRecentAverageSmoothingFactor;
    QVector<QVector<double>> mInputs;
    QVector<QVector<double>> mOutputs;

    int mTrainingIndex;
};

#endif // NET_H
