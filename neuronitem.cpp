#include "neuronitem.h"
#include "neuron.h"
#include "connection.h"
#include <QDebug>


NeuronItem::NeuronItem(int layerIndex, int neuronIndex, Neuron *neuron):
mLayerIndex(layerIndex),
mNeuronIndex(neuronIndex),
mNeuron(neuron)
{

    //qDebug() << "pos x:" << pos.x() << "  pos y:" << pos.y();
    //setPos(QPointF(layerIndex * LAYER_SPACER, neuronIndex * NEURON_SPACER));
    //setPos(QPointF(LAYER_SPACER,NEURON_SPACER));
    //qDebug() << "neuron x:" << layerIndex * LAYER_SPACER << "  neuron y:" << neuronIndex * NEURON_SPACER;
    //qDebug() << "neuron x:" << layerIndex * LAYER_SPACER << "  neuron y:" << neuronIndex * NEURON_SPACER;
    //qDebug() << "";

}

void NeuronItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QColor color;
    if (mNeuron->outputVal() >= 0 && mNeuron->outputVal() <= 1)
    {
        color = QColor(255,255 - mNeuron->outputVal() * 255,255 - mNeuron->outputVal() * 255);
    }
    else
        color = QColor(255,255,255);
    const QRect inputRectangle = QRect(10, NEURON_HEIGHT / 2, NEURON_WIDTH, NEURON_HEIGHT);
    const QRect outputRectangle = QRect(NEURON_WIDTH - 10, NEURON_HEIGHT / 2, NEURON_WIDTH, NEURON_HEIGHT);
    const QRect gradientRectangle = QRect(NEURON_WIDTH / 2, 10, NEURON_WIDTH, NEURON_HEIGHT);
    painter->setBrush(color);
    painter->drawEllipse(0,0, NEURON_WIDTH, NEURON_HEIGHT);
    painter->drawText(inputRectangle, QString::number(mNeuron->inputVal()));
    painter->drawText(outputRectangle, QString::number(mNeuron->outputVal()));
    painter->drawText(gradientRectangle, QString::number(mNeuron->gradient()));
    int n = 0;
    /*for (Connection *connection :mNeuron->getConnections())
    {
        color = QColor(0,0,0);
        painter->setBrush(color);
        painter->drawLine(QLine(NEURON_WIDTH *1.5 ,NEURON_HEIGHT / 2, (mLayerIndex + 1) * LAYER_SPACER + NEURON_WIDTH / 2, n * NEURON_SPACER + NEURON_HEIGHT / 2  ));
        n++;
    }*/
}

QRectF NeuronItem::boundingRect() const
{
    return QRectF(0,0,NEURON_WIDTH, NEURON_HEIGHT);
}

QPainterPath NeuronItem::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(0,0,NEURON_WIDTH, NEURON_HEIGHT));
    return path;
}

