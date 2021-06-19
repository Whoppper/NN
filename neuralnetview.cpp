#include "neuralnetview.h"

#include "mainwindow.h"
#include "net.h"
#include "neuronitem.h"

#include <QGraphicsEllipseItem>

NeuralNetView::NeuralNetView(QWidget *parent) : QGraphicsView(parent)
{
    mScene = new QGraphicsScene(this);
    mScene->setSceneRect(-300, -300, 600, 600);

    mScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Net"));
    //resize(800, 600);
    show();


}


void NeuralNetView::onNetReady(QSharedPointer<Net> net)
{
    if (net.isNull())
        return;
    mNet = net.toWeakRef();
    for (int l = 0; l < net->layers().size(); l++)
    {
        Layer layer = net->layers()[l];
        for (int n = 0; n < layer.size(); n++)
        {
            Neuron *neuron = layer[n];
            NeuronItem *item= new NeuronItem(/*0 + l * 50, 0 + n * 50, 20, 20*/);
            mScene->addItem(item);
            auto circle = new QGraphicsEllipseItem(0, 0, 100, 100);
            circle->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
            circle->setBrush(Qt::green);
            mScene->addItem(circle);
        }
    }
    mScene->update();
    show();
}


