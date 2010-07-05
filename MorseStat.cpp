#include "MorseStat.h"
#include "qdebug.h"

MorseStat::MorseStat(QObject *parent) :
    QObject(parent), maxTimeCount(20), m_tryCount(0), m_timeList()
{
}

int MorseStat::getTryCount() {
    return m_tryCount;
}

float MorseStat::getAverageTime() {
    QList<float>::iterator iter;
    QList<float>::iterator end = m_timeList.end();
    float total = 0.0;

    if (m_tryCount == 0)
        return -1.0;

    for(iter = m_timeList.begin(); iter != end; iter++) {
        total = total + *iter;
    }
    return total/m_tryCount;
}

void MorseStat::addTime(float newtime) {
    qDebug() << "  adding time: " << newtime << " ms";
    m_timeList.append(newtime);
    if (m_tryCount >= maxTimeCount)
        m_timeList.pop_front();
    else
        m_tryCount++;
}
