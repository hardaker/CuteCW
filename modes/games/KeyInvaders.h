#ifndef KEYINVADERS_H
#define KEYINVADERS_H

#include "../MorseMode.h"
#include "MGameMode.h"
#include "HighScores.h"

#include <QtCore/QList>
#include <QtCore/QList>
#include <QtCore/QTime>
#include <QtGui/QGraphicsView>

class KeyInvaders : public MorseMode, MGameMode
{
public:
    KeyInvaders(Morse *parent, Ui::MainWindow *main);

    void advanceFrame();

    virtual void switchToMode();
    virtual void modeMenus();
    void setupWidgets();

    // standard functions
    virtual QString helpText();
    virtual QString name();
    virtual QString icon();
    virtual void gameOver();

public slots:
    virtual void handleKeyPress(QChar letterPressed);
    virtual void handleKeyRelease(QChar letterPressed);

private:
    HighScores  m_scores;
    int m_keyCount;
    QTime m_startTime;
    QList<int> m_keyedTimes;
    QGraphicsView *m_graph;
};

#endif // KEYINVADERS_H
