#include "KeyingReader.h"
#include "Morse.h"
#include "qdebug.h"

KeyingReader::KeyingReader()
    : pastDitLength(-1)
{
    int intCode;

#include "inverse_code.h"
}

QString KeyingReader::analyzeKeying(const QList<int> &m_keyedTimes, int *timingUsed, bool useAll)
{
    QString results = "";
    const int letterSpaceWeight = 4; // number of dits
    int spaceExpected = -1;
    int ditLength = 0xffff;
    int usedCount = 0;
    *timingUsed = 0;
    QList<int>::const_iterator end = m_keyedTimes.end();
    QList<int>::const_iterator spot = m_keyedTimes.begin();
    QList<int>::const_iterator startAt;

    //qDebug() << "keyed times: " << m_keyedTimes;

    while ((useAll && spot != end) || spot == m_keyedTimes.begin()) {
        QList<int> keyedTimes;

        startAt = spot;
        for(; spot != end;) {
            usedCount++;
            // starting point is a key-down
            int downKey = *spot;
            spot++;
            if (spot == end)
                return results; // key pressed down, not up yet

            // next is the release
            int upKey = *spot;

            // calculate the time in between
            int delta = upKey - downKey;

            keyedTimes.push_back(delta);

            // Always use the smallest value for the dit
            if (ditLength > delta) {
                ditLength = delta;
                spaceExpected = delta * letterSpaceWeight;
            }

            // now calculate the length of the space after this
            if (spot != end) {
                // XXX: we should really just require an ending space and force a timer usage
                spot++;
                int nextSpace = *spot - upKey;

                if (nextSpace > spaceExpected) {
                    // we've gotten to a point where everything from startAt till now is a letter.
                    if (m_keyedTimes.count() > 3) // at least two keys
                        results = results + analyzeKey(keyedTimes, (*(m_keyedTimes.begin() + 2) - (*(m_keyedTimes.begin() + 1))));
                    else
                        results = results + analyzeKey(keyedTimes, -1); // single character; we don't know how long it should be
                    keyedTimes.clear();
                    *timingUsed = usedCount;
                }
            }
        }
    }
    return results;
}

QString KeyingReader::analyzeKey(const QList<int> &m_keyedLengths, int pauseLength) {
    // we have a complete sequence from first keyDown to last keyUp for a letter
    QList<int>::const_iterator pt1, pt2;
    int sequence = 0;

    qDebug() << "keyed lengths:" << m_keyedLengths;

    int maxv = -1, minv = 0xffff;
    int aveLength;

    QList<int>::const_iterator end = m_keyedLengths.end();
    QList<int>::const_iterator length;

    for(length =  m_keyedLengths.begin() ; length != end; length++) {
        maxv = qMax(maxv, *length);
        minv = qMin(minv, *length);
    }

    qDebug() << "min=" << minv << ", max=" << maxv << ", pause=" << pauseLength * 3;
    // Ok, if the maxv is at least 2x the minv then they're different lengths in general (yay)
    if (maxv > minv*2.0) {
        pastDitLength = minv;
        aveLength = minv + (maxv - minv)/2;
    } else {
        // the lengths are the same, so either they're all dits or they're all dahs
        if (pastDitLength != -1) {
            if (maxv < 2 * pastDitLength)
                aveLength = maxv + 1; // all dahs
            else
                aveLength = minv - 1; // all dits
        } else {
            // the guessing gets worse if we've never recevied a true dit/dah letter yet
            if (maxv < pauseLength/2)
                aveLength = maxv+1; // they're all dits
            else
                aveLength = minv-1; // they're all dahs
        }
    }

    // Ok, now loop through them all and determine what each is.
    for(length =  m_keyedLengths.begin() ; length != end; length++) {
        if (*length > aveLength) {
            // it's a DAH
            sequence = (sequence << 4) | 2;
        } else {
            sequence = (sequence << 4) | 1;
        }
    }

    qDebug() << "keyed: " << sequence;
    if (!inverseCode.contains(sequence)) {
        qWarning() << "ERROR: they keyed something invalid: " << QString::number(sequence, 16);
        return "";
    }
    return inverseCode[sequence];
    // Now we have the list, we can get the char from the sequence
}
