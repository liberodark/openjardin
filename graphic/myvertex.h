#ifndef MYVERTEX_H
#define MYVERTEX_H

#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QPainterPath>

class MyVertex : public QGraphicsItem
{
public:
    enum TypeShape
    {
        Rectangle = 1,
        RoundedRectangle,
        Circle,
        Image
    };
    enum { Type = UserType + 2 }; //65538
    // userType = 65536 valeur par défaut

    MyVertex(qreal wid, qreal hgt);

    QRectF boundingRect() const override;

    // overriding paint()
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QPainterPath shape() const override;

    int type() const
    {    // type d'item
        return Type;
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

    void setTypeShape(const int&type)
    {
        typeShape = type;
    }

    int getTypeShape()
    {
        return typeShape;
    }

    void setMode(const int&mode)
    {
        m_mode = mode;
    }

    int getMode()
    {
        return m_mode;
    }

    int getWidth()
    {
        return int(m_width);
    }

    void setSizeShape(const qreal&W, const qreal&H)
    {
        m_width  = W;
        m_height = H;
    }

    void setOldX(const qreal&X)
    {
        m_oldX = X;
    }

    void setOldY(const qreal&Y)
    {
        m_oldY = Y;
    }

    qreal getOldX()
    {
        return m_oldX;
    }

    qreal getOldY()
    {
        return m_oldY;
    }

    void setId(const int&id)
    {
        m_itemId = id;
    }

    int getId()
    {
        return m_itemId;
    }

    void setOpac(const qreal&Opacity)
    {
        m_opacity = Opacity;
    }

    qreal getOpac()
    {
        return m_opacity;
    }

    // item state
    bool Pressed;
protected:
    // overriding mouse events
    QPoint mousePressPt;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant&value) override;


private:
    int m_itemId;
    int typeAction;
    QString action;
    QColor brushColor;
    QColor penColor;
    qreal m_opacity;
    qreal m_width;
    qreal m_height;
    qreal m_oldX;
    qreal m_oldY;
    int typeShape;
    int m_mode;     //0 modification - 1 utilisation
    QPointF offset;
    QPointF getSnap(const QPointF&pointP);
};

#endif // MYVERTEX_H
