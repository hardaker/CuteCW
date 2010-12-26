#ifndef GAMECOUNTMODE_H
#define GAMECOUNTMODE_H

#include <QtCore/QObject>
#include "MGameMode.h"

class MTimeGameMode : public MGameMode
{
  
public:
  MTimeGameMode(int gameLength = 300);

  virtual void startGame();

public slots:
  virtual void timedGameOver();

 protected:
  int   m_gameLength;
};

#endif
