#include "HighScores.h"

#include <QtCore/QSettings>

#include "HighScores.h"
#include "HighScoresDialog.h"

HighScores::HighScores(const QString &tableName, int maxCount)
  : m_tableName(tableName), m_maxScores(maxCount), m_scores()
{
  loadScores();
}

int HighScores::addScore(const QString &name, int score)
{
    int count = -1;
    QList< QPair<int,QString> >::iterator end = m_scores.end();
    for(QList< QPair<int,QString> >::iterator begin = m_scores.begin();
        begin != end;
        begin++) {
        count++;
	if ((*begin).first < score) {
          m_scores.insert(begin, QPair<int,QString>(score, name));
          if (m_scores.count() > m_maxScores)
	    m_scores.pop_back();
          if (name == "") {
              HighScoresDialog hsdialog(m_tableName, &m_scores, count);
              hsdialog.exec();
              hsdialog.saveDefaultName();
          }
          saveScores();
          return count-1;
        }
      }
    if (count < m_maxScores) {
        m_scores.append(QPair<int,QString>(score, name));
        if (name == "") {
            HighScoresDialog hsdialog(m_tableName, &m_scores, count);
            hsdialog.exec();
            hsdialog.saveDefaultName();
        }
        saveScores();
        return count-1;
    }
    return -1;
}

void HighScores::saveScores()
{
    QSettings settings("WS6Z", "qtcw");
    QString savename("highscores/" + m_tableName);
    int count = 0;

    settings.beginWriteArray(savename);

    QList< QPair<int,QString> >::iterator end = m_scores.end();
    for(QList< QPair<int,QString> >::iterator begin = m_scores.begin();
        begin != end;
        begin++) {
        settings.setArrayIndex(count);
        settings.setValue("score", (*begin).first);
        settings.setValue("name", (*begin).second);
        count++;
    }
    settings.endArray();
}

void HighScores::loadScores()
{
    QSettings settings("WS6Z", "qtcw");
    QString savename("highscores/" + m_tableName);

    int size = settings.beginReadArray(savename);
    m_scores.clear();
    for(int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        m_scores.push_back(QPair<int,QString>(settings.value("score").toInt(), settings.value("name").toString()));
					      
    }
    settings.endArray();
}
