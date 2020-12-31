// myitem.cpp

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

MyItem::MyItem(qreal wid, qreal hgt)
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

QRectF MyItem::boundingRect() const
{
    return QRectF(-m_width / 2 - 1, -m_height / 2 - 1, m_width, m_height);
}

QPainterPath MyItem::shape() const
{
    QPainterPath path;

    path.addRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));
    return path;
}

void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    case MyItem::Rectangle:
        painter->drawRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));
        break;

    case MyItem::RoundedRectangle:
        painter->drawRoundedRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height), 10, 10);
        break;

    case MyItem::Circle:
        painter->drawEllipse(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));         //à verifier
        break;

    case MyItem::Image:
        QImage pixm(pixmapfileName);
        painter->drawImage(QRectF(-m_width / 2, -m_height / 2, m_width, m_height), pixm);        //test pixmap
        break;
    }
    painter->drawText(rect, Qt::AlignCenter, getNom());
}

void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void MyItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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

void MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void MyItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    setToolTip(getTexte());
}

void MyItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
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

QVariant MyItem::itemChange(GraphicsItemChange change, const QVariant&value)
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

void MyItem::setTexte(const QString text)
{
    itemText = text;
}

QString MyItem::getTexte()
{
    return itemText;
}

void MyItem::setComment(const QString comment)
{
    itemComment = comment;
}

QString MyItem::getComment()
{
    return itemComment;
}

int MyItem::getTypeAction()
{
    return typeAction;
}

void MyItem::setTypeAction(int typeAct)
{
    typeAction = typeAct;
}

QString MyItem::getAction()
{
    return action;
}

void MyItem::setAction(QString act)
{
    action = act;
}

QString MyItem::getPixFile()
{
    return pixmapfileName;
}

void MyItem::setPixFile(QString file)
{
    pixmapfileName = file;
}

QString MyItem::getDetailFile()
{
    return detailfileName;
}

void MyItem::setDetailFile(QString file)
{
    detailfileName = file;
}

QUuid MyItem::getUuid() const
{
    return m_Uuid;
}

void MyItem::setUuid(const QUuid&uuid)
{
    m_Uuid = uuid;
}
