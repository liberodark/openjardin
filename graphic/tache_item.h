#ifndef TACHE_ITEM_H
#define TACHE_ITEM_H
#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QPainterPath>

class tache_item : public QGraphicsItem
{
public:
    enum TypeShape
    {
        Rectangle = 1,
        RoundedRectangle,
        Circle,
        Image,
        Triangle,
        Line,
        Avancement,
        Losange,
    };
    enum { Type = UserType + 1 };     //65537
    // userType = 65536 valeur par défaut
    tache_item(qreal wid, qreal hgt);

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

    void setMyPen(const QPen&MyPen)
    {
        m_pen = MyPen;
    }

    QPen getMyPen()
    {
        return m_pen;
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

    void setTypeLine(const int&typeLine)
    {
        m_typeLine = typeLine;
    }

    int getTypeLine()
    {
        return m_typeLine;
    }

    void setWidthLine(const int&widthLine)
    {
        m_widthLine = widthLine;
    }

    int getWidthLine()
    {
        return m_widthLine;
    }

    void setTypeAction(int typeAction);
    int getTypeAction();
    void setAction(QString action);
    QString getAction();
    void setTexte(const QString text);
    QString getTexte();
    void setComment(const QString comment);
    QString getComment();
    void setPixFile(QString pixmapfileName);
    QString getPixFile();

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
    int m_typeLine;
    int m_widthLine;
    Qt::PenStyle m_penStyle;
    QString action;
    QColor brushColor;
    QColor penColor;
    QPen m_pen;
    qreal m_opacity;
    qreal m_width;
    qreal m_height;
    qreal m_oldX;
    qreal m_oldY;
    QString itemNom;
    QString itemText;
    QString itemComment;
    int typeShape;
    QString pixmapfileName;
    int m_etat;          // 0 item caché et non utilisé - 1 affiché
    int m_mode;          //0 modification - 1 utilisation
    QPointF offset;
    QPointF getSnap(const QPointF&pointP);
};

#endif // TACHE_ITEM_H
