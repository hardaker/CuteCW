#include "MorseMode.h"
#include "Morse.h"
#include <qdebug.h>

MorseMode::MorseMode(Morse *morse, Ui::MainWindow *ui)
    : m_morse(morse), m_ui(ui),
      m_badCount(0), m_goodCount(0), m_countWeight(100), m_playIcon(":/icons/play.png"), m_pauseIcon(":/icons/pause.png"), m_runningMode(PAUSED)
{
    setRunningMode(PAUSED);
}

Morse *MorseMode::morseParent() {
    return m_morse;
}

void MorseMode::playButton() {
    qDebug() << "playButton()";

    if (m_runningMode == RUNNING) {
        // stop! (pause, whatever...)
        setRunningMode(PAUSED);
        m_morse->setAudioMode(Morse::STOPPED);
    } else {
        setRunningMode(RUNNING);
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

void MorseMode::hideWidgets()
{
    m_ui->wordbox->hide();
    m_ui->letter->hide();
    m_ui->clearTraining->hide();
    m_ui->modeMenu->setText("Recognition Training");
    m_ui->changeSequence->hide();
    m_ui->changeWords->hide();
    m_ui->helpBar->setText("<font color=\"green\">Type the letter you hear ASAP.</font>");
    m_ui->play->hide();
    m_ui->WPM->hide();
}

void MorseMode::switchToYou()
{
    hideWidgets();

    // remove the mode specific layout objects
    QLayoutItem *child;
    while ((child = m_ui->forModes->takeAt(0)) != 0) {
        delete child;
    }

    m_morse->pauseAudio();
    setRunningMode(PAUSED);
    m_morse->pauseAudio();
    switchToMode();
}

MorseMode::RunningMode MorseMode::runningMode()
{
    return m_runningMode;
}

void MorseMode::setRunningMode(RunningMode newMode)
{
    m_runningMode = newMode;
    if (m_runningMode == RUNNING) {
        m_ui->play->setIcon(m_pauseIcon);
        m_ui->play->setText("Pause");
        play();
    } else {
        if (m_morse->audioMode() != Morse::STOPPED) {
            m_morse->pauseAudio();
        }
        m_ui->play->setIcon(m_playIcon);
        m_ui->play->setText("Play");
        stop();
    }
}

