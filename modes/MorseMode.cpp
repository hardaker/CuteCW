#include "MorseMode.h"
#include "Morse.h"
#include <qdebug.h>

MorseMode::MorseMode(Morse *morse, Ui::MainWindow *ui)
    : m_morse(morse), m_ui(ui),
      m_badCount(0), m_goodCount(0)
{
    m_morse = morse;
}

Morse *MorseMode::morseParent() {
    return m_morse;
}

void MorseMode::playButton() {
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
    qDebug() << "Training response: elapsed " << msElapsed << "ms (" << m_morse->msToPauseWPM(msElapsed) << " WPM)";
    MorseStat *pressedStat = m_morse->getStat(letterPressed);

    // if the user took a *really* long time, ignore the key press and assume they got distracted from training
    if ((pressedStat->getTryCount() > 0 && msElapsed > 5 * pressedStat->getAverageTime()) ||
        (pressedStat->getTryCount() == 0 && msElapsed > 5 * m_morse->msToPauseWPM(m_morse->currentWPMAccept()))) {
        qDebug() << "ignoring key press; too long and probably an interruption";
        return;
    }

    // set the last WPM record on the display
    m_ui->lastwpm->setText(QString().setNum(m_morse->msToPauseWPM(msElapsed)));

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
