#include "FrontPage.h"

FrontPage::FrontPage(Morse *parent, Ui::MainWindow *ui)
    : MorseMode(parent, ui)
{
}

void FrontPage::switchToMode()
{
    m_ui->helpBar->setText("<font color=\"green\">Selecting a starting place from the buttons below</font>");
}

QString FrontPage::helpText()
{
    return tr("<p>From this screen you can access all the other modes.  This is just a starting screen.  "
              "<p>If you want to learn morse code, consider following the steps in the left hand panel.  "
              "Then, to continue your training and/or have some fun in the process, try the modes in the right hand panel."
              "<p>once you enter a mode, use the menus to navegate to other modes.</p>");
}

QString FrontPage::name()
{
    return tr("Home Page");
}
