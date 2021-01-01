#include "cote.h"
#include "sommet.h"

#include <qmath.h>
#include <QPainter>

Cote::Cote(Sommet *sourceSommet, Sommet *destSommet)
    : arrowSize(6)
{
    setAcceptedMouseButtons(nullptr);
    source = sourceSommet;
    dest   = destSommet;
    source->addCote(this);
    dest->addCote(this);
    adjust();
}

Sommet *Cote::sourceSommet() const
{
    return source;
}

Sommet *Cote::destSommet() const
{
    return dest;
}

void Cote::adjust()
{
    if (!source || !dest)
    {
        return;
    }

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal  length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.))
    {
        QPointF coteOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + coteOffset;
        destPoint   = line.p2() - coteOffset;
    }
    else
    {
        sourcePoint = destPoint = line.p1();
    }
}

QRectF Cote::boundingRect() const
{
    if (!source || !dest)
    {
        return QRectF();
    }

    qreal penWidth = 1;
    qreal extra    = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
           .normalized()
           .adjusted(-extra, -extra, extra, extra);
}

void Cote::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
    {
        return;
    }

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
    {
        return;
    }

    painter->setPen(QPen(Qt::blue, 0.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
}
