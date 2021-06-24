#include "net.h"
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDomDocument>
#include <QCoreApplication>
#include "connection.h"

namespace
{
    void showVectorVals(QString label, QVector<double> &v)
    {
        QString str;
        for (int i = 0; i < v.size(); ++i)
        {
            if (i != 0)
                str+= " ";
            str +=  QString::number(v[i]) ;
        }

        qDebug() << label << str;
    }
}

double Net::mRecentAverageSmoothingFactor = 100.0; // Number of training samples to average over

Net::Net()
{
    reset();
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
    test = true;
}



void Net::getResults(QVector<double> &resultVals)
{
    resultVals.clear();

    for (int n = 0; n < mLayers.back().size(); ++n)
    {
        resultVals.push_back(mLayers.back()[n]->outputVal());
    }
}

void Net::backProp(const QVector<double> &targetVals)
{
    Layer &outputLayer = mLayers.back();
    mError = 0.0;
    for (int n = 0; n < outputLayer.size(); ++n)
    {
        double delta = targetVals[n] - outputLayer[n]->outputVal();
        mError += delta * delta;
    }
    mError /= outputLayer.size();
    mError = sqrt(mError);
    mRecentAverageError = (mRecentAverageError * mRecentAverageSmoothingFactor + mError)/ (mRecentAverageSmoothingFactor + 1.0);

    for (int n = 0; n < outputLayer.size(); ++n)
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

        for (int n = 0; n < layer.size(); ++n)
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
        for (int n = 0; n < mLayers[layerNum].size(); ++n)
        {
            if (layerNum != mLayers.size() -1 && n == mLayers[layerNum].size() - 1) //ne pas faire le feedforward sur les biais
            {
                continue;
            }
            mLayers[layerNum][n]->feedForward(prevLayer);
        }

    }
}

bool Net::parseTrainingFile(const QString &filename)
{
    QFile mTrainingDataFile(filename);
    if(!QFileInfo::exists(filename))
    {
        qDebug () <<  QString("file: [%1] does not exist").arg(filename);
        return false;
    }
    if(!mTrainingDataFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << QString("Can not open the file: ") + filename;
        return false;
    }
    mInputs.clear();
    mOutputs.clear();
    mTrainingIndex = 0;
    while (!mTrainingDataFile.atEnd())
    {
        QByteArray bline;
        while (!mTrainingDataFile.atEnd())
        {
            QVector<double> inputs;
            bline = mTrainingDataFile.readLine();
            QString inputLine(bline);
            QStringList inputLst = inputLine.split(' ');
            for (const QString &t : inputLst)
            {
                inputs.push_back(t.trimmed().toDouble());
            }
            mInputs.push_back(inputs);
            if (mTrainingDataFile.atEnd())
            {
                qDebug() << "Wrong number of lines";
                return false;
            }
            QVector<double> outputs;
            bline = mTrainingDataFile.readLine();
            QString outputLine(bline);
            QStringList outputLst = outputLine.split(' ');
            for (const QString &t : outputLst)
            {
                outputs.push_back(t.trimmed().toDouble());
            }
            mOutputs.push_back(outputs);
        }
    }
    return true;
}

bool Net::parseNetFile(const QString &filename)
{
    reset();
    QDomDocument xmlNet;
    // Load xml file as raw data
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly ))
    {
        qDebug () << QString("Can not open the file: ") + filename;
        return false;
    }
    // Set data into the QDomDocument before processing
    xmlNet.setContent(&file);
    file.close();
    QDomNode nodeNet = xmlNet.firstChild();
    QDomElement elementNet = nodeNet.toElement();
    if (elementNet.tagName() != "Net")
    {
        qDebug() << "e.tagName() != Net";
        return false;
    }
    QDomNode nodeLayer = nodeNet.firstChild();
    while (!nodeLayer.isNull())
    {
        QDomElement elementLayer = nodeLayer.toElement();
        if (elementLayer.tagName() != "Layer" )
        {
            qDebug() << "e.tagName() != Layer";
            return false;
        }
        mLayers.push_back(Layer());
        QDomNode nodeNeuron = nodeLayer.firstChild();
        Layer &layer = mLayers.back();
        while (!nodeNeuron.isNull())
        {
            QDomElement elementNeuron = nodeNeuron.toElement();
            if (elementNeuron.tagName() != "Neuron")
            {
                qDebug() << "e.tagName() != Neuron";
                return false;
            }
            layer.push_back(new Neuron(elementNeuron.attribute("id").toInt()));
            Neuron *neuron = layer.back();
            QDomNode nodeConnection = nodeNeuron.firstChild();
            while (!nodeConnection.isNull())
            {
                QDomElement elementConnection = nodeConnection.toElement();
                if (elementConnection.tagName() != "Connection")
                {
                    qDebug() << "e.tagName() != Connection";
                    return false;
                }
                (neuron->getConnections())[elementConnection.attribute("id").toInt()] = new Connection();
                (neuron->getConnections())[elementConnection.attribute("id").toInt()]->setWeight(elementConnection.attribute("weight").toDouble());
                (neuron->getConnections())[elementConnection.attribute("id").toInt()]->setMomentum(elementConnection.attribute("momentum").toDouble());

                nodeConnection = nodeConnection.nextSibling();
            }

            nodeNeuron = nodeNeuron.nextSibling();
        }
        nodeLayer = nodeLayer.nextSibling();
    }
    setBiaisOutputVal();
    return true;
}



