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
    m_listPos = m_gens.begin();
    nextSequence(QAudio::IdleState);
}

void
Morse::nextSequence(QAudio::State state)
{
    qDebug() << "got here: " << state;
    if (state != QAudio::IdleState && state != QAudio::StoppedState)
        return;

    if (m_listPos != m_gens.end()) {
        (*m_listPos)->restartData();
        m_audioOutput->start(*m_listPos);
        m_listPos++;
    } else {
        m_audioOutput->stop();
    }
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

    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(nextSequence(QAudio::State)));
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
