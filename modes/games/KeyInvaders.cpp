#include "KeyInvaders.h"
#include <qdebug.h>
#include <QtGui/QMenu>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsTextItem>

KeyInvaders::KeyInvaders(Morse *parent, Ui::MainWindow *main)
    : MorseMode(parent, main), m_scores("Key Invaders"), invadingTimer(this)
{
}

void
KeyInvaders::advanceFrame() {
    qDebug() << "here";
}


void KeyInvaders::switchToMode() {
    m_ui->helpBar->setText(tr("<font color=\"green\">Key the letters using a st.</font>"));
    m_ui->play->show();
    setupWidgets();
}

void KeyInvaders::modeMenus() {
    // Create the high scores button
    QAction *action = menu()->addAction("HighScores");
    connect(action, SIGNAL(triggered()), &m_scores, SLOT(showScores()));
}

void KeyInvaders::setupWidgets() {
    m_ui->forModes->addWidget(m_graph = new QGraphicsView());
    QGraphicsScene *myScene = new QGraphicsScene(m_graph);
    m_graph->setScene(myScene);

    myScene->addEllipse(10,10,10,10);
    myScene->addEllipse(10,100,20,10);
    myScene->addEllipse(100,10,20,10);
    myScene->addItem(new Invader(0, "a"));
    QGraphicsTextItem *item = myScene->addText("foo");
    item->setPos(50,50);
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

void KeyInvaders::play()
{
    qDebug() << "foo";
    invadingTimer.setInterval(100);
    connect(&invadingTimer, SIGNAL(timeout()), this, SLOT(advanceFrame()));
    invadingTimer.start();
}

void KeyInvaders::stop()
{
    invadingTimer.stop();
}
