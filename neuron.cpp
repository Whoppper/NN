#include "neuron.h"
#include "connection.h"

double Neuron::eta = 0.15;    // overall net learning rate, [0.0..1.0]
double Neuron::alpha = 0.1;   // momentum, multiplier of last deltaWeight, [0.0..1.0]


void Neuron::updateInputWeights(Layer &prevLayer)
{
    // The weights to be updated are in the Connection container
    // in the neurons in the preceding layer

    for (int n = 0; n < prevLayer.size(); ++n)
    {
        Neuron *neuron = prevLayer[n];
        double oldDeltaWeight = neuron->mOutputWeights[mIndex]->deltaWeight;

        double newDeltaWeight = eta * neuron->getOutputVal() * mGradient + oldDeltaWeight;

        neuron->mOutputWeights[mIndex]->deltaWeight = newDeltaWeight;
        neuron->mOutputWeights[mIndex]->weight += newDeltaWeight;
    }
}

double Neuron::sumDOW(const Layer &nextLayer) const
{
    double sum = 0.0;

    // Sum our contributions of the errors at the nodes we feed.

    for (int n = 0; n < nextLayer.size() - 1; ++n)
    {
        sum += mOutputWeights[n]->weight * nextLayer[n]->mGradient;
    }

    return sum;
}

void Neuron::calcHiddenGradients(const Layer &nextLayer)
{
    double dow = sumDOW(nextLayer);
    //mGradient = dow * Neuron::transferFunctionDerivative(mOutputVal);
    mGradient = dow * Neuron::transferFunctionDerivative(mInputVal);
}

void Neuron::calcOutputGradients(double targetVal)
{
    double delta = targetVal - mOutputVal;
    //mGradient = delta * Neuron::transferFunctionDerivative(mOutputVal);
    mGradient = dow * Neuron::transferFunctionDerivative(mInputVal);
}

/*where m_inputVal is a new member variable and is set to sum:
...
m_inputVal = sum;
m_outputVal = Neuron::transferFunction(sum);
}*/

double Neuron::transferFunction(double x)
{
    // tanh - output range [-1.0..1.0]


    double res = tanh(x);
    return res;
}

double Neuron::transferFunctionDerivative(double x)
{
    // tanh derivative

    double res = 1.0 - tanh(x) * tanh(x);
    //return 1.0 - x * x;
    return res;
}

void Neuron::feedForward(const Layer &prevLayer)
{
    double sum = 0.0;

    // Sum the previous layer's outputs (which are our inputs)
    // Include the bias node from the previous layer.

    for (int n = 0; n < prevLayer.size(); ++n)
    {
        sum += prevLayer[n]->getOutputVal() * prevLayer[n]->mOutputWeights[mIndex]->weight;
    }

    mOutputVal = Neuron::transferFunction(sum);
}

Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
    for (unsigned c = 0; c < numOutputs; ++c)
    {
        mOutputWeights.push_back(new Connection());
        mOutputWeights.back()->weight = randomWeight();
    }

    mIndex = myIndex;
}

Neuron::~Neuron()
{
    for (int i = 0; i < mOutputWeights.size(); i++)
    {
        Connection *connection = mOutputWeights[i];
        delete connection;
    }
    mOutputWeights.clear();
}
