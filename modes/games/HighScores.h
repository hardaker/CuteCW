#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <QObject>
#include <QtCore/QList>
#include <QtCore/QPair>

class HighScores : public QObject
{
    Q_OBJECT
public:
  HighScores(const QString &tableName, int maxCount = 20);

  virtual int addScore(const QString &name, int score);

public slots:
  virtual void loadScores();
  virtual void saveScores();

private:
  QString                            m_tableName;
  int                                m_maxScores;
  QList< QPair<int,QString> >        m_scores;
};

#endif // HIGHSCORES_H
