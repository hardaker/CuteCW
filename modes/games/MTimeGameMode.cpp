#include "MTimeGameMode.h"

#include <qdebug.h>

#include <QtCore/QTimer>

MTimeGameMode::MTimeGameMode(int gameLength)
  : MGameMode(), m_gameLength(gameLength)
{
}

void MTimeGameMode::startGame() 
{
  MGameMode::startGame();
  QTimer::singleShot(m_gameLength, dynamic_cast<QObject *>(this), SLOT(timedGameOver()));
}

void MTimeGameMode::timedGameOver() {
    gameOver();
}
