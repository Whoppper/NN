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
    void randomizeConnectionsWeight(void);
    void setBiaisOutputVal(void);

    const QVector<Layer> &layers() const;
    void setLayers(const QVector<Layer> &newLayers);
    void clearOutput(void);

public slots:
    void reset();
    bool parseTrainingFile(const QString &filename);
    bool parseNetFile(const QString &filename);
    void create(const QVector<int> &topology);
    void startTraining();

signals:
    void netUpdated();

private:
    void createConnections();
    bool netIsValid();

private:
    QVector<Layer> mLayers;
    double mError;
    double mRecentAverageError;
    static double mRecentAverageSmoothingFactor;
    QVector<QVector<double>> mInputs;
    QVector<QVector<double>> mOutputs;

    bool test;

    int mTrainingIndex;
};

#endif // NET_H
