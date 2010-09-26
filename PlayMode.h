#ifndef PLAYMODE_H
#define PLAYMODE_H

#include "Morse.h"
#include "MorseMode.h"

class Morse;

class PlayMode : public MorseMode
{
public:
    PlayMode(Morse *parent);

private:
    Morse   *m_morse;

};

#endif // PLAYMODE_H
