#include "mainwindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QDomDocument>

#include "net.h"
#include "newnetdialog.h"
#include "trainingdialog.h"

namespace
{
    QString getXmlConnectionId(const QVector<int> &topology, int layerPos, int connection)
    {
        int idNum = 0;
        for (int i = 0; i <= layerPos; i++)
        {
            idNum += topology[i] + 1; // +1 pour compter le biais
        }
        QString id = QString::number(idNum + connection);
        return (id);
    }

    QString getXmlNeuronId(const QVector<int> &topology, int layerPos, int neuron)
    {
        int idNum = 0;
        for (int i = 0; i < layerPos; i++)
        {
            idNum += topology[i] + 1; // +1 pour compter le biais
        }
        QString id = QString::number(idNum + neuron);
        return (id);
    }



    void createXmlFile(const QVector<int> &topology, const QString &filename)
    {
        QDir directory("nets");
        if (!directory.exists())
        {
            directory.mkpath(".");
        }
        QFile file("nets/" + filename +".xml");
        if (QFileInfo::exists("nets/" + filename+".xml"))
        {
            if ( QMessageBox::question(nullptr, "Messagebox", QObject::tr("Replace existing file ?"),QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
            {
                return ;
            }
        }
        QDomDocument doc;
        QDomElement netXml = doc.createElement("Net");
        for (int layer = 0; layer < topology.size() ; layer++)
        {
            QDomElement layerXml = doc.createElement("Layer");
            netXml.appendChild(layerXml);
            for (int neuron = 0; neuron < topology[layer] + 1 ; neuron++)
            {
                if (neuron == topology[layer] && layer == topology.size() - 1) // pas de biais pour l'output layer
                    continue;
                QDomElement neuronXml = doc.createElement("Neuron");
                neuronXml.setAttribute("id", getXmlNeuronId(topology, layer, neuron));
                layerXml.appendChild(neuronXml);
                if (layer == topology.size() - 1) // pas de connection pour les neurones de l'output layer
                    continue;
                for (int connection = 0; connection < topology[layer + 1]; connection++)
                {
                    QDomElement connectionXml = doc.createElement("Connection");
                    connectionXml.setAttribute("weight", "0");
                    connectionXml.setAttribute("momentum", "0");
                    connectionXml.setAttribute("id", getXmlConnectionId(topology,layer, connection));
                    neuronXml.appendChild(connectionXml);
                }
            }
        }
        doc.appendChild(netXml);
        if ( file.open(QIODevice::WriteOnly))
        {
            QString sss =  doc.toString();
            QTextStream stream( &file );
            stream << doc.toString();
            file.close();
        }
        else
        {
            qDebug() << "Can not open  the file: nets/" + filename+ ".xml";
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mNet(Q_NULLPTR)

{
    createMenus();
    show();
}

MainWindow::~MainWindow()
{
    if (mNet)
        delete mNet;
    mNet = Q_NULLPTR;
}

void MainWindow::createMenus()
{
    QAction *newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new net"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newNet);

    QAction *EditAct = new QAction(tr("&Edit"), this);
    newAct->setStatusTip(tr("Edit net"));
    connect(EditAct, &QAction::triggered, this, &MainWindow::editNet);

    QAction *startAct = new QAction(tr("&Start"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Start Training"));
    connect(startAct, &QAction::triggered, this, &MainWindow::startTraining);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(EditAct);
    fileMenu->addAction(startAct);
}

void MainWindow::newNet()
{
    NewNetDialog *netDialog = new NewNetDialog(this);
    if (netDialog->exec() == QDialog::Accepted)
    {
        qDebug() << netDialog->getTopology();
        createXmlFile(netDialog->getTopology(), netDialog->getFilename());
    }
}

void MainWindow::startTraining()
{
    TrainingDialog *trainingDialog = new TrainingDialog(this);
    if (trainingDialog->exec() == QDialog::Accepted)
    {
        if (mNet)
            delete mNet;
        mNet = new Net();
        bool success = mNet->parseTrainingFile(trainingDialog->getTrainingFile());
        if (!success)
        {
            qDebug() << "Error parseTrainingFile";
        }
        success = mNet->parseNetFile(trainingDialog->getNetFile());
        if (!success)
        {
            qDebug() << "Error parseNetFile";
        }
        mNet->randomizeConnectionsWeight();
        mNet->startTraining();
    }
}

void MainWindow::editNet()
{

}
