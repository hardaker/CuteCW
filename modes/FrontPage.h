#ifndef FRONTPAGE_H
#define FRONTPAGE_H

#include "MorseMode.h"

class FrontPage : public MorseMode
{
public:
    FrontPage(Morse *parent, Ui::MainWindow *ui);
};

#endif // FRONTPAGE_H
