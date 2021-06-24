#include "neuralnetview.h"

#include "mainwindow.h"
#include "net.h"
#include "neuronitem.h"

#include <QGraphicsEllipseItem>
#include <QTimeLine>
#include <QtMath>
#include <QDebug>
#include <QScrollBar>
#include <QThread>

NeuralNetView::NeuralNetView(QWidget *parent) : QGraphicsView(parent)
{
    mScene = new QGraphicsScene(0, 0, 1000, 500, this);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setScene(mScene);
    setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Net"));
    mRightMousePressed = false;
    viewport()->installEventFilter(this);
    _zoom_factor_base = 1.0005;
}

QGraphicsScene *NeuralNetView::scene() const
{
    return mScene;
}


void NeuralNetView::onNetReady(QSharedPointer<Net> net)
{
    if (net.isNull())
        return;
    mScene->clear();
    viewport()->update();
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
            item->setPos(l * LAYER_SPACER, n * NEURON_SPACER );
        }
    }
    setSceneRect(-1000, -1000, net->layers().size() * LAYER_SPACER + 2000, maxNeuronInLayer * NEURON_SPACER + 2000);
}

void NeuralNetView::gentle_zoom(double factor)
{
    scale(factor, factor);
    centerOn(target_scene_pos);
    QPointF delta_viewport_pos = target_viewport_pos - QPointF(viewport()->width() / 2.0,
                                                             viewport()->height() / 2.0);
    QPointF viewport_center = mapFromScene(target_scene_pos) - delta_viewport_pos;
    centerOn(mapToScene(viewport_center.toPoint()));
}

bool NeuralNetView::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);

        QPointF delta = target_viewport_pos - mouse_event->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
        {
            target_viewport_pos = mouse_event->pos();
            target_scene_pos = mapToScene(mouse_event->pos());
        }
        if (mRightMousePressed)
        {
            QPointF oldp = mapToScene(mOriginX, mOriginY);
            QPointF newP = mapToScene(mouse_event->pos());
            QPointF translation = newP - oldp;
            translate(translation.x(), translation.y());
        }
        mOriginX = (mouse_event->pos()).x();
        mOriginY = (mouse_event->pos()).y();
        return false;
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
        double angle = wheel_event->angleDelta().y();
        double factor = qPow(_zoom_factor_base, angle);
        gentle_zoom(factor);
        return false;
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        if (mouse_event->button() == Qt::RightButton)
        {
            mRightMousePressed = true;
            mOriginX = (mouse_event->pos()).x();
            mOriginY = (mouse_event->pos()).y();
            return false;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        if (mouse_event->button() == Qt::RightButton)
        {
            mRightMousePressed = false;
            mOriginX = (mouse_event->pos()).x();
            mOriginY = (mouse_event->pos()).y();
        }
        return false;
    }
    Q_UNUSED(object);
    return false;
}

void NeuralNetView::test()
{
   viewport()->repaint();
   //QThread::msleep(500);
}
