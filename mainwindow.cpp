#include "mainwindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QDomDocument>
#include <QInputDialog>
#include <QMap>

#include "net.h"
#include "newnetdialog.h"
#include "trainingdialog.h"
#include "connection.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mNet(Q_NULLPTR)

{
    mNetView = new NeuralNetView();
    connect(this, &MainWindow::netReady,mNetView, &NeuralNetView::onNetReady );
    setCentralWidget(mNetView);
    createMenus();
    //resize(1000, 700);
    show();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenus()
{
    QAction *newAct = new QAction(tr("&New"), this);
    newAct->setStatusTip(tr("Create a new net"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newNet);

    QAction *EditAct = new QAction(tr("&Edit"), this);
    newAct->setStatusTip(tr("Edit net"));
    connect(EditAct, &QAction::triggered, this, &MainWindow::editNet);

    QAction *startAct = new QAction(tr("&Start"), this);
    newAct->setStatusTip(tr("Start Training"));
    connect(startAct, &QAction::triggered, this, &MainWindow::startTraining);

    QAction *saveAct = new QAction(tr("&Save"), this);
    newAct->setStatusTip(tr("Save Net"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveNet);

    QAction *loadAct = new QAction(tr("&Load"), this);
    newAct->setStatusTip(tr("Load Net"));
    connect(loadAct, &QAction::triggered, this, &MainWindow::loadNet);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(EditAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(loadAct);
    fileMenu->addAction(startAct);
}

void MainWindow::newNet()
{
    NewNetDialog *netDialog = new NewNetDialog(this);
    if (netDialog->exec() == QDialog::Accepted)
    {
        qDebug() << netDialog->getTopology();
        if (!mNet.isNull())
            mNet.clear();
        mNet = QSharedPointer<Net>(new Net());
        mNet->create(netDialog->getTopology());
        emit netReady(mNet);
    }

}

void MainWindow::startTraining()
{
    if (mNet.isNull())
    {
        qDebug() << "Load a net first";
        return ;
    }
    TrainingDialog *trainingDialog = new TrainingDialog(this);
    if (trainingDialog->exec() == QDialog::Accepted)
    {
        bool success = mNet->parseTrainingFile(trainingDialog->getTrainingFile());
        if (!success)
        {
            qDebug() << "Error parseTrainingFile";
            return ;
        }
        mNet->startTraining();
    }
}

void MainWindow::loadNet()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Load Net file"), ".", tr("File (*.xml)"));
    if (path.isEmpty())
        return;
    if (!mNet.isNull())
    {
        mNet.clear();
    }
    mNet = QSharedPointer<Net>(new Net());
    if (mNet->parseNetFile(path) == false)
    {
        qDebug() << "Error Load Net file";
        mNet.clear();
        return ;
    }
    emit netReady(mNet);
}

void MainWindow::editNet()
{


}

void MainWindow::saveNet()
{
    if (mNet.isNull())
    {
        qDebug() << "No net to save";
        return ;
    }
    bool ok;
    QString filename = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("save file:"), QLineEdit::Normal,
                                         tr("netFile"), &ok);
    if (!ok && filename.isEmpty())
    {
        qDebug() << "Invalid input";
        return ;
    }
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
            qDebug() << "Net not saved";
            return ;
        }
    }
    QDomDocument doc;
    QDomElement netXml = doc.createElement("Net");
    for (int layer = 0; layer < mNet->layers().size() ; layer++)
    {
        QDomElement layerXml = doc.createElement("Layer");
        netXml.appendChild(layerXml);
        for (int neuron = 0; neuron < mNet->layers()[layer].size() ; neuron++)
        {
            QDomElement neuronXml = doc.createElement("Neuron");
            neuronXml.setAttribute("id", mNet->layers()[layer][neuron]->id());
            layerXml.appendChild(neuronXml);
            for (int connectionIndex : mNet->layers()[layer][neuron]->getConnections().keys())
            {
                Connection * connection = mNet->layers()[layer][neuron]->getConnections()[connectionIndex];
                QDomElement connectionXml = doc.createElement("Connection");
                connectionXml.setAttribute("weight", connection->weight());
                connectionXml.setAttribute("momentum", connection->momentum());
                connectionXml.setAttribute("id", connectionIndex);
                neuronXml.appendChild(connectionXml);
            }
        }
    }
    doc.appendChild(netXml);
    if ( file.open(QIODevice::WriteOnly))
    {
        QTextStream stream( &file );
        stream << doc.toString();
        file.close();
    }
    else
    {
        qDebug() << "Can not open  the file: nets/" + filename+ ".xml";
    }
}


