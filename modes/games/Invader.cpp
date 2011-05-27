#include "Invader.h"

#include <QtGui/QPainter>

Invader::Invader(QGraphicsWidget *parent, const QString &text) :
    QGraphicsItem(), m_text(text)
{
}

QRectF Invader::boundingRect() const
{
    return QRectF(0, 0, 20, 20);
}

QPainterPath Invader::shape() const
{
    QPainterPath path;
    path.addEllipse(0, 0, 20, 20);
    return path;
}

void Invader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::yellow);
    painter->drawEllipse(-5, -5, 20, 20);

    QFont font = painter->font();
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::red);
    painter->drawText(QRectF(-4,-4,19,19), Qt::AlignHCenter | Qt::AlignVCenter, m_text);
}

void Invader::advance() {
    QPointF myPos = pos();
    myPos.setY(myPos.y() + 1);
    setPos(myPos);
}
