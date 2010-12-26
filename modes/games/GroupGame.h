#ifndef GROUPGAMEMODE_H
#define GROUPGAMEMODE_H

#include "modes/GroupingMode.h"
#include "modes/games/MCountGameMode.h"
#include "modes/games/HighScores.h"
#include "Morse.h"

class GroupGame : public GroupingMode, public MCountGameMode
{
    Q_OBJECT
public:
    explicit GroupGame(Morse *parent, Ui::MainWindow *ui);

    virtual void switchToMode();

    virtual QString helpText();

signals:

public slots:
    virtual void gameOver();
    void groupGuessed(int right, int total);
    virtual void play();
    virtual void setSequenceText();

private:
    HighScores  m_scores;
    int         m_WPM;
};

#endif // GROUPGAMEMODE_H
