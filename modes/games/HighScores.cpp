#include "HighScores.h"

#include <QtCore/QSettings>

#include "HighScoresDialog.h"

HighScores::HighScores(const QString &tableName, int maxCount = 20)
  : m_tableName(tableName), m_maxScores(maxCount), m_scores()
{
  loadScores();
}

int HighScores::addScore(const QString &name, int score)
{
    int count = -1;
    end = m_scores.end();
    for(begin = m_scores.begin(),
	  begin != end,
	  begin++)
      {
        count++;
	if ((*begin).first < score) {
	  m_scores.insert(QPair<int,QString>(score, name));
          if (m_scores.count() > m_maxScores)
	    m_scores.pop_back();
          if (!name.isValid()) {
              QHighScoreDialog hsdialog(m_tableName, &m_scores, count);
              hsdialog.exec();
          }
          saveScores();
          return count-1;
        }
      }
}

int HighScores::saveScores()
{
    QSettings settings("WS6Z", "qtcw");
    QString savename("highscores/" + m_tableName);
    int count = 0;

    settings.beginWriteArray(savename);

    end = m_scores.end();
    for(begin = m_scores.begin(),
	  begin != end,
	  begin++) {
      settings.setValue("score", (*begin).first);        settings.setArrayIndex(count);
        settings.setValue("score", (*begin).first);
        settings.setValue("name", (*begin).second);
        count++
    }
    settings.endArray();
}

int HighScores::loadScores()
{
    QSettings settings("WS6Z", "qtcw");
    QString savename("highscores/" + m_tableName);
    int count = 0;

    int size = settings.beginReadArray(savename);
    m_scores.clear();
    for(int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
	m_scores.push_back(QPair<int,Qstring>(settings.value("score").toInt(), settings.value("name").toString()));
					      
    }
    settings.endArray();
}
