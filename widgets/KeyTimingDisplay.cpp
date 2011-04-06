#include "KeyTimingDisplay.h"

#include <QtGui/QPainter>

KeyTimingDisplay::KeyTimingDisplay(QWidget *parent) :
    QWidget(parent)
{
}

void KeyTimingDisplay::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::red);
    painter.drawRect(5,5,width()-10,height()-10);
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

