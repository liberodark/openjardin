#ifndef MYPOLYGONE_H
#define MYPOLYGONE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QPainterPath>
#include <QPolygon>
#include <QVector>
#include <QPoint>
#include <QUuid>

class MyPolygone : public QGraphicsItem
{
public:
    enum { Type = UserType + 1 };     //65537
    // userType = 65536 valeur par défaut

    MyPolygone(const QPolygon&poly);

    QRectF boundingRect() const override;

    // overriding paint()
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QPainterPath shape() const override;

    int type() const override
    {    // type d'item
        return Type;
    }

    void setPoly(const QPolygon&poly)
    {
        m_poly = poly;
    }

    QPolygon getPoly()
    {
        return m_poly;
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

    void setEtat(const int&etat)
    {
        m_etat = etat;
    }

    int getEtat()
    {
        return m_etat;
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
        return m_width;
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

    void setNom(const QString nom)
    {
        itemNom = nom;
    }

    QString getNom()
    {
        return itemNom;
    }

    void setOpac(const qreal&Opacity)
    {
        m_opacity = Opacity;
    }

    qreal getOpac()
    {
        return m_opacity;
    }

    void setStrPoly(const QString nom)
    {
        m_strpoly = nom;
    }

    QString getStrPoly()
    {
        return m_strpoly;
    }

    void setTypeLine(const int&typeLine)
    {
        m_typeLine = typeLine;
    }

    int getTypeLine()
    {
        return m_typeLine;
    }

    void setOldTypeLine(const int&typeLine)
    {
        m_oldtypeLine = typeLine;
    }

    int getOldTypeLine()
    {
        return m_oldtypeLine;
    }

    void setWidthLine(const int&widthLine)
    {
        m_widthLine = widthLine;
    }

    int getWidthLine()
    {
        return m_widthLine;
    }

    void setModif(const int&modif)
    {
        m_modif = modif;
    }

    int getModif()
    {
        return m_modif;
    }

    void setTypeAction(int typeAction);
    int getTypeAction();
    void setAction(QString action);
    QString getAction();
    void setTexte(const QString text);
    QString getTexte();
    void setComment(const QString comment);
    QString getComment();
    void setDetailFile(QString detail_filename);
    QString getDetailFile();
    QUuid getUuid() const;
    void setUuid(const QUuid&uuid);

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
    QUuid m_Uuid = QUuid::createUuid();
    int typeAction;
    int m_typeLine;
    int m_oldtypeLine;
    int m_widthLine;
    Qt::PenStyle m_penStyle;
    QString action;
    QColor brushColor;
    QColor penColor;
    qreal m_opacity;
    qreal m_width;
    qreal m_height;
    qreal m_oldX;
    qreal m_oldY;
    QString itemNom;
    QString itemText;
    QString itemComment;
    QString m_strpoly;
    QString detailfileName;
    int m_etat;
    int m_mode;           //0 modification - 1 utilisation
    int typeShape;
    int m_modif;          //0 normal - 1 en cours de modification
    QPolygon m_poly;
};

#endif // MYPOLYGONE_H
