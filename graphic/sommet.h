#ifndef SOMMET_H
#define SOMMET_H

#include <QGraphicsItem>
#include <QPainter>
#include <QList>
#include <QMenu>
#include <QPoint>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QPainterPath>

class Cote;
class mygraphicsscene;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE


class Sommet : public QGraphicsItem
{
public:
    Sommet();


    void addCote(Cote *cote);

    QList <Cote *> cotes() const;

    enum { Type = UserType + 2 };//65538
    int type() const override
    {
        return Type;
    }

    void setOpac(const qreal&Opacity)
    {
        m_opacity = Opacity;
    }

    qreal getOpac()
    {
        return m_opacity;
    }

    void setColor(const QColor&color)
    {
        brushColor = color;
    }

    void setPenColor(const QColor&color)
    {
        penColor = color;
    }

    QColor getColor()
    {
        return brushColor;
    }

    QColor getPenColor()
    {
        return penColor;
    }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant&value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;


public slots:


private:
    QList <Cote *> coteList;
    QColor brushColor;
    QColor penColor;
    qreal m_opacity;
};


#endif // SOMMET_H
