#ifndef MORSE_H
#define MORSE_H

#include <QtCore/QString>
#include <QtMultimedia/QAudioOutput>

#include "Generator.h"

class Morse
{
public:
    Morse();
    Morse(const QString &sequence, QAudioOutput *output);

    void setSequence(const QString &sequence);
    void clearList();
    void appendList(Generator *nextsound);
    void playSequence();

private:
    QString m_sequence;
    QAudioOutput *m_output;
    QList<Generator *> m_gens;
};

#endif // MORSE_H
