#include "MGameMode.h"

MGameMode::MGameMode()
  : m_score(0), m_scoreLabel(0)
{
}

void MGameMode::startGame()
{
   m_score = 0;
}

int MGameMode::score()
{
  return m_score;
}

void MGameMode::addToScore(int addition)
{
  m_score += addition;
  updateGUI();
}

void MGameMode::nextTurn(int addition)
{
  addToScore(addition);
}

void MGameMode::setScore(int score)
{
  m_score = score;
  updateGUI();
}

void MGameMode::updateGUI()
{
  if (m_scoreLabel)
      m_scoreLabel->setText(QString::number(m_score));
}
