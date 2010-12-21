#ifndef GAMECOUNTMODE_H
#define GAMECOUNTMODE_H

#include <QtCore/QObject>
#include "MGameMode.h"

class MCountGameMode : public MGameMode
{
  
public:
  MCountGameMode(int maxTurns = 1);

  virtual void startGame();

  virtual void nextTurn(int scoreAddition);

 protected:
  int   m_turnNumber;
  int   m_maxTurns;
};

#endif
