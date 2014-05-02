#ifndef QMODESTART_H
#define QMODESTART_H

#include <QDialog>
#include <QVBoxLayout>

class QModeStart : public QDialog
{
    Q_OBJECT
public:
    explicit QModeStart(QWidget *parent = 0, QString modeTitle = "");
    QVBoxLayout *mainLayout();

signals:

public slots:

private:
    QVBoxLayout *m_main;

};

#endif // QMODESTART_H
