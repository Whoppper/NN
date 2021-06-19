#ifndef NEURALNETVIEW_H
#define NEURALNETVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWeakPointer>

class Net;

class NeuralNetView : public QGraphicsView
{
    Q_OBJECT
public:
    NeuralNetView(QWidget *parent = nullptr);


private:

    QGraphicsScene *mScene;
    QWeakPointer<Net> mNet;

public slots:

    void onNetReady(QSharedPointer<Net> net);
};

#endif // NEURALNETVIEW_H
