#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <QtCore/QObject>

class GameMode : public QObject
{
public:
    Q_OBJECT
  
public:
  MCountGameMode(int maxTurns = 25);

  virtual void startGame();

  virtual void nextTurn(int scoreAddition);

 protected:
  int   m_maxTurns;
  int   m_turnNumber;
};

#endif
