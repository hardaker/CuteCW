#include <qdebug.h>

#include "modes/games/WordGame.h"
#include "modes/QModeStart.h"

WordGame::WordGame(Morse *parent, Ui::MainWindow *ui) :
  WordTrainingMode(parent, ui), MCountGameMode(), m_scores("Word Accuracy Game")
{
    connect(this, SIGNAL(groupEntered(int, int)),
            this, SLOT(groupGuessed(int, int)));
}

void WordGame::switchToMode() {
    WordTrainingMode::switchToMode();
    m_ui->clearTraining->hide();
    m_ui->modeMenu->setText(tr("Word Game"));
    m_ui->helpBar->setText(tr("<font color=\"green\">Type the groups of characters you hear ASAP.</font>"));
    m_ui->changeSequence->hide();
}

void WordGame::play()
{
    startGame();

    QModeStart startInfo(0, tr("Word Training Game"));
    QVBoxLayout *layout = startInfo.mainLayout();

    QFormLayout form;
    layout->addLayout(&form);

    QSpinBox WPM;
    WPM.setValue(m_WPM);
    form.addRow(tr("Starting WPM:"), &WPM);

    if (startInfo.exec() == QDialog::Accepted) {
        m_WPM = WPM.value();
        m_morse->createTones(m_WPM);
        m_maxWord = (*(words[m_wordsNumber])).count();
        enterPressed();
    }
}

void WordGame::gameOver()
{
  playButton();
  m_scores.addScore("", score());
}

void WordGame::groupGuessed(int right, int total)
{
    nextTurn(10 * m_WPM * right * ((total==right)?2:1));
    m_maxWord = (*(words[m_wordsNumber])).count();
    if (right == total)
        m_WPM++;
    else
        m_WPM--;
    m_morse->createTones(m_WPM);
}

void WordGame::setSequenceText()
{
    m_morse->m_sequenceLabel->setText(tr("WPM=%1, score = %2, turn=%3/%4").arg(m_WPM).arg(score()).arg(turn()).arg(maxTurns()));
    qDebug() << tr("WPM=%1, score = %2, turn=%3/%4").arg(m_WPM).arg(score()).arg(turn()).arg(maxTurns());
}

QString WordGame::helpText()
{
    return tr("Characters in the chosen training sequence will be keyed starting with a single character.  "
              "Type in the characters you hear in the group.  Your results good or bad will be displayed "
              "on the screen in green/red colors indicating good/bad guesses."
              "<p>As you guess more and more groups correctly the number of characters in the group will slowly go upward.");
}
