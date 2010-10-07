#include "MorseMode.h"
#include "Morse.h"

MorseMode::MorseMode(Morse *morse)
{
    m_morse = morse;
}

Morse *MorseMode::morseParent() {
    return m_morse;
}
