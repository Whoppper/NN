#ifndef NEURONITEM_H
#define NEURONITEM_H

#include <QObject>
#include <QWidget>

#include <QGraphicsItem>
#include <QPainter>

#include "neuron.h"

class NeuronItem : public QGraphicsItem
{
public:
    NeuronItem();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
};

#endif // NEURONITEM_H
