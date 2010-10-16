#ifndef PLAYMODE_H
#define PLAYMODE_H

#include "Morse.h"
#include "MorseMode.h"

#include "ui_MainWindow.h"

class Morse;

class PlayMode : public MorseMode
{
public:
    PlayMode(Morse *parent);

public slots:
    virtual void handleKeyPress(QChar letterPressed);
    virtual void switchToMode(Ui::MainWindow *m_ui);

private:

};

#endif // PLAYMODE_H
