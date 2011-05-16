#include "KeyingReader.h"

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
                }
            }
        }
    }

}
