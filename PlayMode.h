#ifndef PLAYMODE_H
#define PLAYMODE_H

#include "Morse.h"
#include "MorseMode.h"

class Morse;

class PlayMode : public MorseMode
{
public:
    PlayMode(Morse *parent);

public slots:
    virtual void handleKeyPress(QChar letterPressed);
    virtual void switchToMode();

private:
    Morse   *m_morse;

};

#endif // PLAYMODE_H
