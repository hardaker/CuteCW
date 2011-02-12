#include "MorseStat.h"

#include <QtCore/QSettings>

#include "qdebug.h"

MorseStat::MorseStat(QObject *parent) :
    QObject(parent), maxTimeCount(20), m_tryCount(0), m_goodCount(0), m_timeList()
{
}

int MorseStat::getTryCount() {
    return m_tryCount;
}

int MorseStat::getGoodCount() {
    return m_goodCount;
}

int MorseStat::getGoodPercentage() {
    if (m_tryCount == 0)
        return 0;
    return int(100.0 * float(m_goodCount)/float(m_tryCount));
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
    return total/m_timeList.count();
}

void MorseStat::addTime(float newtime) {
    m_timeList.append(newtime);
    if (m_tryCount >= maxTimeCount)
        m_timeList.pop_front();
    m_tryCount++;
}

void MorseStat::addStat(float newtime, bool successful) {
    addTime(newtime);
    if (successful)
        m_goodCount++;
    m_tryCount++;
}

void MorseStat::saveStats(QSettings &settings, const QString &statSetName) {
    settings.setValue(statSetName + "/stats/tryCount", m_tryCount);
    settings.setValue(statSetName + "/stats/goodCount", m_goodCount);

    settings.beginWriteArray(statSetName + "/stats/times");
    int maxListNum = m_timeList.count();
    for(int i = 0; i < maxListNum; i++) {
        settings.setArrayIndex(i);
        settings.setValue("time", m_timeList[i]);
    }
    settings.endArray();
}

void MorseStat::loadStats(QSettings &settings, const QString &statSetName) {
    m_tryCount  = settings.value(statSetName + "/stats/tryCount", 0).toInt();
    m_goodCount = settings.value(statSetName + "/stats/goodCount", 0).toInt();
    m_timeList.clear();
    int maxListNum = settings.beginReadArray(statSetName + "/stats/times");
    for(int i = 0; i < maxListNum; i++) {
        settings.setArrayIndex(i);
        m_timeList.push_back(settings.value("time").toFloat());
    }
    settings.endArray();
}
