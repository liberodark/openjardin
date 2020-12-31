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

#include "planning_item.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QtWidgets>


planning_item::planning_item(qreal wid, qreal hgt)
{
    m_width    = wid; m_height = hgt; //initialisation des valeurs largeur - hauteur
    brushColor = Qt::white;
    penColor   = Qt::black;

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsPathItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QRectF planning_item::boundingRect() const
{
    return QRectF(-m_width / 2 - 1, -m_height / 2 - 1, m_width, m_height);
}

QPainterPath planning_item::shape() const
{
    QPainterPath path;

    path.addRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));
    return path;
}

void planning_item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(brushColor);
    QPen pen(penColor, 1);
    painter->setPen(pen);
    if (typeShape == 1) //rectangle
    {
        painter->drawRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));
    }
    if (typeShape == 2) //roundedrectangle
    {
        painter->drawRoundedRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height), 10, 10);
    }
    if (typeShape == 3)                                                               //circle
    {
        painter->drawEllipse(QRectF(-m_width / 2, -m_height / 2, m_width, m_height)); //à verifier
    }
    if (typeShape == 4)                                                               //image pixmap
    {
        QImage pixm(pixmapfileName);
        painter->drawImage(QRectF(-m_width / 2, -m_height / 2, m_width, m_height), pixm); // pixmap
    }
    painter->drawText(rect, Qt::AlignCenter, getNom());
}

void planning_item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (getMode() == 1)
    {
        scene()->clearSelection();
        setSelected(true);
    }
    else
    {
        if (getMode() == 2) //mode modification planning
        {
            setOldX(pos().x() - (m_width / 2));
            setOldY(pos().y() - (m_height / 2));
            setSelected(true);
            setFocus();
            const QPointF event_pos = event->pos();
            //    const QPointF event_scene_pos = event->scenePos();
            if (event_pos.x() < (m_width / 2) - 10 && event_pos.y() < (m_height / 2) - 10)
            {
                setCursor(Qt::ClosedHandCursor);
                mousePressPt.setX(int(pos().x() - event->scenePos().x()));
            }
            else
            {
                setCursor(Qt::SizeFDiagCursor);
            }
            update();
        }
        else //mode modification plan
        {
            setOldX(pos().x() - (m_width / 2));
            setOldY(pos().y() - (m_height / 2));
            qDebug() << "old position x y" << getOldX() << " " << getOldY();
            setSelected(true);
            setFocus();

            const QPointF event_pos = event->pos();
            //     const QPointF event_scene_pos = event->scenePos();
            qDebug() << "position in " << event_pos.x() << "width " << m_width;

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

void planning_item::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (getMode() == 1)
    {
        //mode utilisation
    }
    else
    {
        if (getMode() == 2) //mode modification planning
        {
            if (cursor().shape() == Qt::ClosedHandCursor)
            {
                setX(mousePressPt.x() + event->scenePos().x());
            }
            else
            {
                setOpacity(0.5);

                double ObjetWidth  = event->scenePos().x() - pos().x();
                double ObjetHeight = event->scenePos().y() - pos().y();
                if (ObjetWidth > 0 && ObjetHeight > 0)
                {
                    ObjetWidth  = ObjetWidth + (m_width / 2);
                    ObjetHeight = m_height;
                    setSizeShape(ObjetWidth, ObjetHeight);
                    setPos(getOldX() + (ObjetWidth / 2), getOldY() + (ObjetHeight / 2));
                }
            }
            event->accept();
            update();
        }
        else    //mode modification plan
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
                    setPos(getOldX() + (ObjetWidth / 2), getOldY() + (ObjetHeight / 2));
                }
            }
            event->accept();
            update();
        }
    }
}

void planning_item::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (getMode() == 1)
    {
        //mode utilisation
    }
    else
    {
        if (getMode() == 2) //mode modification planning
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
                    ObjetHeight = m_height;
                    setPos(getOldX() + (ObjetWidth / 2), getOldY() + (ObjetHeight / 2));
                    setSizeShape(ObjetWidth, ObjetHeight);
                }
                setCursor(Qt::ArrowCursor);
            }
            update();
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

void planning_item::setTexte(const QString text)
{
    itemText = text;
}

QString planning_item::getTexte()
{
    return itemText;
}

void planning_item::setComment(const QString comment)
{
    itemComment = comment;
}

QString planning_item::getComment()
{
    return itemComment;
}

int planning_item::getTypeAction()
{
    return typeAction;
}

void planning_item::setTypeAction(int typeAct)
{
    typeAction = typeAct;
}

QString planning_item::getAction()
{
    return action;
}

void planning_item::setAction(QString act)
{
    action = act;
}

QString planning_item::getPixFile()
{
    return pixmapfileName;
}

void planning_item::setPixFile(QString file)
{
    pixmapfileName = file;
}
