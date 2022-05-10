#include "Invader.h"

#include <QPainter>

Invader::Invader(QGraphicsWidget *parent, const QString &text) :
    QGraphicsItem(parent), m_text(text), m_exploding(false), m_explodeSize(InvaderCircleSize)
{
    setZValue(2);
}

QRectF Invader::boundingRect() const
{
    return QRectF(-m_explodeSize, -m_explodeSize, m_explodeSize, m_explodeSize);
}

QPainterPath Invader::shape() const
{
    QPainterPath path;
    path.addEllipse(-m_explodeSize, -m_explodeSize, m_explodeSize, m_explodeSize);
    return path;
}

void Invader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    int oneLess = m_explodeSize-1;

    painter->setPen(Qt::NoPen);
    QBrush yellow(Qt::yellow);
    if (m_exploding) {
        QColor newYellow = yellow.color();
        newYellow.setAlpha(255.0 * (float(InvaderExplodedSize) - m_explodeSize) / float(InvaderCircleSize));
        yellow.setColor(newYellow);
    }
    painter->setBrush(yellow);
    painter->drawEllipse(-m_explodeSize, -m_explodeSize, m_explodeSize, m_explodeSize);

    QFont font = painter->font();
    font.setPointSizeF(float(m_explodeSize)/2.5);
    painter->setFont(font);
    painter->setPen(Qt::red);
    painter->drawText(QRectF(-oneLess,-oneLess,oneLess,oneLess), Qt::AlignHCenter | Qt::AlignVCenter, m_text);
    painter->setPen(Qt::darkYellow);
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(-m_explodeSize,-m_explodeSize,m_explodeSize,m_explodeSize);
}

bool Invader::advanceInvader(int maxYPos) {
    QPointF myPos = pos();
    myPos.setY(myPos.y() + 1);
    setPos(myPos);

    if (m_exploding)
        m_explodeSize++;
    else if (maxYPos && myPos.y() > maxYPos)
        return true; // only return true if not exploding yet
    return false;
}

bool Invader::doneExploding() {
    if (m_exploding && m_explodeSize > InvaderExplodedSize)
        return true;
    return false;
}
