#include "LetterTrainingMode.h"
#include "Morse.h"

LetterTrainingMode::LetterTrainingMode(Morse *parent, Ui::MainWindow *ui)
    : TrainingMode(parent, ui)
{
}

void LetterTrainingMode::switchToMode() {
    m_ui->letter->show();
    m_ui->play->show();
    setupTrainingWidgets();
    m_ui->changeSequence->show();
    m_ui->helpBar->setText("<font color=\"green\">Type the letter you hear ASAP.</font>");
    clear();

    setupSequences();
    setupWidgets(m_trainingSequence);
}

void LetterTrainingMode::handleKeyPress(QChar letterPressed) {
    if (runningMode() != RUNNING)
        return;

    // analyze they're keyed letter and immediately start playing a new one
    TrainingMode::handleKeyPress(letterPressed);
    startNextTrainingKey();
}

QString LetterTrainingMode::helpText()
{
    return tr("In this mode characters will be sent and you need to press the appropriate key before a minimum time period, defined by the training speed preference.  "
              "As you get fast enough for particular characters then the next character in the sequence will be added.  Characters that you need the most work on will be "
              "sent more frequently than ones you're fast at responding to.");
}

QString LetterTrainingMode::name()
{
    return tr("Recognition Training");
}
