#include "KeyInvaders.h"
#include <QtGui/QMenu>

KeyInvaders::KeyInvaders(Morse *parent, Ui::MainWindow *main)
    : MorseMode(parent, main), m_scores("Key Invaders")
{
}

void
KeyInvaders::advanceFrame() {

}

void KeyInvaders::switchToMode() {
    m_ui->helpBar->setText(tr("<font color=\"green\">Key the letters using a st.</font>"));
    m_ui->changeSequence->hide();
    setupWidgets();
}

void KeyInvaders::modeMenus() {
    // Create the high scores button
    QAction *action = menu()->addAction("HighScores");
    connect(action, SIGNAL(triggered()), &m_scores, SLOT(showScores()));
}

void KeyInvaders::setupWidgets() {

}

void KeyInvaders::handleKeyPress(QChar letterPressed)
{
    Q_UNUSED(letterPressed);

    // memorize keypress timing
    if (m_keyCount == 0) {
        m_startTime.start();
        m_keyedTimes.push_back(0);
    } else {
        m_keyedTimes.push_back(m_startTime.elapsed());
    }
}

void KeyInvaders::handleKeyRelease(QChar letterPressed)
{
    handleKeyPress(letterPressed);  // doesn't really matter whether it's up/down
}


void KeyInvaders::gameOver()
{
  m_scores.addScore("", score());
}

QString KeyInvaders::helpText()
{
    return tr("<p>Shoot letters falling from the sky by keying them appropriately.</p>");
}

QString KeyInvaders::name()
{
    return tr("Key Invaders");
}

QString KeyInvaders::icon()
{
    return ":/icons/64x64/game.png";
}
