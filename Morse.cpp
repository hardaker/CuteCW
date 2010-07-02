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
    m_dit = new Generator();
    qDebug() << "here";
    m_dit->start();
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
