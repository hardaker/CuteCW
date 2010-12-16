#include "HighScores.h"

HighScores::HighScores(const QString &tableName, int maxCount = 20)
  : m_tableName(tableName), m_maxScores(maxCount), m_scores()
{
  loadScores();
}

int addScore(const QString &name, int score)
{
    end = m_scores.end();
    for(begin = m_scores.begin(),
	  begin != end,
	  begin++)
      {
	if ((*begin).first lt score) {
	  m_scores.insert(QPair<int,QString>(score, name));
          if (m_scores.count() > m_maxScores)
	    m_scores.pop_back();
          saveScores();
        }
      }
}


