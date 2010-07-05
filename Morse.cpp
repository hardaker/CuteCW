#include "Morse.h"

#include <qdebug.h>

Morse::Morse()
    : QObject(), m_audioOutput(), m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playingMode(STOPPED), m_gameMode(PLAY), m_statusBar(0)
{
}

Morse::Morse(QAudioOutput *output, QLabel *statusBar)
    : QObject(), m_audioOutput(output), m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playingMode(STOPPED), m_gameMode(PLAY), m_statusBar(statusBar)
{
    createTones(float(.1));
    setStatus("ready: Play Mode");
}

void
Morse::playSequence()
{
    m_playBuffer->restartData();
    m_playBuffer->start();
    qDebug() << "left: " << m_playBuffer->bytes_left;
    m_playingMode = PLAYING;
    m_audioOutput->start(m_playBuffer);
    return;
}

void Morse::maybePlaySequence() {
    if (m_playingMode == STOPPED) {
        playSequence();
    }
}

void Morse::addAndPlayIt(QChar c) {
    if (m_playingMode == STOPPED)
        clearList();
    add(c);
    maybePlaySequence();
}

void Morse::keyPressed(QString newtext) {
    if (m_gameMode == PLAY) {
        QChar newletter = newtext.at(newtext.length()-1);
        addAndPlayIt(newletter);
    }
}

void
Morse::nextSequence(QAudio::State state)
{
    qDebug() << "got here: " << state;
    if (state != QAudio::IdleState && state != QAudio::StoppedState)
        return;
    m_playingMode = STOPPED;
}

void
Morse::setSequence(const QString &sequence)
{
   // m_sequence = sequence;
}

void
Morse::clearList()
{
    m_playBuffer->clearBuffer();
}

void
Morse::add(Generator *nextSound)
{
    m_playBuffer->appendDataFrom(nextSound);
}

void Morse::switchMode(int newmode) {
    m_gameMode = (Morse::mode) newmode;
    qDebug() << "switch to:" << m_gameMode;
}

void
Morse::add(QChar c, bool addpause)
{
    QList<ditdah>::iterator iter;
    QList<ditdah>::iterator endat = code[c]->end();

    for(iter = code[c]->begin(); iter != endat; iter++)
    {
        switch (*iter) {
        case DIT:
            add(dit());
            break;
        case DAH:
            add(dah());
            break;
        default:
            qWarning() << "error: illegal morse type added";
        }
    }
    if (addpause) {
        add(pause());
    }
}

void
Morse::createTones(float ditSecs, int dahMult, int pauseMult, int letterPauseMult, int spaceMult)
{
    m_dit = new Generator(ditSecs);
    m_dit->start();

    m_dah = new Generator(ditSecs * dahMult);
    m_dah->start();

    m_pause = new Generator(ditSecs * pauseMult, 10);
    m_pause->start();

    m_letterPause = new Generator(ditSecs * letterPauseMult, 10);
    m_letterPause->start();

    m_space = new Generator(ditSecs * spaceMult, 10);
    m_space->start();

    m_playBuffer = new Generator(m_pause);
    m_playBuffer->start();

    #include "morse_code.h"

    qDebug() << "created tones";
    //connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(nextSequence(QAudio::State)));
}

void Morse::setStatus(const QString &status) {
    if (m_statusBar)
        m_statusBar->setText(status);
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
Morse::letterPause()
{
    return m_letterPause;
}

Generator *
Morse::space()
{
    return m_space;
}
