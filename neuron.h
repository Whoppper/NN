#ifndef NEURON_H
#define NEURON_H

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <QVector>
#include <QHash>
#include <random>

enum class ActivationFunction
{
    TanH
};

class Connection;

class Neuron;
typedef  QVector<Neuron *> Layer;

class Neuron
{
public:
    Neuron(const QHash<int, Connection *> &connections);
    Neuron();
    ~Neuron();

    void feedForward(const Layer &prevLayer);
    void calcOutputGradients(double targetVal);
    void calcHiddenGradients(const Layer &nextLayer);
    void updateInputWeights(Layer &prevLayer);

    double transferFunction();
    double transferFunctionDerivative();

    double sumDOW(const Layer &nextLayer) const;

    double outputVal() const;
    void setOutputVal(double newOutputVal);

    double inputVal() const;
    void setInputVal(double newInputVal);

    double gradient() const;
    void setGradient(double newGradient);

    int id() const;
    void setId(int newId);

    void setConnections(const QHash<int, Connection *> &connections);

private:

    void setRandomWeight(void);

    static double sEta;   // [0.0..1.0] overall net training rate
    static double sAlpha; // [0.0..n] multiplier of last weight change (momentum)
    static int sNeuronId;

    int mId;
    double mOutputVal;
    double mInputVal;
    QHash<int, Connection *> mConnections;
    double mGradient;
    ActivationFunction mFunction;

};



#endif // NEURON_H
