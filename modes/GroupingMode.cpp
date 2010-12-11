#include <qdebug.h>

#include "modes/GroupingMode.h"

GroupingMode::GroupingMode(Morse *parent, Ui::MainWindow *ui) :
    TrainingMode(parent, ui)
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

    if (m_currentSequence.length() == m_enteredWord.length()) // they already hit the length previously
        return;

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
            m_goodGuesses++;
            m_ui->letter->setText(m_ui->letter->text() + " - <font color=\"green\">GOOD</font>");
        } else {
            m_badGuesses++;
            m_ui->letter->setText(m_ui->letter->text() + " - <font color=\"red\">FAIL (" + m_currentSequence + ")</font>");
        }
    }
    setSequenceText();
}

void GroupingMode::setSequenceText() {
    m_morse->m_sequenceLabel->setText(QString("current length = %1, good = %2, bad = %3").arg(m_groupLength).arg(m_goodGuesses).arg(m_badGuesses));
}

void GroupingMode::createNextSequence() {
    m_currentSequence = QString();

    m_groupLength = DEFAULT_GROUPLENGTH + ((m_goodGuesses - m_badGuesses)/GROUPLENGTH_WEIGHT);
    if (m_groupLength < 1)
        m_groupLength = 1;

    float m_trainingLength = float(m_trainingSequence.length());
    for(int i = 0 ; i < m_groupLength; i++) {
        m_currentSequence += m_trainingSequence[int(float(m_trainingLength) * float(qrand())/float(RAND_MAX))];
    }
}

void GroupingMode::startNextGroup()
{
    createNextSequence();
    m_morse->add(" " + m_currentSequence);
    m_morse->maybePlaySequence(true);
    m_wordWasGood = true;
    m_enteredWord = "";
    setSequenceText();
    m_ui->letter->setText(QString().number(m_groupLength) + ": ");
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

void GroupingMode::clear()
{
    m_goodGuesses = 0;
    m_badGuesses = 0;
}

QString GroupingMode::helpText()
{
    return tr("Characters in the chosen training sequence will be keyed starting with a single character.  "
              "Type in the characters you hear in the group.  Your results good or bad will be displayed "
              "on the screen in green/red colors indicating good/bad guesses."
              "<p>As you guess more and more groups correctly the number of characters in the group will slowly go upward.");
}

