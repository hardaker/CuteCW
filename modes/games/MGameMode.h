#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <QtCore/QObject>
#include <QtGui/QLabel>

class MGameMode
{
public:
  MGameMode();

  virtual void startGame();

  virtual int score();
  virtual void addToScore(int addition);
  virtual void setScore(int score);
  virtual void nextTurn(int scoreAddition);

  virtual void updateGUI();

  virtual void gameOver() = 0;

 protected:
  int       m_score;
  QLabel   *m_scoreLabel;
};

#endif
