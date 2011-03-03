#ifndef WORDGAMEMODE_H
#define WORDGAMEMODE_H

#include "modes/WordTrainingMode.h"
#include "modes/games/MCountGameMode.h"
#include "modes/games/HighScores.h"
#include "Morse.h"

class WordGame : public WordTrainingMode, public MCountGameMode
{
    Q_OBJECT
public:
    explicit WordGame(Morse *parent, Ui::MainWindow *ui);

    virtual void switchToMode();

    virtual QString name();
    virtual QString helpText();
    virtual QString icon();

signals:

public slots:
    virtual void gameOver();
    void groupGuessed(int right, int total);
    virtual void play();
    virtual void setSequenceText();

private:
    HighScores  m_scores;
};

#endif // WORDGAMEMODE_H
