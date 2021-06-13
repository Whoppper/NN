#include "newnetdialog.h"

NewNetDialog::NewNetDialog(QWidget *parent) : QDialog(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    mLeTopology = new QLineEdit(this);
    mLeFilename = new QLineEdit(this);
    mLeFilename->setPlaceholderText("filename");
    mLeTopology->setPlaceholderText("3 2 1");
    mPbValidate = new QPushButton (tr("Validate"), this);
    mPbCancel = new QPushButton (tr("Cancel"), this);
    mPbValidate->setEnabled(false);
    hLayout->addWidget(mLeTopology);
    hLayout->addWidget(mLeFilename);
    hLayout->addWidget(mPbValidate);
    hLayout->addWidget(mPbCancel);

    QObject::connect(mPbValidate, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(mPbCancel, &QPushButton::clicked, this, &QDialog::reject);
    QObject::connect(mLeTopology, &QLineEdit::textChanged, this, &NewNetDialog::checkEnabled);
    QObject::connect(mLeFilename, &QLineEdit::textChanged, this, &NewNetDialog::checkEnabled);
}

void NewNetDialog::checkEnabled()
{
    bool enableButton = true;
    QString line = mLeTopology->text().trimmed();
    QStringList layersSize = line.split(" ");
    if (layersSize.size() < 2)
    {
        enableButton = false;
    }
    else
    {
        for (int i = 0; i < layersSize.size(); i++)
        {
            if (layersSize[i].toInt() == 0)
                enableButton = false;
        }
    }
    if (mLeFilename->text().trimmed() == "")
        enableButton = false;
    mPbValidate->setEnabled(enableButton);
}

QVector<int> NewNetDialog::getTopology()
{
    QVector<int> topology;
    QString line = mLeTopology->text().trimmed();
    QStringList layersSize = line.split(" ");
    for (int i = 0; i < layersSize.size(); i++)
    {
        topology.push_back(layersSize[i].toInt());
    }
    return topology;
}

QString NewNetDialog::getFilename()
{
    return mLeFilename->text().trimmed();
}
