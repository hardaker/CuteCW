#include "KeyTimingDisplay.h"

#include <QtGui/QPainter>
#include <qdebug.h>

KeyTimingDisplay::KeyTimingDisplay(QWidget *parent) :
    QWidget(parent)
{
}

void KeyTimingDisplay::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();

    int minx = 10, miny = 0, maxx = width() - 10, maxy = height();
    int widgetWidth = maxx - minx, widgetHeight = height();
    int lineHeight = 10;

    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, width(), height());

    if (m_keyedTimings.length() > 0) {
        int biggestTime = qMax(m_keyedTimings.last(), m_requiredTimings.last());

        painter.setBrush(Qt::yellow);
        QList<int>::const_iterator spot = m_requiredTimings.begin();
        QList<int>::const_iterator end = m_requiredTimings.end();

        QList<int>::const_iterator keyedSpot = m_keyedTimings.begin();
        QList<int>::const_iterator keyedEnd = m_keyedTimings.end();

        while(spot != end) {
            QList<int>::const_iterator startingSpot = spot;
            QList<int>::const_iterator keyedStartingSpot = keyedSpot;

            // draw the required timing box that is the perfect keying
            spot++;
            painter.drawRect(minx + (widgetWidth * *startingSpot)/biggestTime,    lineHeight,
                             (widgetWidth * (*spot - *startingSpot))/biggestTime, lineHeight);
            spot++;

            // draw the box that was actually keyed
            keyedSpot++;
            painter.drawRect(minx + (widgetWidth * *keyedStartingSpot)/biggestTime,          3*lineHeight,
                             (widgetWidth * (*keyedSpot - *keyedStartingSpot))/biggestTime,  lineHeight);
            keyedSpot++;
        }
    }

    painter.restore();
}

void KeyTimingDisplay::setTimings(QList<int> keyedTimings, QList<int> requiredTimings)
{
    m_keyedTimings = keyedTimings;
    m_requiredTimings = requiredTimings;
    update();
}

void KeyTimingDisplay::clear()
{
    m_keyedTimings.clear();
    m_requiredTimings.clear();
    update();
}

