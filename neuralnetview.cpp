#include "neuralnetview.h"

#include "mainwindow.h"
#include "net.h"
#include "neuronitem.h"

#include <QGraphicsEllipseItem>
#include <QTimeLine>


NeuralNetView::NeuralNetView(QWidget *parent) : QGraphicsView(parent)
{
    mScene = new QGraphicsScene(-100, -100, 600, 600, this);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setScene(mScene);
    setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Net"));
    mRightMousePressed = false;
}


void NeuralNetView::onNetReady(QSharedPointer<Net> net)
{
    if (net.isNull())
        return;
    mNet = net.toWeakRef();
    int maxNeuronInLayer = 0;
    for (int l = 0; l < net->layers().size(); l++)
    {
        Layer layer = net->layers()[l];
        maxNeuronInLayer = std::max(maxNeuronInLayer, layer.size());
        for (int n = 0; n < layer.size(); n++)
        {
            Neuron *neuron = layer[n];
            NeuronItem *item= new NeuronItem(l, n, neuron);
            mScene->addItem(item);
        }
    }
    setSceneRect(-100, -100, net->layers().size() * LAYER_SPACER, maxNeuronInLayer * NEURON_SPACER);
}

void NeuralNetView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.15;
    if(event->angleDelta().y() > 0)
    {
        scale(scaleFactor, scaleFactor);
    }
    else
    {
         scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
    setTransformationAnchor(QGraphicsView::NoAnchor);
}

void NeuralNetView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        mRightMousePressed = true;
        mOriginX = event->x();
        mOriginY = event->y();
        event->accept();
        return;
    }
}

void NeuralNetView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        mRightMousePressed = false;
        event->accept();
        return;
    }
    event->ignore();
}

void NeuralNetView::mouseMoveEvent(QMouseEvent *event)
{
    if (mRightMousePressed)
    {
        QPointF oldp = mapToScene(mOriginX, mOriginY);
        QPointF newP = mapToScene(event->pos());
        QPointF translation = newP - oldp;
        translate(translation.x(), translation.y());
        mOriginX = event->x();
        mOriginY = event->y();
        event->accept();
        return;
    }
    event->ignore();

}
