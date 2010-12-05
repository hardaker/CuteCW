#include "MorseMode.h"
#include "Morse.h"

#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <qdebug.h>

MorseMode::MorseMode(Morse *morse, Ui::MainWindow *ui)
    : m_morse(morse), m_ui(ui),
      m_badCount(0), m_goodCount(0), m_countWeight(100),
      m_playIcon(":/icons/play.png"), m_pauseIcon(":/icons/pause.png"), m_runningMode(PAUSED),
      m_mapper(new QSignalMapper()), m_buttons(0)
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

void MorseMode::help() {
    QMessageBox::information(0, tr("Mode Help"), tr("This is a test of <b>text</b>."));
}

bool MorseMode::enterPressed() {
    return false;
}

void MorseMode::handleKeyPress(QChar letterPressed) {
    Q_UNUSED(letterPressed);
}

void MorseMode::handleKeyPress(const QString &letterPressed) {
    handleKeyPress(letterPressed[0]);
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
    return (60*1000)/ms;
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
    m_ui->letter->hide();
    m_ui->clearTraining->hide();
    m_ui->modeMenu->setText("Recognition Training");
    m_ui->changeSequence->hide();
    m_ui->changeWords->hide();
    m_ui->helpBar->setText("<font color=\"green\">Type the letter you hear ASAP.</font>");
    m_ui->play->hide();
    m_ui->WPM->hide();
    m_ui->helpBar->hide();
    clearLayout(m_ui->forModes);
    //m_ui->forModes->hide();
}

void MorseMode::switchToYou()
{
    hideWidgets();
    switchToMode();
    createGlobalActions();
}

void MorseMode::createGlobalActions()
{
    // Create the preference items in the quick menu
    QAction *button = m_morse->menuBar()->addAction("Help");
    connect(button, SIGNAL(triggered()), this, SLOT(help()));
}

void MorseMode::clearModeLayout() {
    // remove the mode specific layout objects
    clearLayout(m_ui->forModes);
    m_buttons = 0;
}

void MorseMode::switchFromYou()
{
    // erase the menu and mode layout
    m_morse->menuBar()->clear();
    clearModeLayout();

    // stop the audio
    m_morse->pauseAudio();
    setRunningMode(PAUSED);

    switchFromMode();
}

void MorseMode::switchFromMode()
{
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

void MorseMode::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            qDebug() << "deleting a layout";
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            qDebug() << "deleting a widget";
            delete item->widget();
        }
        //delete item;
    }
}

void MorseMode::setupKeyWidgets(const QString &sequence) {
    qDebug() << "setting up sequence buttons";

    // if we don't have a grid yet, create it
    if (m_buttons) {
        clearLayout(m_buttons);
        delete m_buttons;
        m_buttons = 0;
    }

    m_buttons = new QGridLayout();
    m_ui->forModes->addLayout(m_buttons);

    int column = 0;
    int row = 0;
    const int buttonsPerRow = 11;
    foreach (QChar letter, sequence) {
        QPushButton *button = new QPushButton(QString(letter));
        m_buttons->addWidget(button, row, column++);
        connect(button, SIGNAL(clicked()), m_mapper, SLOT(map()));
        m_mapper->setMapping(button, letter);
        if (column == buttonsPerRow) {
            column = 0;
            row++;
        }

    }
    connect(m_mapper, SIGNAL(mapped(const QString &)),
            this, SLOT(handleKeyPress(const QString &)));
}
