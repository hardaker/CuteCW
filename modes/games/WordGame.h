#ifndef WORDGAMEMODE_H
#define WORDGAMEMODE_H

#include "WordMode.h"
#include "McountGameMode.h"
#include "Morse.h"

class WordGame : public WordGame, public MCountGameMode
{
    Q_OBJECT
public:
    explicit WordGame(Morse *parent, Ui::MainWindow *ui);

    virtual void switchToMode();
    virtual void handleKeyPress(QChar letter);

    virtual QString helpText();
    virtual void setSeqenceText();

 signals:

public slots:
    void gameOver();
    void groupGuessed(int right, int total);
    virtual void play();

private:
    HighScores  m_scores;
};

#endif // WORDGAMEMODE_H
