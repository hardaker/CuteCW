#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <QtCore/QObject>

class GameMode : public QObject
{
public:
    Q_OBJECT

public:
  MGameMode();

  virtual void startGame();

  virtual int score();
  virtual void addToScore(int addition);
  virtual void setScore();
  virtual void nextTurn(int scoreAddition);

  virtual void updateGUI();

 signals:
  void gameOver();

 protected:
  int       m_score;
  QLabel   *m_scoreLabel;
};

#endif
