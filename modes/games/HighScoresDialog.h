#ifndef HIGHSCORESDIALOG_H
#define HIGHSCORESDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>

#include <QtCore/QList>
#include <QtCore/QPair>

class HighScoresDialog : public QDialog
{
    Q_OBJECT
public:
  HighScoresDialog(const QString &tableName, QList<QPair<int,QString> >  *scores, int score = -1, int newSlot = -1);

public slots:
  void textChanged(QString text);
  void setScoreTable();
  void saveDefaultName();

private:
  QString m_tableName;
  QList<QPair<int,QString> >  *m_scores;
  int m_newSlot;

  QTextEdit *scoreTable;
  QLineEdit *nameEntry;
  QString    m_savedName;
    
};

#endif // HIGHSCORES_H
