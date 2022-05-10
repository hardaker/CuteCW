#ifndef KEYINVADERS_H
#define KEYINVADERS_H

#include "../MorseMode.h"
#include "MGameMode.h"
#include "HighScores.h"
#include "Invader.h"
#include "../MSequences.h"
#include "KeyingReader.h"

#include <QtCore/QList>
#include <QtCore/QList>
#include <QtCore/QTime>
#include <QGraphicsView>
#include <QtCore/QTimer>
#include <QElapsedTimer>

#include "Invader.h"

class KeyInvaders : public MorseMode, MGameMode, MSequences, KeyingReader
{
    Q_OBJECT

public:
    KeyInvaders(Morse *parent, Ui::MainWindow *main);


    virtual void switchToMode();
    virtual void modeMenus();
    void setupWidgets();
    void scaleWindow();

    // standard functions
    virtual QString helpText();
    virtual QString name();
    virtual QString icon();
    virtual void gameOver();


public slots:
    virtual void handleKeyPress(QChar letterPressed);
    virtual void handleKeyRelease(QChar letterPressed);
    virtual void play();
    virtual void stop();

    void advanceFrame();

private:
    HighScores  m_scores;
    int m_keyCount;
    QElapsedTimer m_startTime;
    QList<int> m_keyedTimes;
    QGraphicsView *m_graph;

    QTimer *invadingTimer;

    QList<Invader *> invaders;
    QGraphicsScene *m_scene;

    int addCount;
    int maxInvaderY;
};

#endif // KEYINVADERS_H
