#include "Morse.h"

Morse::Morse()
{
}

Morse::Morse(const QString &sequence, QAudioOutput *output)
    : m_sequence(sequence),
      m_output(output)
{
}

void
Morse::playSequence()
{
}

void
Morse::setSequence(const QString &sequence)
{
    m_sequence = sequence;
}

void
Morse::clearList()
{
    m_gens.clear();
}

void
Morse::appendList(Generator *nextSound)
{
    m_gens.append(nextSound);
}
