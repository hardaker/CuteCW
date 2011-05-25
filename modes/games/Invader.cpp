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
    painter->setBrush(Qt::lightGray);
    painter->drawEllipse(-5, -5, 20, 20);

    QFont font = painter->font();
    font.setPointSize(16);
    painter->setFont(font);
    painter->setPen(Qt::red);
    painter->drawText(QRectF(0,0,20,20), Qt::AlignHCenter | Qt::AlignVCenter, m_text);
}
