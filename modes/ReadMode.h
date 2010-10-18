#ifndef READMODE_H
#define READMODE_H

#include <QtGui/QTextCursor>

#include "MorseMode.h"

class ReadMode : public MorseMode
{
public:
    ReadMode(Morse *parent, Ui::MainWindow *ui);

public slots:
    virtual void switchToMode();
    virtual void readIt();
    virtual void readNextLetter();

private:
    QTextCursor                     m_readSpot;

};

#endif // READMODE_H
