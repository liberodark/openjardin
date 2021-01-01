// tache_item.cpp

/****************************************************************************
**
** Copyright (C) 2017
** **
** You may use this file under the terms of the GNU GENERAL PUBLIC LICENSE
** Version 3, 29 June 2007  as follows:
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/

#include "graphic/tache_item.h"
#include "mainwindow.h"
#include "consts.h"
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QtWidgets>

tache_item::tache_item(qreal wid, qreal hgt)
{                                      //Affichage des formes des tâches dans le planning
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

QRectF tache_item::boundingRect() const
{
    return QRectF(-m_width / 2 - 1, -m_height / 2 - 1, m_width, m_height);
}

QPainterPath tache_item::shape() const
{
    QPainterPath path;

    path.addRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));
    return path;
}

void tache_item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(brushColor);
    QPen pen(penColor);
    painter->setPen(pen);
    painter->setOpacity(m_opacity);

    switch (typeShape)
    {
    case tache_item::Rectangle:
        painter->drawRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));
        break;

    case tache_item::RoundedRectangle:
        painter->drawRoundedRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height), 10, 10);
        break;

    case tache_item::Circle:
        painter->drawEllipse(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));
        break;

    case tache_item::Triangle:
    {
        int      xbase = int(m_width) - 14;
        int      ybase = int((m_height / 2)) - 14;
        int      cote  = 7;
        QPolygon polygon;
        polygon << QPoint(xbase, ybase - 14) << QPoint(xbase + cote, ybase + 14) << QPoint(xbase - cote, ybase + 14);
        painter->drawPolygon(polygon);
        break;
    }

    case tache_item::Losange:
    {
        int      xbase = int( m_width) - 14;
        int      ybase = int((m_height / 2) - 14);
        int      cote  = 7;
        QPolygon polygon;
        polygon << QPoint(xbase, ybase - 14) << QPoint(xbase + cote, ybase)
                << QPoint(xbase, ybase + 14) << QPoint(xbase - cote, ybase);
        painter->drawPolygon(polygon);

        break;
    }

    case tache_item::Line:
    {
        painter->drawRect((QRectF(-m_width / 2, -m_height / 2, m_width, m_height)));
        // painter->drawRect((QRectF(-m_width / 2, (-m_height / 2) + m_height, 4, 8)));
        int      xbase = int( -m_width / 2);
        int      ybase = int((-m_height / 2) + m_height);
        int      cote  = 8;
        QPolygon polygon;
        polygon << QPoint(xbase, ybase) << QPoint(xbase + cote, ybase)
                << QPoint(xbase + (cote / 2), ybase + cote);
        painter->drawPolygon(polygon);
        int      xbase2 = int(m_width / 2);
        QPolygon polygon2;
        polygon2 << QPoint(xbase2, ybase) << QPoint(xbase2 - cote, ybase)
                 << QPoint(xbase2 - (cote / 2), ybase + cote);
        painter->drawPolygon(polygon2);

        break;
    }

    case tache_item::Avancement:
        painter->drawRect((QRectF(-m_width / 2, -m_height / 2, m_width, m_height)));
        break;

    case tache_item::Image:
    { QImage pixm(pixmapfileName);
      painter->drawImage(QRectF(-m_width / 2, -m_height / 2, m_width, m_height), pixm);
      break; }
    }
    painter->drawText(rect, Qt::AlignCenter, getNom());
}

QVariant tache_item::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant&value)
{
    if (change == ItemPositionChange && scene())
    {
        QPointF newPos = value.toPointF();
        return newPos;
    }
    else
    {
        return QGraphicsItem::itemChange(change, value);
    }
}

QPointF tache_item::getSnap(const QPointF& pointP)
{
    int   gridSize = Consts::SPACE_CASE;
    qreal xV       = floor(pointP.x() / gridSize) * gridSize;
    qreal yV       = pointP.y();

    return QPointF(xV, yV);
}

void tache_item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    offset = getSnap(event->scenePos());
    int gridSize = Consts::SPACE_CASE;

    if (getMode() == 1)
    {
        scene()->clearSelection();
        setSelected(true);
        // pas de déplacement
    }
    else
    {
        if (getMode() == 2) //mode modification planning
        {
            scene()->clearSelection();
            setSelected(true);
            double ObjetWidth = m_width;

            if (int(ObjetWidth) % 2 != 0)
            {
                double oldx = floor((pos().x() - (m_width / 2)) / gridSize) * gridSize;
                setOldX(oldx);
            }
            else
            {
                double oldx = ceil((pos().x() - (m_width / 2)) / gridSize) * gridSize;
                setOldX(oldx);
            }
            setOldY(pos().y() - (m_height / 2));
            const QPointF event_pos = event->pos();
            if (event_pos.x() < (m_width / 2) - 10 && event_pos.y() < (m_height / 2) - 10)
            {
                setCursor(Qt::ClosedHandCursor);
                mousePressPt.setX(int(pos().x()) - int(offset.x()));
            }
            else
            {
                setCursor(Qt::SizeFDiagCursor);
            }
            update();
        }
        if (getMode() == 3) //mode déplacement losange et triangle
        {
            scene()->clearSelection();
            setSelected(true);
            double ObjetWidth = m_width;

            if (int(ObjetWidth) % 2 != 0)
            {
                double oldx = floor((pos().x() - (m_width / 2)) / gridSize) * gridSize;
                setOldX(oldx);
            }
            else
            {
                double oldx = ceil((pos().x() - (m_width / 2)) / gridSize) * gridSize;
                setOldX(oldx);
            }
            setOldY(pos().y() - (m_height / 2));
            const QPointF event_pos = event->pos();
            if (event_pos.x() < (m_width / 2) && event_pos.y() < (m_height / 2))
            {
                setCursor(Qt::ClosedHandCursor);
                mousePressPt.setX(int(pos().x()) - int(offset.x()));
            }
            update();
        }
    }
    QGraphicsItem::mousePressEvent(event);
}

void tache_item::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    offset = getSnap(event->scenePos());
    QPointF offset2 = getSnap(mousePressPt);

    int gridSize = Consts::SPACE_CASE;
    if (getMode() == 1)
    {
        //mode sans déplacement
    }
    else
    {
        if (getMode() == 2) //mode modification planning
        {
            if (cursor().shape() == Qt::ClosedHandCursor)
            {
                double ObjetWidth = offset.x() - pos().x();
                if (int(ObjetWidth) % 2 != 0)
                {
                    setX(offset2.x() + offset.x() + (gridSize / 2));
                }
                else
                {
                    setX(offset2.x() + offset.x());
                }
            }
            else
            {
                setOpacity(0.5);
                double ObjetWidth  = offset.x() - pos().x();
                double ObjetHeight = m_height;
                if (ObjetWidth > 14 && ObjetHeight > 0)
                {
                    if (int(ObjetWidth) % 2 != 0)
                    {
                        ObjetWidth = ceil((ObjetWidth + (m_width / 2)) / gridSize) * gridSize;
                    }
                    else
                    {
                        ObjetWidth = floor((ObjetWidth + (m_width / 2)) / gridSize) * gridSize;
                    }
                }
                else
                {
                    ObjetWidth = gridSize;
                }
                ObjetHeight = m_height;
                setSizeShape(ObjetWidth, ObjetHeight);
                setPos(getOldX() + (ObjetWidth / 2), getOldY() + (ObjetHeight / 2));
            }
        }
        if (getMode() == 3)     //mode déplacement triangle et losange
        {
            if (cursor().shape() == Qt::ClosedHandCursor)
            {
                double ObjetWidth = offset.x() - pos().x();
                if (int(ObjetWidth) % 2 != 0)
                {
                    setX(offset2.x() + offset.x() + (gridSize / 2));
                }
                else
                {
                    setX(offset2.x() + offset.x());
                }
            }
        }
        event->accept();
        update();
    }

    QGraphicsItem::mousePressEvent(event);
}

void tache_item::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    offset = getSnap(event->scenePos());
    int gridSize = Consts::SPACE_CASE;
    if (getMode() == 1)
    {
        //mode sans déplacement
    }
    else
    {
        if (getMode() == 2)     //mode modification planning
        {
            if (cursor().shape() == Qt::ClosedHandCursor)
            {
                setCursor(Qt::ArrowCursor);
            }
            else
            {
                setOpacity(1);
                double ObjetWidth  = offset.x() - pos().x();
                double ObjetHeight = m_height;
                if (ObjetWidth > 0 && ObjetHeight > 0)
                {
                    ObjetWidth  = floor((ObjetWidth + (m_width / 2)) / gridSize) * gridSize;
                    ObjetHeight = m_height;
                    setPos(getOldX() + (ObjetWidth / 2), getOldY() + (ObjetHeight / 2));
                    setSizeShape(ObjetWidth, ObjetHeight);
                }
                setCursor(Qt::ArrowCursor);
            }
        }
        if (getMode() == 3)     //mode déplacement triangle et losange
        {
            if (cursor().shape() == Qt::ClosedHandCursor)
            {
                setCursor(Qt::ArrowCursor);
            }
        }
        scene()->clearSelection();
        setSelected(true);
        update();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void tache_item::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    setToolTip(getComment());
}

void tache_item::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (getMode() == 1)
    {
        //mode sans déplacement
    }
    else
    {
        if (getMode() == 2)
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
        if (getMode() == 3)
        {
            if (event->pos().x() < (m_width / 2) && event->pos().y() < (m_height / 2))
            {
                setCursor(Qt::OpenHandCursor);
            }
        }
    }
}

void tache_item::setTexte(const QString text)
{
    itemText = text;
}

QString tache_item::getTexte()
{
    return itemText;
}

void tache_item::setComment(const QString comment)
{
    itemComment = comment;
}

QString tache_item::getComment()
{
    return itemComment;
}

int tache_item::getTypeAction()
{
    return typeAction;
}

void tache_item::setTypeAction(int typeAct)
{
    typeAction = typeAct;
}

QString tache_item::getAction()
{
    return action;
}

void tache_item::setAction(QString act)
{
    action = act;
}

QString tache_item::getPixFile()
{
    return pixmapfileName;
}

void tache_item::setPixFile(QString file)
{
    pixmapfileName = file;
}
