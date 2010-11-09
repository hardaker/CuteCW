#include "LetterTrainingMode.h"
#include "Morse.h"

LetterTrainingMode::LetterTrainingMode(Morse *parent, Ui::MainWindow *ui)
    : TrainingMode(parent, ui)
{
}

void LetterTrainingMode::switchToMode() {
    hideWidgets();
    m_ui->letter->show();
    m_ui->clearTraining->show();
    m_ui->modeMenu->setText("Recognition Training");
    m_ui->changeSequence->show();
    m_ui->helpBar->setText("<font color=\"green\">Type the letter you hear ASAP.</font>");
    clear();

    setupSequences();

    m_morse->setAudioMode(Morse::PLAYING);
    playButton(); // will change to "paused"
}

void LetterTrainingMode::handleKeyPress(QChar letterPressed) {
    // ensure we're not still playing a sound:
    if (m_morse->audioMode() == Morse::PLAYING)
        return;
    // analyze they're keyed letter and immediately start playing a new one
    TrainingMode::handleKeyPress(letterPressed);
    startNextTrainingKey();
}
