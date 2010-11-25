#ifndef READMODE_H
#define READMODE_H

#include <QtGui/QTextCursor>

#include "MorseMode.h"

class ReadMode : public MorseMode
{
public:
    Q_OBJECT

public:
    ReadMode(Morse *parent, Ui::MainWindow *ui);

    void addButtons();
public slots:
    virtual void switchToMode();
    virtual void play();
    virtual void readNextLetter();
    virtual void audioStopped();
    void openFile();

private:
    QTextCursor                     m_readSpot;

};

#endif // READMODE_H
