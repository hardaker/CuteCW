#ifndef INVADER_H
#define INVADER_H

#include <QGraphicsItem>
#include <QGraphicsWidget>

class Invader : public QGraphicsItem
{
public:
    explicit Invader(QGraphicsWidget *graphWidget = 0, const QString &text = "");

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool advanceInvader(int maxYPos = 0);

    const QString &text() const { return m_text; }

    bool exploding() const { return m_exploding; }
    void setExploding(bool value) { m_exploding = value; m_explodeSize = 10; }
    bool doneExploding();

protected:

signals:

public slots:

private:
    QString m_text;
    bool m_exploding;
    int m_explodeSize;
};

#endif // INVADER_H