bool Net::netIsValid()
{
    //TODO (connections faites (au moins un chemin jusqua la sortie), input size = output size, layers size > 1, layer.back.size = outputs.size, ...)
    return true;
}

const QVector<Layer> &Net::layers() const
{
    return mLayers;
}

void Net::setLayers(const QVector<Layer> &newLayers)
{
    mLayers = newLayers;
}

void Net::startTraining()
{
    if (!netIsValid())
    {
        qDebug() << "Neural net is invalid";
        reset();
        return ;
    }
    test = false;
    while (mTrainingIndex < mInputs.size())
    {
        qDebug()  << "Index " << mTrainingIndex;
        QVector<double> resultVals;

        showVectorVals("Inputs:", mInputs[mTrainingIndex]);
        feedForward(mInputs[mTrainingIndex]);
        emit(netUpdated());
        QCoreApplication::processEvents();
        if (test == true)
            return ;
        getResults(resultVals);
        showVectorVals("Net output: ", resultVals);

        showVectorVals("outputs should have been:", mOutputs[mTrainingIndex]);
        assert(mOutputs[mTrainingIndex].size() == mLayers.back().size());

        backProp(mOutputs[mTrainingIndex]);
        QCoreApplication::processEvents();
        if (test == true)
            return ;
        qDebug()  << "Net recent average error: " << getRecentAverageError() << "\n";
        ++mTrainingIndex;
        clearOutput();
    }

    qDebug() << "Done";
}

void Net::randomizeConnectionsWeight(void)
{
    for (int l = 0 ; l < mLayers.size() - 1; l++)
    {
        for (int n = 0 ; n < mLayers[l].size(); n++)
        {
            mLayers[l][n]->setRandomWeight();
        }
    }
}

void Net::clearOutput(void)
{
    for (int l = 0 ; l < mLayers.size(); l++)
    {
        for (int n = 0 ; n < mLayers[l].size() - 1; n++)
        {
            mLayers[l][n]->setOutputVal(0);
        }
    }
    mLayers[mLayers.size() -1][mLayers[mLayers.size() -1].size() - 1]->setOutputVal(0);
}

void Net::setBiaisOutputVal(void)
{
    for (int l = 0 ; l < mLayers.size() - 1; l++)
    {
        mLayers[l].back()->setOutputVal(1);
    }
}

void Net::create(const QVector<int> &topology)
{
    int neuronId = 0;
    int numLayers = topology.size();
    for (int layerNum = 0; layerNum < numLayers; ++layerNum)
    {
        mLayers.push_back(Layer());
        for (int neuronNum = 0; neuronNum < topology[layerNum]; ++neuronNum)
        {
            mLayers.back().push_back(new Neuron(neuronId++));
        }
        if (layerNum == numLayers - 1)
        {
            // pas de biais pour l'output layer
            continue;
        }
        mLayers.back().push_back(new Neuron(neuronId++));
        mLayers.back().back()->setOutputVal(1.0);
    }
    createConnections();
}

void Net::createConnections()
{
    for (int l = 0; l < mLayers.size() - 1; l++)
    {
        Layer layer = mLayers[l];
        Layer nextLayer = mLayers[l + 1];
        for (int n = 0; n < layer.size(); n++)
        {
            Neuron *neuron = layer[n];
            QMap<int, Connection *> connections;
            for (int nn = 0; nn < nextLayer.size(); nn++)
            {
                if (nn == nextLayer.size() - 1 && l + 1 != mLayers.size() - 1)  // pas de connexion avec le biais
                    continue;
                Neuron *nextNeuron = nextLayer[nn];
                connections[nextNeuron->id()] = new Connection();
            }
            neuron->setConnections(connections);
        }
    }
    randomizeConnectionsWeight();
}

