#ifndef NET_H
#define NET_H

#include <QVector>
#include "neuron.h"

class Net
{
public:
    Net(const QVector<int> &topology);
    ~Net();
    void feedForward(const QVector<double> &inputVals);
    void backProp(const QVector<double> &targetVals);
    void getResults(QVector<double> &resultVals);
    double getRecentAverageError(void) const { return mRecentAverageError; }

public:
    QVector<Layer> mLayers; // m_layers[layerNum][neuronNum]
    double mError;
    double mRecentAverageError;
    static double mRecentAverageSmoothingFactor;
    QVector<QVector<double>> mInputs;
    QVector<QVector<double>> mOutputs;
};

#endif // NET_H
