#include "GoalBar.h"

#include <QtGui/QPainter>
#include <qdebug.h>

GoalBar::GoalBar(QWidget *parent)
    : QProgressBar(parent), m_goalBarLevel(-1)
{
}

GoalBar::GoalBar(QWidget *parent, int goalBarLevel)
    : QProgressBar(parent), m_goalBarLevel(goalBarLevel)
{
}

void GoalBar::paintEvent(QPaintEvent *event)
{
    QProgressBar::paintEvent(event);

    if (m_goalBarLevel < 0)
        return;

    QPainter painter(this);
    int barWidth = width();
    int barHeight = height();
    int drawAtHeight = barHeight - int(float(barHeight) * float(m_goalBarLevel - minimum())/float(maximum() - minimum()));

    QColor markerColor(0,0,0);

    painter.save();
    painter.setPen(markerColor);
    painter.drawLine(0,drawAtHeight,barWidth,drawAtHeight);
    painter.restore();
}

void GoalBar::setGoalBarLevel(int newlevel)
{
    m_goalBarLevel = newlevel;
}

