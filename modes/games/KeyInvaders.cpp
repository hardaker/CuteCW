#include "KeyInvaders.h"
#include <qdebug.h>
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QRandomGenerator>

KeyInvaders::KeyInvaders(Morse *parent, Ui::MainWindow *main)
    : MorseMode(parent, main), MGameMode(), MSequences(), KeyingReader(),
      m_scores("Key Invaders"), invadingTimer(0),
      invaders(), maxInvaderY(90)
{
}

void
KeyInvaders::advanceFrame() {
    int timing_used;


    // check what the user has keyed.

    // temporarily push back the current time
    // But we only do this if they're key is currently up (even number of times)
    if (m_keyedTimes.count() > 0) {
        bool pushedOne = false;

        if (m_keyedTimes.count() % 2 == 0) {
            m_keyedTimes.push_back(m_startTime.elapsed());
            pushedOne = true;
        }

        // check what we have so far
        QString results = analyzeKeying(m_keyedTimes, &timing_used);

        if (results.length() > 0) {
            qDebug() << "got a keying: " << results;
            foreach(QChar result, results) {
                bool gotOne = false;
                foreach (Invader *invader, invaders) {
                    if (!gotOne && !invader->exploding() && invader->text() == result.toUpper()) {
                        invader->setExploding(true);
                        addToScore(10 * (maxInvaderY - invader->y()));
                        gotOne = true;
                    }
                }
            }

            // XXX: this should only drop up till the 'timing_used' number of entries
            m_keyedTimes.clear();
        } else {
            // drop the most recently pushed "space" time.
            if (pushedOne)
                m_keyedTimes.pop_back();
        }
    }

    foreach (Invader *invader, invaders) {
        if (invader->advanceInvader(maxInvaderY)) {
            gameOver();
        }
        if (invader->doneExploding()) {
            invaders.removeOne(invader);
            delete invader;
        }
    }

    addCount++;
    if (addCount%20 == 0) {
        // every once in a while do something interesting.  Like add more invaders.
        Invader *inv;
        m_scene->addItem(inv = new Invader(0, completeCharacterSet[QRandomGenerator::global()->generate() % completeCharacterSet.length()].toUpper()));
        inv->setPos(QRandomGenerator::global()->generate() % 200, 10);
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
    qDebug() << "key down: " << m_keyedTimes;
}

void KeyInvaders::handleKeyRelease(QChar letterPressed)
{
    handleKeyPress(letterPressed);  // doesn't really matter whether it's up/down
    qDebug() << "key up:   " << m_keyedTimes;
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
    m_keyedTimes.clear();

    // set the animation timer
    if (!invadingTimer) {
        invadingTimer = new QTimer(this);
        connect(invadingTimer, SIGNAL(timeout()), this, SLOT(advanceFrame()));
        invadingTimer->setInterval(100);
    }
    invadingTimer->start();
}

void KeyInvaders::stop()
{
    invadingTimer->stop();
}

void KeyInvaders::scaleWindow() {
    // get rid of the current scale
    qreal oldScale = 1.0 / m_graph->transform().mapRect(QRectF(0, 0, 1, 1)).height();
    qDebug() << "old scale = " << oldScale;
    m_graph->scale(oldScale, oldScale);

    // calculate the new scale
    QSize windowSize = m_graph->size();
    qDebug() << "window: " << windowSize;
    qDebug() << "scene: " << m_scene->sceneRect().width() << "x" << m_scene->sceneRect().height();
    qDebug() << "scale: " << windowSize.width() << "/" << m_scene->sceneRect().width() << " : " << windowSize.height() << "/" << m_scene->sceneRect().height();
    qDebug() << "scale: " << windowSize.width()/m_scene->sceneRect().width() << " : " << windowSize.height()/m_scene->sceneRect().height();
    qreal newscale = qMin(windowSize.width() / m_scene->sceneRect().width(), windowSize.height() / m_scene->sceneRect().height());

    // apply it
    m_graph->scale(1.5 * newscale, 1.5 * newscale); // why why why the 1.5?
    qDebug() << "scale: " << newscale;
}
