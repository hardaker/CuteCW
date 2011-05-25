#include "Invader.h"

#include <QtGui/QPainter>

Invader::Invader(QGraphicsWidget *parent, const QString &text) :
    QGraphicsItem(), m_text(text)
{
}

QRectF Invader::boundingRect() const
{
    return QRectF(-5, -5, 20, 20);
}

QPainterPath Invader::shape() const
{
    QPainterPath path;
    path.addEllipse(-5, -5, 20, 20);
    return path;
}

void Invader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-5, -5, 20, 20);

    QFont font = painter->font();
    font.setPointSize(4);
    painter->setFont(font);
    painter->drawText(QRectF(-5, -5, 20, 20), m_text);
}

QVariant Invader::itemChange(GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsItem::itemChange(change, value);
}
