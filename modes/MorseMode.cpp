#include "MorseMode.h"
#include "Morse.h"
#include <qdebug.h>

MorseMode::MorseMode(Morse *morse, Ui::MainWindow *ui)
    : m_morse(morse), m_ui(ui),
      m_badCount(0), m_goodCount(0), m_countWeight(100)
{
    m_morse = morse;
}

Morse *MorseMode::morseParent() {
    return m_morse;
}

void MorseMode::playButton() {
    if (m_morse->playingMode() == Morse::PAUSED) {
        m_morse->setPlayingMode(Morse::STOPPED);
        m_lastKeys.clear();
        m_lastTimes.clear();
        m_ui->play->setText("Pause");

        play();

    } else { // PLAYING or PAUSED
        m_morse->setPlayingMode(Morse::PAUSED);
        m_ui->play->setText("Go");

        pause();
    }
}

void MorseMode::play()
{
}

void MorseMode::pause()
{
}

bool MorseMode::enterPressed() {
    return false;
}

void MorseMode::handleKeyResponse(QChar letterPressed) {

    qDebug() << "Key pressed = " << letterPressed << ", Queue of stored keys: keys=" << m_lastKeys.count() << ", times=" << m_lastTimes.count();

    // ensure we actually have a stored key in memory
    if (m_lastTimes.count() == 0) {
        qDebug() << "KEY PRESSED EARLY";
        return;
    }

    // pull off the last key from the "keyed" output
    QChar lastKey = m_lastKeys.takeFirst();
    QTime lastTime = m_lastTimes.takeFirst();

    // calculate the time since the keying ended to the time the user hit a key
    // XXX: we need to store a list of times, not just a single time

    int msElapsed = lastTime.elapsed() - m_morse->ditSecs(); // subtract off blank-after time
    if (msElapsed <= 0)
        msElapsed = 1;
    qDebug() << "Training response: elapsed " << msElapsed << "ms (" << msToPauseWPM(msElapsed) << " WPM)";
    MorseStat *pressedStat = m_morse->getStat(letterPressed);

    // if the user took a *really* long time, ignore the key press and assume they got distracted from training
    if ((pressedStat->getTryCount() > 0 && msElapsed > 5 * pressedStat->getAverageTime()) ||
        (pressedStat->getTryCount() == 0 && msElapsed > 5 * msToPauseWPM(m_morse->currentWPMAccept()))) {
        qDebug() << "ignoring key press; too long and probably an interruption";
        return;
    }

    // set the last WPM record on the display
    m_ui->lastwpm->setText(QString().setNum(msToPauseWPM(msElapsed)));

    // if the keyed incorrectly, penalize them 3 times their average else add in the results
    if (letterPressed == lastKey) {
        pressedStat->addTime(msElapsed);
        m_goodCount++;
    } else {
        if (pressedStat->getAverageTime() > 0) /* don't do this unless the letter has been pressed before */
            pressedStat->addTime(3.0 * pressedStat->getAverageTime());
        if (getStat(lastKey)->getAverageTime() > 0) /* don't do this unless the letter has been pressed before */
            getStat(lastKey)->addTime(3.0 * getStat(lastKey)->getAverageTime());
        m_badCount++;
    }
}

MorseStat *MorseMode::getStat(const QChar &key) {
    if (! m_stats.contains(key))
        m_stats[key] = new MorseStat(0);
    return m_stats[key];
}

void MorseMode::audioFinished(QAudio::State state) {
    playButton();

    //qDebug() << "audio state changed: " << state;
    if (state != QAudio::IdleState && state != QAudio::StoppedState)
        return;

    if (m_morse->playingMode() != Morse::STOPPED) {
        m_lastTimes.push_back(QTime::currentTime());
    }
    m_morse->setPlayingMode(Morse::STOPPED);
}

void MorseMode::clear() {
    m_lastKeys.clear();
    m_lastTimes.clear();
}


// Utilities

int MorseMode::msToWPM(float ms) {
    return (60*1000)/ms; // XXX: fix me; doesn't include keying times
}

int MorseMode::msToPauseWPM(float ms) {
    return int(msToPauseWPMF(ms));
}

float MorseMode::msToPauseWPMF(float ms) {
    // 3 dits in length is the pause between letter spacing
    float pauseLength = 3.0 * m_morse->ditSecs();
    // calculate the WPM based on the space it took for the letter to be identified during the pause
    // qDebug() << "pause length: " << pauseLength << ", recorded time: " << ms << ", % = " << (pauseLength * 1000.0 * 100.0 / ms );
    return float(m_morse->currentWPMGoal()) * pauseLength * 1000.0 / ms;
}
