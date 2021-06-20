#ifndef NEURALNETVIEW_H
#define NEURALNETVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWeakPointer>
#include <QWheelEvent>

class Net;

class NeuralNetView : public QGraphicsView
{
    Q_OBJECT
public:
    NeuralNetView(QWidget *parent = nullptr);

    QGraphicsScene *scene() const;

private:

    QGraphicsScene *mScene;
    QWeakPointer<Net> mNet;
    bool mRightMousePressed;
    int mOriginX;
    int mOriginY;

    QPointF target_scene_pos, target_viewport_pos;

    void gentle_zoom(double factor);
    double _zoom_factor_base;
    bool eventFilter(QObject* object, QEvent* event);

public slots:

    void onNetReady(QSharedPointer<Net> net);
    /*void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *e);*/

    void test();


};

#endif // NEURALNETVIEW_H
