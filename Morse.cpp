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
    qsrand(QTime::currentTime().msec());
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
    QChar newletter = newtext.at(newtext.length()-1);

    if (m_gameMode == PLAY) {
        addAndPlayIt(newletter);
    } else if (m_gameMode == TRAIN) {
        getStat(newletter)->addTime(m_lastTime.elapsed());
    }
}

int Morse::msToWPM(float ms) {
    return (60*1000)/ms; // XXX: fix me; doesn't include keying times
}

void Morse::startNextTrainingKey() {
    int letterCount = 0;
    QList<QPair<QChar, float> > letters;
    float totalTime = 0.0, thisTime;

    QString::iterator letter;
    QString::iterator lastLetter = m_trainingSequence.end();
    for(letter = m_trainingSequence.begin(); letter != lastLetter; ++letter) {
        letterCount++;
        MorseStat *stat = getStat(*letter);
        thisTime = stat->getAverageTime();
        totalTime += thisTime;
        if (thisTime == 0) {
            // never keyed yet; do it immediately if we got this far
            setStatus(tr("Starting a new letter: %s").arg(QString(*letter)));
            addAndPlayIt(*letter);
            return;
        } else if(msToWPM(thisTime) < m_currentWPMGoal) {
            // we're not fast enough; break here
            break;
        }
        letters.append(QPair<QChar, float>(*letter, thisTime));
    }

    // now pick a random time between 0 and the total of all the averages; averages with a slower speed are more likely
    // XXX: probably could use a weighted average (subtract off min speed from all speeds)?
    float randTime = float(qrand())/float(RAND_MAX);
    float newTotal = 0;
    QList<QPair<QChar, float> >::iterator search;
    QList<QPair<QChar, float> >::iterator last = letters.end();
    for(search = letters.begin(); search != last; ++search) {
        newTotal += (*search).second;
        if (newTotal > randTime) {
            qDebug() << "keying: " << (*search).first;
            addAndPlayIt((*search).first);
        }
    }
}

MorseStat *Morse::getStat(const QChar &key) {
    if (! m_stats.contains(key))
        m_stats[key] = new MorseStat(0);
    return m_stats[key];
}

void
Morse::audioFinished(QAudio::State state)
{
    if (state != QAudio::IdleState && state != QAudio::StoppedState)
        return;
    m_lastTime = QTime::currentTime();
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
        add(letterPause());
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
    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audioFinished(QAudio::State)));
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
