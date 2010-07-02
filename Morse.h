#ifndef MORSE_H
#define MORSE_H

#include <QtCore/QString>
#include <QtMultimedia/QAudioOutput>
#include <QtCore/QMap>
#include <QtCore/QList>

#include "Generator.h"

class Morse : QObject
{
    Q_OBJECT

public:
    Morse();
    Morse(QAudioOutput *output);

    enum ditdah{ DIT, DAH };

    void setSequence(const QString &sequence);
    void clearList();
    void add(Generator *nextsound);
    void createTones(float ditSecs, int dahMult = 3, int pauseMult = 3, int spaceMult = 6);

    Generator *dit();
    Generator *dah();
    Generator *pause();
    Generator *space();

public slots:
    void playSequence();
    void nextSequence(QAudio::State state);

private:
    QAudioOutput *m_audioOutput;
    Generator      *m_dit, *m_dah, *m_space, *m_pause;
    QList<Generator *> m_gens;
    QList<Generator *>::iterator m_listPos;
    QMap<QChar, QList<ditdah> *> code;
};

#endif // MORSE_H
