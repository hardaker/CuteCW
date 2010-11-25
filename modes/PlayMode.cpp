#include "PlayMode.h"

PlayMode::PlayMode(Morse *parent, Ui::MainWindow *ui)
    : MorseMode(parent, ui)
{
}

void PlayMode::handleKeyPress(QChar letterPressed)
{
    m_morse->addAndPlayIt(letterPressed);
}

void PlayMode::switchToMode()
{
    m_ui->modeMenu->setText("Play Morse Code");
    m_ui->helpBar->setText("<font color=\"green\">Type letters to hear the keys in morse code</font>");
    clear();
}
