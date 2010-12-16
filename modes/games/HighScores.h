#ifndef HIGHSCORES_H
#define HIGHSCORES_H

class HighScores : public QObject
{
    Q_OBJECT
public:
  explicit HighScores(const QString &tableName, int maxCount = 20);

  virtual int addScore(const QString &name, int score);

public slots:
  virtual void displayScores();

private:
  QString                          m_tableName;
  int                              m_maxScores;
  QLinkedList<QPair<int,QString>>  scores;
};

#endif // HIGHSCORES_H
