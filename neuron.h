#ifndef NEURON_H
#define NEURON_H

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <QVector>

class Connection;

class Neuron;
typedef  QVector<Neuron *> Layer;
class Neuron
{
public:
    Neuron(unsigned numOutputs, unsigned myIndex, bool isBiais = false);
    ~Neuron();
    void setOutputVal(double val) { mOutputVal = val; }
    double getOutputVal(void) const { return mOutputVal; }


    void feedForward(const Layer &prevLayer);
    void calcOutputGradients(double targetVal);
    void calcHiddenGradients(const Layer &nextLayer);
    void updateInputWeights(Layer &prevLayer);

public:
    static double eta;   // [0.0..1.0] overall net training rate
    static double alpha; // [0.0..n] multiplier of last weight change (momentum)

    static double transferFunction(double x);
    static double transferFunctionDerivative(double x);
    static double randomWeight(void) { return ((1 + rand() % 10) / 10.1) ; }
    double sumDOW(const Layer &nextLayer) const;
    double mOutputVal;
    double mInputVal;
    QVector<Connection *> mOutputWeights;
    unsigned mIndex;
    double mGradient;
    bool mIsBiais;
};



#endif // NEURON_H
