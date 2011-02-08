#ifndef MORSESTAT_H
#define MORSESTAT_H

#include <QObject>

class MorseStat : public QObject
{
    Q_OBJECT
public:
    explicit MorseStat(QObject *parent = 0);

    const int maxTimeCount;

    float getAverageTime();
    int   getTryCount();
    int   getGoodCount();
    int   getGoodPercentage();

    void addStat(float newtime, bool successful);

private:
    void  addTime(float newtime);

signals:

public slots:

private:
    int             m_tryCount;
    int             m_goodCount;
    QList<float>    m_timeList;
};

#endif // MORSESTAT_H
