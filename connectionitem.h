#ifndef CONNECTIONITEM_H
#define CONNECTIONITEM_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
#include <QPainter>

constexpr int NEURON_WIDTH = 80;
constexpr int NEURON_HEIGHT = 80;
constexpr int LAYER_SPACER = NEURON_WIDTH * 4;
constexpr int NEURON_SPACER = NEURON_HEIGHT * 3 ;

class Connection;

/*class ConnectionItem : public QGraphicsItem
{
public:
    ConnectionItem(int Neuron, int neuronIndex, Connection *connection);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget) override;

private:

    int mLayerIndex;
    int mNeuronIndex;
    Neuron *mNeuron;
};*/

#endif // CONNECTIONITEM_H
