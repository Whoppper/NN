#ifndef NEURONITEM_H
#define NEURONITEM_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
#include <QPainter>

constexpr int NEURON_WIDTH = 60;
constexpr int NEURON_HEIGHT = 60;
constexpr int LAYER_SPACER = NEURON_WIDTH * 2;
constexpr int NEURON_SPACER = NEURON_HEIGHT * 1.5 ;

class Neuron;

class NeuronItem : public QGraphicsItem
{
public:
    NeuronItem(int layerIndex, int neuronIndex, Neuron *neuron);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget) override;

private:

    int mLayerIndex;
    int mNeuronIndex;
    Neuron *mNeuron;
};

#endif // NEURONITEM_H
