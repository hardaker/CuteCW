#include "PlayMode.h"

PlayMode::PlayMode(Morse *parent)
    : MorseMode(parent)
{
}

void PlayMode::handleKeyPress(QChar letterPressed)
{
    m_morse->addAndPlayIt(letterPressed);
}

void PlayMode::switchToMode(Ui::MainWindow *m_ui)
{
    m_ui->wordbox->hide();
    m_ui->letter->hide();
    m_ui->clearTraining->hide();
    m_ui->readButton->hide();
    m_ui->changeSequence->hide();
    m_ui->changeWords->hide();
    m_ui->modeMenu->setText("Play Morse Code");
    m_ui->helpBar->setText("<font color=\"green\">Type letters to hear the keys in morse code</font>");
    m_ui->play->hide();
    m_ui->WPM->hide();
}
