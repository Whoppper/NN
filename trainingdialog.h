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
    QString getNetFile();

    void enableValidButton();

private:
    QPushButton *mPbTraining;
    QPushButton *mPbNetFile;
    QLineEdit *mLeTraining;
    QLineEdit *mLeNetFile;

    QPushButton *mPbValidate;
    QPushButton *mPbCancel;

private slots:

    void selectTrainingFile();
    void selectNetFile();
};

#endif // TRAININGDIALOG_H
