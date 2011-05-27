#ifndef MSEQUENCES_H
#define MSEQUENCES_H

#include <QtCore/QStringList>

const QString completeCharacterSet = "abcdefghijklmnopqrstuvwxyz1234567890.,/=?";

class MSequences
{
public:
    MSequences();

    enum sequences { KOCH, KOCH1, KOCH2, KOCH3, KOCH4,
                     ALPHABET, AF, GM, NT, UZ,
                     NUMBERS, SYMBOLS,
                     SIM1, SIM2, SIM3, SIM4, SIM5,
                     EVERYTHING};
protected:
    QStringList                     m_sequences;
};

#endif // MSEQUENCES_H
