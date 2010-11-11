#ifndef CHOOSESEQUENCEDIALOG_H
#define CHOOSESEQUENCEDIALOG_H

#include <QtCore/QSignalMapper>
#include <QtCore/QTime>
#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtGui/QGridLayout>
#include <QtGui/QAction>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QMessageBox>
#include <QtGui/QInputDialog>
#include <QtGui/QCheckBox>

class CustomSequenceDialog : public QDialog
{
    Q_OBJECT
public:
    CustomSequenceDialog(QString &sequenceSet, QWidget *parent = 0);

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
