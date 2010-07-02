#ifndef MORSE_H
#define MORSE_H

#include <QtCore/QString>
#include <QtMultimedia/QAudioOutput>

#include "Generator.h"

class Morse
{
public:
    Morse();
    Morse(QAudioOutput *output);

    void setSequence(const QString &sequence);
    void clearList();
    void add(Generator *nextsound);
    void createTones();

    Generator *dit();
    Generator *dah();
    Generator *pause();
    Generator *space();

public slots:
    void playSequence();

private:
    QAudioOutput *m_audioOutput;
    Generator      *m_dit, *m_dah, *m_space, *m_pause;
    QList<Generator *> m_gens;
    QList<Generator *>::iterator m_listPos;
};

#endif // MORSE_H
