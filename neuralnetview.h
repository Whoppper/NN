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

private:

    QGraphicsScene *mScene;
    QWeakPointer<Net> mNet;
    bool mRightMousePressed;
    int mOriginX;
    int mOriginY;

public slots:
    void wheelEvent(QWheelEvent *e);
    void onNetReady(QSharedPointer<Net> net);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // NEURALNETVIEW_H
