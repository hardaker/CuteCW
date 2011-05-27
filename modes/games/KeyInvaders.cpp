#include "KeyInvaders.h"
#include <qdebug.h>
#include <QtGui/QMenu>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsTextItem>

KeyInvaders::KeyInvaders(Morse *parent, Ui::MainWindow *main)
    : MorseMode(parent, main), MSequences(), m_scores("Key Invaders"), invadingTimer(this),
      invaders(), maxInvaderY(90)
{
}

void
KeyInvaders::advanceFrame() {
    qDebug() << "here";
    foreach (Invader *invader, invaders) {
        if (invader->advanceInvader(maxInvaderY)) {
            gameOver();
        }
    }

    addCount++;
    if (addCount%20 == 0) {
        // every once in a while do something interesting.  Like add more invaders.
        Invader *inv;
        m_scene->addItem(inv = new Invader(0, completeCharacterSet[qrand() % completeCharacterSet.length()].toUpper()));
        inv->setPos(qrand() % 200, 10);
        invaders.push_back(inv);
    }
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
    m_scene = new QGraphicsScene(m_graph);
    m_graph->setBackgroundBrush(Qt::black);
    m_graph->setScene(m_scene);

    QPen whitePen(Qt::white);
    QPen bluePen(Qt::blue);
    QPen greenPen(Qt::green);
    QBrush greenBrush(Qt::green);

    m_scene->addRect(-10,0,220,100,whitePen);
    m_scene->addEllipse(10,10,10,10);
    m_scene->addEllipse(10,100,20,10);
    m_scene->addEllipse(100,10,20,10);
    m_scene->addRect(-10,maxInvaderY,220,10, greenPen, greenBrush);

    QGraphicsTextItem *item = m_scene->addText("Fear The Invaders");
    item->setZValue(-5);
    item->setPos(20,50);

    scaleWindow();
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
  stop();
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
    addCount = 0;
    foreach (Invader *inv, invaders) {
        m_scene->removeItem(inv);
        delete inv;
    }
    invaders.clear();
    invadingTimer.setInterval(100);
    connect(&invadingTimer, SIGNAL(timeout()), this, SLOT(advanceFrame()));
    invadingTimer.start();
}

void KeyInvaders::stop()
{
    invadingTimer.stop();
}

void KeyInvaders::scaleWindow() {
    // get rid of the current scale
    qreal oldScale = 1.0 / m_graph->transform().mapRect(QRectF(0, 0, 1, 1)).height();
    m_graph->scale(oldScale, oldScale);

    // calculate the new scale
    QSize windowSize = m_graph->size();
    qDebug() << "window: " << windowSize;
    qreal newscale = qMin(windowSize.width() / m_scene->sceneRect().width(), windowSize.height() / m_scene->sceneRect().width());

    // apply it
    m_graph->scale(.95 * newscale, .95 * newscale);
    qDebug() << "scale: " << newscale;
}
