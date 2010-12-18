#ifndef HIGHSCORESDIALOG_H
#define HIGHSCORESDIALOG_H

#include <QtGui/QDialog>

class HighScoresDialog : public QDialog
{
    Q_OBJECT
public:
  HighScoresDialog(const QString &tableName, QLinkedList<QPair<int,QString>>  *scores, int newSlot = -1);

public slots:
  virtual void displayScores();
  void textChanged(QString &text);
  void setScoreTable();


private:
  int m_newSlot;
  QLinkedList<QPair<int,QString>>  *m_scores;

    QTextEdit *scoreTable;
    QLineEdit *nameEntry;
    
};

#endif // HIGHSCORES_H
