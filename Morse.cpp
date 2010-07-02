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
