#include <qdebug.h>

#include "modes/GroupGame.h"

WordGame::WordGame(Morse *parent, Ui::MainWindow *ui) :
  GroupingMode(parent, ui), MCountGameMode(), m_scores("Word Accuracy Game")
{
    connect(this, SIGNAL(groupEntered(int, int)),
            this, SLOT(groupGuessed(int, int)));
    connect(this, SIGNAL(gameOver()),
            this, SLOT(gameOver()));
}

void WordGame::switchToMode() {
    GroupingMode::switchToMode();
    m_ui->clearTraining->hide();
    m_ui->modeMenu->setText(tr("Word Game"));
    m_ui->helpBar->setText(tr("<font color=\"green\">Type the groups of characters you hear ASAP.</font>"));
}

void WordGame::play()
{
    startGame();
    enterPressed();
}

void WordGame::gameOver()
{
  pause();
  m_scores.addScore(QString(), score());
}

void WordGame::groupGuessed(int right, int total)
{
    nextTurn(right*10*((total==right)?2:1));
}

void WordGame::setSequenceText()
{
  m_morse->m_sequenceLabel->setText(tr("current length = %1, good = %2, bad = %3, score = %4").arg(m_groupLength).arg(m_goodGuesses).arg(m_badGuesses)).arg(score());
}

QString WordGame::helpText()
{
    return tr("Characters in the chosen training sequence will be keyed starting with a single character.  "
              "Type in the characters you hear in the group.  Your results good or bad will be displayed "
              "on the screen in green/red colors indicating good/bad guesses."
              "<p>As you guess more and more groups correctly the number of characters in the group will slowly go upward.");
}