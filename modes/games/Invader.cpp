#include "Invader.h"

#include <QtGui/QPainter>

Invader::Invader(QGraphicsWidget *parent, const QString &text) :
    QGraphicsItem(), m_text(text)
{
    setZValue(2);
}

QRectF Invader::boundingRect() const
{
    return QRectF(-10, -10, 10, 10);
}

QPainterPath Invader::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 10, 10);
    return path;
}

void Invader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::yellow);
    painter->drawEllipse(-10, -10, 10, 10);

    QFont font = painter->font();
    font.setPointSize(6);
    painter->setFont(font);
    painter->setPen(Qt::red);
    painter->drawText(QRectF(-9,-9,9,9), Qt::AlignHCenter | Qt::AlignVCenter, m_text);
}

bool Invader::advanceInvader(int maxYPos) {
    QPointF myPos = pos();
    myPos.setY(myPos.y() + 1);
    setPos(myPos);
    if (maxYPos && myPos.y() > maxYPos)
        return true;
    return false;
}

