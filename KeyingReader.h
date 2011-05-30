#ifndef KEYINGREADER_H
#define KEYINGREADER_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QHash>

class KeyingReader
{
public:
    KeyingReader();

    QString analyzeKeying(const QList<int> &m_keyedTimes, int *timingUsed, bool useAll = false);
    QString analyzeKey(const QList<int> &m_keyedTimes, int pauseLength);

private:
    QHash<int, QChar> inverseCode;
    int pastDitLength;
};

#endif // KEYINGREADER_H
