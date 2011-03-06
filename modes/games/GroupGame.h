#ifndef GROUPGAMEMODE_H
#define GROUPGAMEMODE_H

#include <QtGui/QSpinBox>

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
    virtual void modeMenus();

    virtual QString name();
    virtual QString helpText();
    virtual QString icon();

signals:

public slots:
    virtual void gameOver();
    void groupGuessed(int right, int total);
    virtual void play();
    virtual void setSequenceText();
    void limitLength(int newValue);

private:
    HighScores  m_scores;
    int         m_WPM;
    QSpinBox   *m_groupLengthSpinBox;
};

#endif // GROUPGAMEMODE_H
