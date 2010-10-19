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

void MorseMode::handleKeyPress(QChar letterPressed) {
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
