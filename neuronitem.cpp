#include "neuronitem.h"
#include "neuron.h"

#include <QDebug>


NeuronItem::NeuronItem(int layerIndex, int neuronIndex, Neuron *neuron):
mLayerIndex(layerIndex),
mNeuronIndex(neuronIndex),
mNeuron(neuron)
{
    setPos(QPointF(layerIndex * LAYER_SPACER, neuronIndex * NEURON_SPACER));
}

void NeuronItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //qDebug() << "Paint called";
    QColor color(255, 128, 50);
    if (mNeuron->outputVal() >= 0 && mNeuron->outputVal() < 1)
    {
        color = QColor(mNeuron->outputVal() * 250, mNeuron->outputVal() * 250, mNeuron->outputVal() * 250);
    }
    else
        color = QColor(0,0,0);
    painter->setBrush(color);
    painter->drawEllipse(x(), y(), NEURON_WIDTH, NEURON_HEIGHT);
}

QRectF NeuronItem::boundingRect() const
{
    return QRectF(- NEURON_WIDTH / 2,- NEURON_HEIGHT / 2,NEURON_WIDTH / 2, NEURON_HEIGHT / 2);
}

QPainterPath NeuronItem::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(- NEURON_WIDTH / 2,- NEURON_HEIGHT / 2,NEURON_WIDTH / 2, NEURON_HEIGHT / 2));
    return path;
}

