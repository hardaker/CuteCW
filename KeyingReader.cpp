#include "KeyingReader.h"
#include "Morse.h"

KeyingReader::KeyingReader()
{
}

QString KeyingReader::analyzeKeying(const QList<int> &m_keyedTimes, int *timingUsed, bool useAll)
{
    QString results = "";
    const int letterSpaceWeight = 5; // number of dits
    int spaceExpected = -1;
    int ditLength = -1;
    int usedCount = 0;
    *timingUsed = 0;
    QList<int>::const_iterator end = m_keyedTimes.end();
    QList<int>::const_iterator spot = m_keyedTimes.begin();
    QList<int>::const_iterator startAt;

    while ((useAll && spot != end) || spot == m_keyedTimes.begin()) {
        QList<int> keyedTimes;

        startAt = spot;
        for(; spot != end; spot++) {
            // starting point is a key-down
            int downKey = *spot;
            spot++;
            if (spot == end)
                return results;

            // next is the release
            int upKey = *spot;

            // calculate the time in between
            int delta = upKey - downKey;

            keyedTimes.push_back(delta);

            // if we don't have a spaceExpected setting, then this was either
            // the first dit or dah.  Assume a dit for now and thus the space must be at least 5 long.
            if (spaceExpected == -1)
                spaceExpected = delta * letterSpaceWeight;

            // now calculate the length of the space after this
            if (spot != end) {
                spot++;
                int nextSpace = *spot - upKey;

                if (nextSpace > spaceExpected) {
                    // we've gotten to a point where everything from startAt till now is a letter.
                    analyzeKey(keyedTimes, spaceExpected);
                }
            }
        }
    }
}

QString KeyingReader::analyzeKey(const QList<int> &m_keyedLengths, int pauseLength) {
    // we have a complete sequence from first keyDown to last keyUp
    QList<int>::const_iterator pt1, pt2;
    QList<Morse::ditdah> ditDahs;

    int maxv = -1, minv = 0xffff;
    int aveLength;

    QList<int>::const_iterator end = m_keyedLengths.end();
    QList<int>::const_iterator length;

    for(length =  m_keyedLengths.begin() ; length != end; length++) {
        maxv = qMax(maxv, *length);
        minv = qMin(minv, *length);
    }

    // Ok, if the maxv is at least 2x the minv then they're different lengths in general.
    if (maxv > minv*2.0) {
        aveLength = minv + (maxv - minv)/2;
    } else {
        // the lengths are the same, so either they're all dits or they're all dahs
        if (maxv < pauseLength)
            aveLength = maxv+1; // they're all dits
        else
            aveLength = minv-1; // they're all dahs
    }

    // Ok, now loop through them all and determine what each is.
    for(length =  m_keyedLengths.begin() ; length != end; length++) {
        if (*length > aveLength) {
            // it's a DAH
            ditDahs.push_back(Morse::DAH);
        } else {
            ditDahs.push_back(Morse::DAH);
        }
    }

    // Now we have the list, we can get the char from the sequence
}
