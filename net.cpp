#include "net.h"
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include "connection.h"

namespace
{
    void showVectorVals(QString label, QVector<double> &v)
    {
        QString str;
        for (int i = 0; i < v.size(); ++i)
        {
            str +=  QString::number(v[i]) + " ";
        }

        qDebug() << label << str;
    }
}

double Net::mRecentAverageSmoothingFactor = 100.0; // Number of training samples to average over

Net::Net() : mTrainingIndex(0)
{
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

void Net::reset()
{
    for (Layer &layer :  mLayers)
    {
        for (Neuron *neuron :  layer)
        {
            delete neuron;
        }
    }
    mTrainingIndex = 0;
    mLayers.clear();
    mError = 0.0;
    mRecentAverageError = 0.0;
    mInputs.clear();
    mOutputs.clear();
}



void Net::getResults(QVector<double> &resultVals)
{
    resultVals.clear();

    for (int n = 0; n < mLayers.back().size() - 1; ++n)
    {
        resultVals.push_back(mLayers.back()[n]->outputVal());
    }
}

void Net::backProp(const QVector<double> &targetVals)
{
    Layer &outputLayer = mLayers.back();
    mError = 0.0;
    for (int n = 0; n < outputLayer.size() - 1; ++n)
    {
        double delta = targetVals[n] - outputLayer[n]->outputVal();
        mError += delta * delta;
    }
    mError /= outputLayer.size() - 1;
    mError = sqrt(mError); // RMS
    mRecentAverageError = (mRecentAverageError * mRecentAverageSmoothingFactor + mError)/ (mRecentAverageSmoothingFactor + 1.0);

    for (int n = 0; n < outputLayer.size() - 1; ++n)
    {
        outputLayer[n]->calcOutputGradients(targetVals[n]);
    }

    for (int layerNum = mLayers.size() - 2; layerNum > 0; --layerNum)
    {
        Layer &hiddenLayer = mLayers[layerNum];
        Layer &nextLayer = mLayers[layerNum + 1];

        for (int n = 0; n < hiddenLayer.size(); ++n)
        {
            hiddenLayer[n]->calcHiddenGradients(nextLayer);
        }
    }

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

    for (int i = 0; i < inputVals.size(); ++i)
    {
        mLayers[0][i]->setOutputVal(inputVals[i]);
    }

    for (int layerNum = 1; layerNum < mLayers.size(); ++layerNum)
    {
        Layer &prevLayer = mLayers[layerNum - 1];
        for (int n = 0; n < mLayers[layerNum].size() - 1; ++n)
        {
            mLayers[layerNum][n]->feedForward(prevLayer);
        }
    }
}


void Net::createTopology(const QVector<int> &topology)
{
    int numLayers = topology.size();
    for (int layerNum = 0; layerNum < numLayers; ++layerNum)
    {
        mLayers.push_back(Layer());
        for (int neuronNum = 0; neuronNum < topology[layerNum]; ++neuronNum)
        {
            mLayers.back().push_back(new Neuron());
        }
        mLayers.back().push_back(new Neuron());
        mLayers.back().back()->setOutputVal(1.0);
    }
}

void Net::parseTrainingFile(const QString &filename)
{
    QFile mTrainingDataFile(filename);
    if(QFileInfo::exists(filename))
    {
        qDebug () <<  QString("file: [%1] does not exist").arg(filename);
        return ;
    }
    if(!mTrainingDataFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << QString("Can not open the file: ") + filename;
        return ;
    }
    while (!mTrainingDataFile.atEnd())
    {
        QByteArray bline = mTrainingDataFile.readLine();
        while (!mTrainingDataFile.atEnd())
        {
            QVector<double> inputs;
            bline = mTrainingDataFile.readLine();
            QString inputLine(bline);
            QStringList inputLst = inputLine.split(' ');
            for (const QString &t : inputLst)
            {
                inputs.push_back(t.toDouble());
            }
            mInputs.push_back(inputs);
            if (mTrainingDataFile.atEnd())
            {
                qDebug() << "Wrong number of lines";
                Q_ASSERT(0);
            }
            QVector<double> outputs;
            bline = mTrainingDataFile.readLine();
            QString outputLine(bline);
            QStringList outputLst = outputLine.split(' ');
            for (const QString &t : outputLst)
            {
                outputs.push_back(t.toDouble());
            }
            mOutputs.push_back(outputs);
        }
    }
}
void Net::createConnections(const QString &filename)
{
    bool isValid = true;
    QFile mTrainingDataFile(filename);
    if(QFileInfo::exists(filename))
    {
        //qDebug () <<  QString("file: [%1] does not exist").arg(filename);
        isValid = false;
    }
    if(!mTrainingDataFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug () << QString("Can not open the file: ") + filename;
        isValid = false;
    }
    if (isValid)
    {
        //TODO
        Q_ASSERT(0);
    }
    else
    {
        for (int l = 0; l < mLayers.size() - 2; l++)
        {
            Layer layer = mLayers[l];
            Layer nextLayer = mLayers[l + 1];
            for (int n = 0; n < layer.size() - 1; n++)
            {
                Neuron *neuron = layer[n];
                QHash<int, Connection *> connections;
                for (int nn = 0; nn < nextLayer.size(); nn++)
                {
                    Neuron *nextNeuron = nextLayer[nn];
                    connections[nextNeuron->id()] = new Connection();
                }
                neuron->setConnections(connections);
            }
        }
    }
}

bool Net::netIsValid()
{
    //TODO (connections faites (au moins un chemin jusqua la sortie), input size = output size, layers size > 1, layer.back.size = outputs.size, ...)
    return true;
}

void Net::startTraining()
{
    if (!netIsValid())
    {
        qDebug() << "Neural net is invalid";
        reset();
        return ;
    }
    while (mTrainingIndex < mInputs.size())
    {
        qDebug()  << "Index " << mTrainingIndex;
        QVector<double> resultVals;

        showVectorVals("Inputs:", mInputs[mTrainingIndex]);
        feedForward(mInputs[mTrainingIndex]);

        getResults(resultVals);
        showVectorVals("Net output: ", resultVals);

        showVectorVals("outputs should have been:", mOutputs[mTrainingIndex]);
        assert(mOutputs[mTrainingIndex].size() == mLayers.back().size());

        backProp(mOutputs[mTrainingIndex]);

        qDebug()  << "Net recent average error: " << getRecentAverageError() << "\n";
        ++mTrainingIndex;
    }

    qDebug() << "Done";
}


