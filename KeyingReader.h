#ifndef KEYINGREADER_H
#define KEYINGREADER_H

#include <QtCore/QString>
#include <QtCore/QList>

class KeyingReader
{
public:
    KeyingReader();

    QString analyzeKeying(const QList<int> &m_keyedTimes, int *timingUsed, bool useAll = false);
};

#endif // KEYINGREADER_H
