#include "modes/GroupingMode.h"

GroupingMode::GroupingMode(Morse *parent, Ui::MainWindow *ui) :
    TrainingMode(parent, ui)
{
}

void GroupingMode::switchToMode() {
    m_ui->letter->show();
    m_ui->play->show();
    m_ui->clearTraining->show();
    m_ui->modeMenu->setText(tr("Grouping Training");
    m_ui->changeSequence->show();
    m_ui->helpBar->setText(tr("<font color=\"green\">Type the groups of characters you hear ASAP.</font>"));
    clear();

    setupSequences();
    setupWidgets(m_trainingSequence);
}

void GroupingMode::handleKeyPress(QChar letterPressed) {
    if (runningMode() != RUNNING)
        return;

}
