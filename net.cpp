#include "net.h"
#include <QVector>
#include <QDebug>
double Net::mRecentAverageSmoothingFactor = 100.0; // Number of training samples to average over


void Net::getResults(QVector<double> &resultVals)
{
    resultVals.clear();

    for (int n = 0; n < mLayers.back().size() - 1; ++n)
    {
        resultVals.push_back(mLayers.back()[n]->getOutputVal());
    }
}

void Net::backProp(const QVector<double> &targetVals)
{
    // Calculate overall net error (RMS of output neuron errors)

    Layer &outputLayer = mLayers.back();
    mError = 0.0;

    for (int n = 0; n < outputLayer.size() - 1; ++n)
    {
        double delta = targetVals[n] - outputLayer[n]->getOutputVal();
        mError += delta * delta;
    }
    mError /= outputLayer.size() - 1; // get average error squared
    mError = sqrt(mError); // RMS

    // Implement a recent average measurement

    mRecentAverageError = (mRecentAverageError * mRecentAverageSmoothingFactor + mError)/ (mRecentAverageSmoothingFactor + 1.0);

    // Calculate output layer gradients

    for (int n = 0; n < outputLayer.size() - 1; ++n)
    {
        outputLayer[n]->calcOutputGradients(targetVals[n]);
    }

    // Calculate hidden layer gradients

    for (int layerNum = mLayers.size() - 2; layerNum > 0; --layerNum)
    {
        Layer &hiddenLayer = mLayers[layerNum];
        Layer &nextLayer = mLayers[layerNum + 1];

        for (int n = 0; n < hiddenLayer.size(); ++n)
        {
            hiddenLayer[n]->calcHiddenGradients(nextLayer);
        }
    }

    // For all layers from outputs to first hidden layer,
    // update connection weights

    for (int layerNum = mLayers.size() - 1; layerNum > 0; --layerNum)
    {
        Layer &layer = mLayers[layerNum];
        Layer &prevLayer = mLayers[layerNum - 1];

        for (int n = 0; n < layer.size() - 1; ++n)
        {
            layer[n]->updateInputWeights(prevLayer);
        }
    }
}

void Net::feedForward(const QVector<double> &inputVals)
{
    assert(inputVals.size() == mLayers[0].size() - 1);

    // Assign (latch) the input values into the input neurons
    for (int i = 0; i < inputVals.size(); ++i)
    {
        mLayers[0][i]->setOutputVal(inputVals[i]);
    }

    // forward propagate
    for (int layerNum = 1; layerNum < mLayers.size(); ++layerNum)
    {
        Layer &prevLayer = mLayers[layerNum - 1];
        for (int n = 0; n < mLayers[layerNum].size() - 1; ++n)
        {
            mLayers[layerNum][n]->feedForward(prevLayer);
        }
    }
}

Net::Net(const QVector<int> &topology)
{
    int numLayers = topology.size();
    for (int layerNum = 0; layerNum < numLayers; ++layerNum)
    {
        mLayers.push_back(Layer());
        int numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

        // We have a new layer, now fill it with neurons, and
        // add a bias neuron in each layer.
        for (int neuronNum = 0; neuronNum < topology[layerNum]; ++neuronNum)
        {
            mLayers.back().push_back(new Neuron(numOutputs, neuronNum));
            qDebug() << "Made a Neuron!" ;
        }
        mLayers.back().push_back(new Neuron(numOutputs, topology[layerNum], true ));
        // Force the bias node's output to 1.0 (it was the last neuron pushed in this layer):
        mLayers.back().back()->setOutputVal(1.0);
    }
}

Net::~Net()
{
    for (Layer &layer :  mLayers)
    {
        for (Neuron *neuron :  layer)
        {
            delete neuron;
        }
    }
}
