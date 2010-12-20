#ifndef HIGHSCORESDIALOG_H
#define HIGHSCORESDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QTextEdit>
#include <QtGui/QLineEdit>

#include <QtCore/QList>
#include <QtCore/QPair>

class HighScoresDialog : public QDialog
{
    Q_OBJECT
public:
  HighScoresDialog(const QString &tableName, QList<QPair<int,QString> >  *scores, int newSlot = -1);

public slots:
  void textChanged(QString &text);
  void setScoreTable();


private:
  QString m_tableName;
  QList<QPair<int,QString> >  *m_scores;
  int m_newSlot;

  QTextEdit *scoreTable;
  QLineEdit *nameEntry;

    
};

#endif // HIGHSCORES_H
