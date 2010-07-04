#ifndef MORSESTAT_H
#define MORSESTAT_H

#include <QObject>

class MorseStat : public QObject
{
    Q_OBJECT
public:
    explicit MorseStat(QObject *parent = 0);

    const int maxTimeCount;

    void  addTime(float newtime);
    float getAverageTime();
    int   getTryCount();

signals:

public slots:

private:
    int             m_tryCount;
    QList<float>    m_timeList;
};

#endif // MORSESTAT_H
