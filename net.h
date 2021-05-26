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

private:
    QVector<Layer> mLayers; // m_layers[layerNum][neuronNum]
    double mError;
    double mRecentAverageError;
    static double mRecentAverageSmoothingFactor;
};

#endif // NET_H
