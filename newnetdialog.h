#ifndef NEWNETDIALOG_H
#define NEWNETDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

class NewNetDialog : public QDialog
{
    Q_OBJECT
public:
    NewNetDialog(QWidget *parent=0);
    QVector<int> getTopology();
    QString getFilename();

public slots:

    void checkEnabled();

private:
    QLineEdit *mLeFilename;
    QLineEdit *mLeTopology;
    QPushButton *mPbValidate;
    QPushButton *mPbCancel;
};

#endif // NEWNETDIALOG_H
