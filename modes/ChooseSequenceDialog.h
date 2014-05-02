#ifndef CHOOSESEQUENCEDIALOG_H
#define CHOOSESEQUENCEDIALOG_H

#include <QtCore/QSignalMapper>
#include <QtCore/QTime>
#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QGridLayout>
#include <QAction>
#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QCheckBox>

class CustomSequenceDialog : public QDialog
{
    Q_OBJECT
public:
    CustomSequenceDialog(QString &sequenceSet, const QString preSelected = "", QWidget *parent = 0);

    QString getResults();

public slots:
    void toggle(const QString &item);

private:
    QVBoxLayout      *topLayout;
    QGridLayout      *grid;
    QDialogButtonBox *buttonBox;
    QSignalMapper    *mapper;
    QSet<QString>     results;
};

#endif // CHOOSESEQUENCEDIALOG_H
