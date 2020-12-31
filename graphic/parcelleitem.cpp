#include "parcelleitem.h"
#include "myitem.h"
#include "mainwindow.h"
#include "mygraphicsscene.h"
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QtWidgets>
#include <QUuid>

parcelleItem::parcelleItem(qreal wid, qreal hgt)
{
    m_width     = wid; m_height = hgt; //initialisation des valeurs largeur - hauteur
    brushColor  = Qt::blue;
    penColor    = Qt::black;
    m_opacity   = 1.0; //transparence
    m_typeLine  = 1;
    m_widthLine = 1;
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsPathItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF parcelleItem::boundingRect() const
{
    return QRectF(-m_width / 2 - 1, -m_height / 2 - 1, m_width, m_height);
}

QPainterPath parcelleItem::shape() const
{
    QPainterPath path;

    path.addRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));
    return path;
}

void parcelleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(brushColor);
    if (getTypeLine() == 1)
    {
        m_penStyle = Qt::SolidLine;
    }
    if (getTypeLine() == 2)
    {
        m_penStyle = Qt::DashLine;
    }
    if (getTypeLine() == 3)
    {
        m_penStyle = Qt::DotLine;
    }
    if (getTypeLine() == 4)
    {
        m_penStyle = Qt::DashDotLine;
    }



    QPen pen(penColor, m_widthLine, m_penStyle);
    painter->setPen(pen);
    painter->setOpacity(m_opacity);

    switch (typeShape)
    {
    case parcelleItem::Rectangle:
        painter->drawRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));
        break;

    case parcelleItem::RoundedRectangle:
        painter->drawRoundedRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height), 10, 10);
        break;

    case parcelleItem::Circle:
        painter->drawEllipse(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));         //à verifier
        break;

    case parcelleItem::Image:
        QImage pixm(pixmapfileName);
        painter->drawImage(QRectF(-m_width / 2, -m_height / 2, m_width, m_height), pixm);        //test pixmap
        break;
    }
    painter->drawText(rect, Qt::AlignCenter, getNom());
}

void parcelleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (getMode() == 1)
    {
        scene()->clearSelection();
        setSelected(true);
        // mode utilisation
    }
    else
    {
        if (getMode() == 2) //mode modification planning
        {                   // pas d'action
        }
        else //mode modification plan
        {
            scene()->clearSelection();
            setOldX(pos().x() - (m_width / 2));
            setOldY(pos().y() - (m_height / 2));
            setSelected(true);
            setFocus();
            const QPointF event_pos = event->pos();
            if (event_pos.x() < (m_width / 2) - 10 && event_pos.y() < (m_height / 2) - 10)
            {
                setCursor(Qt::ClosedHandCursor);
                mousePressPt.setX(int(pos().x() - event->scenePos().x()));
                mousePressPt.setY(int(pos().y() - event->scenePos().y()));
            }
            else
            {
                setCursor(Qt::SizeFDiagCursor);
            }
            update();
        }
    }
    QGraphicsItem::mousePressEvent(event);
}

void parcelleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (getMode() == 1)
    {
        //mode utilisation
    }
    else
    {
        if (getMode() == 2) //mode modification planning
        {                   // pas d'action
        }
        else                //mode modification plan
        {
            if (cursor().shape() == Qt::ClosedHandCursor)
            {
                setX(mousePressPt.x() + event->scenePos().x());
                setY(mousePressPt.y() + event->scenePos().y());
            }
            else
            {
                setOpacity(0.5);
                double ObjetWidth  = event->scenePos().x() - pos().x();
                double ObjetHeight = event->scenePos().y() - pos().y();
                if (ObjetWidth > 0 && ObjetHeight > 0)
                {
                    ObjetWidth  = ObjetWidth + (m_width / 2);
                    ObjetHeight = ObjetHeight + (m_height / 2);
                    setSizeShape(ObjetWidth, ObjetHeight);
                    // positionnement de l'item pour conserver la position de l'angle haut-gauche
                    setPos(getOldX() + (ObjetWidth / 2), getOldY() + (ObjetHeight / 2));
                }
            }
            event->accept();
            update();
        }
    }
}

void parcelleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (getMode() == 1)
    {
        // mode utilisation
    }
    else
    {
        if (getMode() == 2) //mode modification planning
        {                   // pas d'action
        }
        else //mode modification plan
        {
            if (cursor().shape() == Qt::ClosedHandCursor)
            {
                setCursor(Qt::ArrowCursor);
            }
            else
            {
                setOpacity(1);
                double ObjetWidth  = event->scenePos().x() - pos().x();
                double ObjetHeight = event->scenePos().y() - pos().y();
                if (ObjetWidth > 0 && ObjetHeight > 0)
                {
                    ObjetWidth  = ObjetWidth + (m_width / 2);
                    ObjetHeight = ObjetHeight + (m_height / 2);
                    setPos(getOldX() + (ObjetWidth / 2), getOldY() + (ObjetHeight / 2));
                    setSizeShape(ObjetWidth, ObjetHeight);
                }
                setCursor(Qt::ArrowCursor);
            }
            update();
        }
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void parcelleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    setToolTip(getTexte());
}

void parcelleItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (getMode() != 1 &&
        getMode() != 2)
    {
        if (event->pos().x() < (m_width / 2) - 10 && event->pos().y() < (m_height / 2) - 10)
        {
            setCursor(Qt::OpenHandCursor);
        }
        else
        {
            setCursor(Qt::SizeFDiagCursor);
        }
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}

QVariant parcelleItem::itemChange(GraphicsItemChange change, const QVariant&value)
{
    /*  if (change == QGraphicsItem::ItemSelectedChange)
     * {
     *    if (value == false)
     *    {
     *        setTypeLine(getOldTypeLine());
     *    }
     *    else
     *    {
     *        setOldTypeLine(getTypeLine());
     *        setTypeLine(Qt::DotLine);
     *    }
     * }*/


    return QGraphicsItem::itemChange(change, value);
}

void parcelleItem::setTexte(const QString text)
{
    itemText = text;
}

QString parcelleItem::getTexte()
{
    return itemText;
}

void parcelleItem::setComment(const QString comment)
{
    itemComment = comment;
}

QString parcelleItem::getComment()
{
    return itemComment;
}

int parcelleItem::getTypeAction()
{
    return typeAction;
}

void parcelleItem::setTypeAction(int typeAct)
{
    typeAction = typeAct;
}

QString parcelleItem::getAction()
{
    return action;
}

void parcelleItem::setAction(QString act)
{
    action = act;
}

QString parcelleItem::getPixFile()
{
    return pixmapfileName;
}

void parcelleItem::setPixFile(QString file)
{
    pixmapfileName = file;
}

QUuid parcelleItem::getUuid() const
{
    return m_Uuid;
}

void parcelleItem::setUuid(const QUuid&uuid)
{
    m_Uuid = uuid;
}
