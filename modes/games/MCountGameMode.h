#ifndef GAMECOUNTMODE_H
#define GAMECOUNTMODE_H

#include <QtCore/QObject>
#include "MGameMode.h"

class MCountGameMode : public MGameMode
{
  
public:
  MCountGameMode(int maxTurns = 20);

  virtual void startGame();

  virtual void nextTurn(int scoreAddition);

  virtual int turn() { return m_turnNumber; }
  virtual int maxTurns() { return m_maxTurns; }

 protected:
  int   m_turnNumber;
  int   m_maxTurns;
};

#endif
