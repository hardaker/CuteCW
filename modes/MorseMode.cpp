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
    qDebug() << "playButton()";

    // if the current mode is stopped, switcth to play
    if (m_morse->audioMode() == Morse::STOPPED) {
        m_morse->setAudioMode(Morse::STOPPED);
        m_ui->play->setText("Pause");

        play();

    // if the current mode is playing, stop it
    } else { // PLAYING or STOPPED
        m_morse->setAudioMode(Morse::STOPPED);
        m_ui->play->setText("Go");

        stop();
    }
}

void MorseMode::play()
{
}

void MorseMode::pause()
{
}

void MorseMode::stop()
{
}

bool MorseMode::enterPressed() {
    return false;
}

void MorseMode::handleKeyPress(QChar letterPressed) {
    Q_UNUSED(letterPressed);
}

void MorseMode::audioFinished(QAudio::State state) {
    if (state != QAudio::IdleState && state != QAudio::StoppedState)
        return;

    qDebug() << "audio state changed: " << state << ", old state = " << m_morse->audioMode();

    if (m_morse->audioMode() != Morse::STOPPED) {
        audioStopped();
    }
    m_morse->setAudioMode(Morse::STOPPED);
}

void MorseMode::audioStopped() {
}

void MorseMode::clear() {
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
