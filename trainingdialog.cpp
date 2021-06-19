#include "trainingdialog.h"

TrainingDialog::TrainingDialog(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QWidget *widget1 = new QWidget(this);
    QWidget *widget2 = new QWidget(this);

    vLayout->addWidget(widget1);
    vLayout->addWidget(widget2);

    QHBoxLayout *hLayout = new QHBoxLayout();
    QHBoxLayout *h2Layout = new QHBoxLayout();

    widget1->setLayout(hLayout);
    widget2->setLayout(h2Layout);

    mPbValidate = new QPushButton (tr("Validate"), this);
    mPbCancel = new QPushButton (tr("Cancel"), this);
    mPbTraining = new QPushButton(tr("Select training file"),this);
    mLeTraining = new QLineEdit(this);

    mLeTraining->setEnabled(false);
    mPbValidate->setEnabled(false);

    hLayout->addWidget(mLeTraining);
    hLayout->addWidget(mPbTraining);

    h2Layout->addWidget(mPbValidate);
    h2Layout->addWidget(mPbCancel);

    QObject::connect(mPbValidate, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(mPbCancel, &QPushButton::clicked, this, &QDialog::reject);

    QObject::connect(mPbTraining, &QPushButton::clicked, this, &TrainingDialog::selectTrainingFile);
}

void TrainingDialog::enableValidButton()
{
    if (mLeTraining->text().trimmed() != "")
        mPbValidate->setEnabled(true);
}

void TrainingDialog::selectTrainingFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Trainning File"), ".", tr("File (*.txt)"));
    if (path.isEmpty())
        return;
    mLeTraining->setText(path);
    enableValidButton();
}

QString TrainingDialog::getTrainingFile()
{
    return mLeTraining->text().trimmed();
}
