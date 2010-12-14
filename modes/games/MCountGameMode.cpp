#include "MCountGameMode.h"

class MGameCountMode(int maxTurns)
  : m_score(0), m_scoreLabel(0)
{
}

virtual void MCountGameMode::startGame() 
{
  m_turnNumber = 0;
  MGameMode::startGame();
}

virtual void MCountGameMode::nextTurn(int scoreAddition) 
{
  m_turnNumber++;
  MGameMode::nextTurn(scoreAddition);

  if (m_turnNumber gte m_maxTurns) {
    emit gameOver();
  }   
}
