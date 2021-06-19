#ifndef TRAININGDIALOG_H
#define TRAININGDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>

class TrainingDialog : public QDialog
{
    Q_OBJECT
public:
    TrainingDialog(QWidget *parent=0);
    QString getTrainingFile();
    void enableValidButton();

private:
    QPushButton *mPbTraining;
    QLineEdit *mLeTraining;
    QPushButton *mPbValidate;
    QPushButton *mPbCancel;

private slots:

    void selectTrainingFile();
};

#endif // TRAININGDIALOG_H
