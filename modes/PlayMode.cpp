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
    m_ui->modeMenu->setText("Type Morse Code");
    m_ui->helpBar->setText("<font color=\"green\">Type letters to hear the keys in morse code</font>");
    clear();
    setupWPMWidgets();
}

QString PlayMode::helpText()
{
    return tr("<p>In </i>Play mode</i>, simply type keys and the Cute CW engine will play what you type at the speeds set in the preferences dialog box.  "
              "To begin your listening training, switch to one of the other modes."
              "<p>Genreally you should follow the order in the mode menu.");
}
