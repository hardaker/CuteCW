#include "Morse.h"

#include <qdebug.h>

#define WPMGOAL 20

Morse::Morse()
    : QObject(), m_audioOutput(), m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playingMode(STOPPED), m_gameMode(PLAY), m_statusBar(0),
    m_currentWPMGoal(WPMGOAL), m_trainingSequence(KOCH_GROUP), m_sequenceLabel(0)
{
}

Morse::Morse(QAudioOutput *output, QLabel *statusBar, QLabel *sequence)
    : QObject(), m_audioOutput(output), m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playingMode(STOPPED), m_gameMode(PLAY), m_statusBar(statusBar),
    m_currentWPMGoal(WPMGOAL), m_trainingSequence(KOCH_GROUP), m_sequenceLabel(sequence)
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
    if (m_playingMode == STOPPED) {
        clearList();
        add(pause());
    }
    add(c, false);
    maybePlaySequence();
}

void Morse::keyPressed(QString newtext) {
    QChar newletter = newtext.at(newtext.length()-1);
    qDebug() << "user pressed: " << newletter << "and last key was: " << m_lastKey;

    if (m_gameMode == PLAY) {
        addAndPlayIt(newletter);
    } else if (m_gameMode == TRAIN) {
        qDebug() << "training... " << m_lastTime;
        getStat(m_lastKey)->addTime(m_lastTime.elapsed());
        if (newletter != m_lastKey)
            getStat(newletter)->addTime(2.0 * getStat(m_lastKey)->getAverageTime());
        startNextTrainingKey();
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
    qDebug() << " in next";
    for(letter = m_trainingSequence.begin(); letter != lastLetter; ++letter) {
        letterCount++;
        MorseStat *stat = getStat(*letter);
        thisTime = stat->getAverageTime();
        totalTime += thisTime;
        if (thisTime < 0) {
            // never keyed yet; do it immediately if we got this far
            setStatus("Starting a new letter: " + QString(*letter));
            addAndPlayIt(*letter);
            qDebug() << "------- keying: " << *letter;
            m_lastTime = QTime::currentTime(); // XXX: only added to test on broken linux audio
            qDebug() << "setting last time to " << m_lastTime;
            m_lastKey = *letter;
            setSequence(m_trainingSequence, letterCount);
            return;
        }

        qDebug() << "adding " << *letter << " / " << thisTime << " / " << msToWPM(thisTime);
        letters.append(QPair<QChar, float>(*letter, thisTime));

        if(msToWPM(thisTime) < m_currentWPMGoal) {
            // we're not fast enough; break here
            qDebug() << *letter << " too slow: " << letter << " / " << thisTime << " / " << msToWPM(thisTime);
            break;
        }
    }

    // now pick a random time between 0 and the total of all the averages; averages with a slower speed are more likely
    // XXX: probably could use a weighted average (subtract off min speed from all speeds)?
    float randTime = totalTime*float(qrand())/float(RAND_MAX);
    float newTotal = 0;
    qDebug() << "letter set random: " << randTime;
    QList<QPair<QChar, float> >::iterator search;
    QList<QPair<QChar, float> >::iterator last = letters.end();
    setSequence(m_trainingSequence, letterCount);
    for(search = letters.begin(); search != last; ++search) {
        qDebug() << "  -> " << (*search).first << "/" << (*search).second;
        newTotal += (*search).second;
        if (newTotal > randTime) {
            qDebug() << "------- keying: " << (*search).first;
            addAndPlayIt((*search).first);
            m_lastTime = QTime::currentTime(); // XXX: only added to test on broken linux audio
            m_lastKey = (*search).first;
            return;
        }
    }
    qDebug() << "**** shouldn't get here: " << randTime << "," << totalTime;
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
    qDebug() << "time stopped at" << m_lastTime;
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
    qDebug() << "adding codes for: " << c;

    for(iter = code[c]->begin(); iter != endat; iter++)
    {
        qDebug() << *iter;
        switch (*iter) {
        case DIT:
            add(m_dit);
            break;
        case DAH:
            add(m_dah);
            break;
        default:
            qWarning() << "error: illegal morse type added";
        }
        add(m_pause);
    }
    if (addpause) {
        add(m_letterPause);
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

void Morse::setSequence(const QString &sequence, int currentlyAt) {
    if (m_sequenceLabel) {
        QString left = sequence.left(currentlyAt);
        QString right = sequence.right(sequence.length() - currentlyAt);
        m_sequenceLabel->setText("<font color=\"red\">" + left + "</font> " + right);
    }
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
