#include "modes/GroupingMode.h"

GroupingMode::GroupingMode(Morse *parent, Ui::MainWindow *ui) :
    TrainingMode(parent, ui)
{
}

void GroupingMode::switchToMode() {
    m_ui->letter->show();
    m_ui->play->show();
    m_ui->clearTraining->show();
    m_ui->modeMenu->setText("Recognition Training");
    m_ui->changeSequence->show();
    m_ui->helpBar->setText("<font color=\"green\">Type the letter you hear ASAP.</font>");
    clear();

    setupSequences();
    setupWidgets(m_trainingSequence);
}

void GroupingMode::handleKeyPress(QChar letterPressed) {
    if (runningMode() != RUNNING)
        return;
    // ensure we're not still playing a sound:
    if (m_morse->audioMode() == Morse::PLAYING)
        return;
    // analyze they're keyed letter and immediately start playing a new one
    TrainingMode::handleKeyPress(letterPressed);
    startNextTrainingKey();
}
