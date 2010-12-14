#include "MCountGameMode.h"

MCountGameMode::MCountGameMode(int maxTurns)
  : MGameMode(), m_turnNumber(0), m_maxTurns(maxTurns)
{
}

void MCountGameMode::startGame() 
{
  m_turnNumber = 0;
  MGameMode::startGame();
}

void MCountGameMode::nextTurn(int scoreAddition) 
{
  m_turnNumber++;
  MGameMode::nextTurn(scoreAddition);

  if (m_turnNumber >= m_maxTurns) {
    emit gameOver();
  }   
}
