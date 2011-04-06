#ifndef KEYTIMINGDISPLAY_H
#define KEYTIMINGDISPLAY_H

#include <QWidget>

class KeyTimingDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KeyTimingDisplay(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

    void setTimings(QList<int> keyedTimings, QList<int> requiredTimings);
    void clear();

signals:

public slots:

private:
    QList<int> m_keyedTimings;
    QList<int> m_requiredTimings;
};

#endif // KEYTIMINGDISPLAY_H
