#include "Morse.h"

#include <qdebug.h>

Morse::Morse()
{
}

Morse::Morse(QAudioOutput *output)
    : m_audioOutput(output)
{
    createTones();
}

void
Morse::playSequence()
{
    m_dit->restartData();
    m_audioOutput->start(m_dit);
    m_pause->restartData();
    m_audioOutput->start(m_pause);
    m_dah->restartData();
    m_audioOutput->start(m_dah);
}

void
Morse::setSequence(const QString &sequence)
{
   // m_sequence = sequence;
}

void
Morse::clearList()
{
    m_gens.clear();
}

void
Morse::add(Generator *nextSound)
{
    m_gens.append(nextSound);
}

void
Morse::createTones()
{
    m_dit = new Generator(.25);
    m_dit->start();

    m_dah = new Generator(.75,900);
    m_dah->start();

    m_pause = new Generator(.75,10);
    m_pause->start();

    m_space = new Generator(1.5,10);
    m_space->start();
}


Generator *
Morse::dit()
{
    return m_dit;
}

Generator *
Morse::dah()
{
    return m_dah;
}

Generator *
Morse::pause()
{
    return m_pause;
}

Generator *
Morse::space()
{
    return m_space;
}
