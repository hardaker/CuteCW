#include <qdebug.h>

#include "modes/GroupingMode.h"

GroupingMode::GroupingMode(Morse *parent, Ui::MainWindow *ui) :
    TrainingMode(parent, ui), m_groupLength(DEFAULT_GROUPLENGTH)
{
    createNextSequence();
}

void GroupingMode::switchToMode() {
    m_ui->letter->show();
    m_ui->play->show();
    m_ui->clearTraining->show();
    m_ui->modeMenu->setText(tr("Grouping Training"));
    m_ui->changeSequence->show();
    m_ui->helpBar->setText(tr("<font color=\"green\">Type the groups of characters you hear ASAP.</font>"));
    clear();

    setupSequences();
    setupWidgets(m_trainingSequence);
}

void GroupingMode::handleKeyPress(QChar letter) {
    if (runningMode() != RUNNING)
        return;

    if (letter == '\r') {
        enterPressed();
        return;
    }

    // Copied from WordTraining (ish)
    if (m_currentSequence[m_enteredWord.length()] == letter) {
        m_ui->letter->setText(m_ui->letter->text() + "<font color=\"green\">" + letter + "<font>");
    } else {
        m_ui->letter->setText(m_ui->letter->text() + "<font color=\"red\">" + letter + "<font>");
        m_wordWasGood = false;
    }
    m_enteredWord.append(letter);
    if (m_currentSequence.length() == m_enteredWord.length()) {
        if (m_wordWasGood) {
            m_ui->letter->setText(m_ui->letter->text() + " - <font color=\"green\">GOOD</font>");
        } else {
            m_ui->letter->setText(m_ui->letter->text() + " - <font color=\"red\">FAIL (" + m_currentSequence + ")</font>");
        }
    }

}

void GroupingMode::createNextSequence() {
    m_currentSequence = QString();
    float m_trainingLength = float(m_trainingSequence.length());
    for(int i = 0 ; i < m_groupLength; i++) {
        m_currentSequence += m_trainingSequence[int(float(m_trainingLength) * float(qrand())/float(RAND_MAX))];
    }
}

void GroupingMode::startNextGroup()
{
    createNextSequence();
    m_morse->add(m_currentSequence);
    m_morse->maybePlaySequence();
    m_ui->letter->setText("");
    m_wordWasGood = true;
    m_enteredWord = "";
}

void GroupingMode::play()
{
    startNextGroup();
}

bool GroupingMode::enterPressed()
{
    qDebug() << "enter";
    startNextGroup();
    return true;
}

