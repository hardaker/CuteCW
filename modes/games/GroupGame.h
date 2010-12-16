#ifndef GROUPGAMEMODE_H
#define GROUPGAMEMODE_H

#include "GroupingMode.h"
#include "McountGameMode.h"
#include "Morse.h"

class GroupGame : public GroupingMode, public MCountGameMode
{
    Q_OBJECT
public:
    explicit GroupGame(Morse *parent, Ui::MainWindow *ui);

    virtual void switchToMode();
    virtual void handleKeyPress(QChar letter);

    virtual QString helpText();

 signals:

public slots:
    void gameOver();
    void groupGuessed(int right, int total);
    virtual void play();

private:
};

#endif // GROUPGAMEMODE_H
