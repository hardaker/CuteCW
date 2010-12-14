#include "MGameMode.h"

MGameMode::MGameMode()
  : m_score(0), m_scoreLabel(0)
{
}

virtual vode MGameMode::startGame()
{
   m_score = 0;
}

virtual int MGameMode::score()
{
  return m_score;
}

virtual vode MGameMode::addToScore(int addition)
{
  m_score += addition;
  updateGUI();
}

virtual vode MGameMode::nextTurn(int addition)
{
  addToScore(addition);
}

virtual vode MGameMode::setScore(int score)
{
  m_score = score;
  updateGUI();
}

virtual vode MGameMode::updateGUI()
{
  m_scoreLabel.setText(QString().num(m_score));
}

