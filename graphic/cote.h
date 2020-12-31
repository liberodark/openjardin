#ifndef COTE_H
#define COTE_H

#include <QGraphicsItem>


class Sommet;
class mygraphicsscene;
class Cote : public QGraphicsItem
{
public:
    Cote(Sommet *sourceSommet, Sommet *destSommet);

    Sommet *sourceSommet() const;
    Sommet *destSommet() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override
    {
        return Type;
    }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Sommet *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};


#endif // COTE_H
