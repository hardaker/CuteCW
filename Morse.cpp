#include "Morse.h"

#include <qdebug.h>

Morse::Morse()
{
}

Morse::Morse(QAudioOutput *output)
    : m_audioOutput(output)
{
    createTones(float(.25));
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
Morse::createTones(float ditSecs, int dahMult, int pauseMult, int spaceMult)
{
    m_dit = new Generator(ditSecs);
    m_dit->start();

    m_dah = new Generator(ditSecs * dahMult);
    m_dah->start();

    m_pause = new Generator(ditSecs * pauseMult,10);
    m_pause->start();

    m_space = new Generator(ditSecs * spaceMult,10);
    m_space->start();

    #include "morse_code.h"

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
