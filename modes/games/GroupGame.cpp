#include <qdebug.h>

#include "modes/GroupGame.h"

GroupGame::GroupGame(Morse *parent, Ui::MainWindow *ui) :
    GroupingMode(parent, ui), MCountGameMode()
{
    connect(this, SIGNAL(groupEntered(int, int)),
            this, SLOT(groupGuessed(int, int)));
    connect(this, SIGNAL(gameOver()),
            this, SLOT(gameOver()));
}

void GroupGame::switchToMode() {
    GroupingMode::switchToMode();
    m_ui->clearTraining->hide();
    m_ui->modeMenu->setText(tr("Grouping Game"));
    m_ui->helpBar->setText(tr("<font color=\"green\">Type the groups of characters you hear ASAP.</font>"));
}

void GroupGame::play()
{
    startGame();
    startNextGroup();
}

void GroupGame::gameOver()
{
  pause();
  // XXX: calculate high score
}

void GroupGame::groupGuessed(int right, int total)
{
    nextTurn(right*10*((total==right)?2:1));
}


QString GroupMode::helpText()
{
    return tr("Characters in the chosen training sequence will be keyed starting with a single character.  "
              "Type in the characters you hear in the group.  Your results good or bad will be displayed "
              "on the screen in green/red colors indicating good/bad guesses."
              "<p>As you guess more and more groups correctly the number of characters in the group will slowly go upward.");
}

