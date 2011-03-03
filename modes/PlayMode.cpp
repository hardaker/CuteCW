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
    m_ui->helpBar->setText("<font color=\"green\">Type letters to hear the keys in morse code</font>");
    clear();
    setupWPMWidgets();
}

QString PlayMode::helpText()
{
    return tr("<p>In </i>Type Morse Code</i> mode, simply type keys and the Cute CW engine will play what you type at the speeds set in the preferences dialog box.  "
              "This is a fun mode to simply hear code that you type, but to begin your true training, switch to one of the other modes."
              "<p>You may also wish to try the <i>read to me</i> mode as well.");
}

QString PlayMode::name()
{
    return tr("Type Morse Code");
}

QString PlayMode::icon()
{
    return ":/icons/64x64/type.png";
}
