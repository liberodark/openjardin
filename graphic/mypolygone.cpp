//mypolygone.h

/****************************************************************************
**
** Copyright (C) 2017 openjardin.eu.
** Contact: https://openjardin.eu
**
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
#include "mypolygone.h"
#include "mainwindow.h"
#include "myvertex.h"
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QtWidgets>
#include <QPolygon>
#include <QVector>
#include <QPoint>

MyPolygone::MyPolygone(const QPolygon&poly)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsPathItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    m_poly      = poly;
    brushColor  = Qt::blue;
    penColor    = Qt::black;
    m_opacity   = 1.0; //transparence
    m_typeLine  = 1;
    m_widthLine = 1;
    m_modif     = 0;
}

QRectF MyPolygone::boundingRect() const
{   //calcul du rectangle de délimitation du polygone
    if (m_poly.isEmpty())
    {
        return QRect(0, 0, 0, 0);
    }
    else
    {
        const QPoint *pd = m_poly.constData();
        int           minx, maxx, miny, maxy;
        minx = maxx = pd->x();
        miny = maxy = pd->y();
        ++pd;
        for (int i = 1; i < m_poly.size(); ++i)
        {
            if (pd->x() < minx)
            {
                minx = pd->x();
            }
            else if (pd->x() > maxx)
            {
                maxx = pd->x();
            }
            if (pd->y() < miny)
            {
                miny = pd->y();
            }
            else if (pd->y() > maxy)
            {
                maxy = pd->y();
            }
            ++pd;
        }
        return QRect(QPoint(minx - m_widthLine, miny - m_widthLine), QPoint(maxx + m_widthLine, maxy + m_widthLine));
    }
}

QPainterPath MyPolygone::shape() const
{
    QPainterPath path;

    path.addRect(boundingRect());
    // qDebug() << "boundingrect "<< boundingRect() <<" path  "<<path;
    return path;
}

void MyPolygone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    Q_UNUSED(option);
    Q_UNUSED(widget);
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
    QPainterPath path;
    path.addPolygon(m_poly);
    // Draw polygon
    painter->drawPolygon(QPolygon(m_poly));
    painter->fillPath(path, brushColor);
    painter->drawText(rect, Qt::AlignCenter, getNom());
}

void MyPolygone::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (getMode() == 1) // mode utilisation
    {
        scene()->clearSelection();
        setSelected(true);
    }
    else //mode modification plan
    {
        if (getModif() == 0)
        {
            scene()->clearSelection();
            setSelected(true);
            setFocus();
            setCursor(Qt::ClosedHandCursor);
            mousePressPt.setX(int(pos().x() - event->scenePos().x()));
            mousePressPt.setY(int(pos().y() - event->scenePos().y()));
            update();
        }
    }

    QGraphicsItem::mousePressEvent(event);
}

void MyPolygone::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (getMode() == 1)
    {
        //mode utilisation
    }
    else //mode modification plan
    {
        if (cursor().shape() == Qt::ClosedHandCursor && getModif() == 0)
        {
            setX(mousePressPt.x() + event->scenePos().x());
            setY(mousePressPt.y() + event->scenePos().y());
        }
        event->accept();
        update();
    }
}

void MyPolygone::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (getMode() == 1)
    {
        // mode utilisation
    }
    else //mode modification plan
    {
        if (cursor().shape() == Qt::ClosedHandCursor)
        {
            setCursor(Qt::ArrowCursor);
        }
        update();
    }
    QGraphicsItem::mouseReleaseEvent(event);

    update();
}

void MyPolygone::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    //   setToolTip(getComment());
}

void MyPolygone::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (getMode() != 1 &&
        getMode() != 2 && getModif() != 1)
    {
        setCursor(Qt::OpenHandCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}

QVariant MyPolygone::itemChange(GraphicsItemChange change, const QVariant&value)
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
     * }
     *
     **/
    return QGraphicsItem::itemChange(change, value);
}

void MyPolygone::setTexte(const QString text)
{
    itemText = text;
}

QString MyPolygone::getTexte()
{
    return itemText;
}

void MyPolygone::setComment(const QString comment)
{
    itemComment = comment;
}

QString MyPolygone::getComment()
{
    return itemComment;
}

int MyPolygone::getTypeAction()
{
    return typeAction;
}

void MyPolygone::setTypeAction(int typeAct)
{
    typeAction = typeAct;
}

QString MyPolygone::getAction()
{
    return action;
}

void MyPolygone::setAction(QString act)
{
    action = act;
}

QUuid MyPolygone::getUuid() const
{
    return m_Uuid;
}

void MyPolygone::setUuid(const QUuid&uuid)
{
    m_Uuid = uuid;
}

QString MyPolygone::getDetailFile()
{
    return detailfileName;
}

void MyPolygone::setDetailFile(QString file)
{
    detailfileName = file;
}
