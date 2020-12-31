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
#ifndef PLANNING_ITEM_H
#define PLANNING_ITEM_H
#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QPainterPath>



class planning_item : public QGraphicsItem
{
public:
    planning_item(qreal wid, qreal hgt);

    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QPainterPath shape() const;

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

    void setAngle(const int&angle)
    {
        m_angle = angle;
    }

    int getAngle()
    {
        return m_angle;
    }

    void setNom(const QString nom)
    {
        itemNom = nom;
    }

    QString getNom()
    {
        return itemNom;
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
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    int m_itemId;
    int typeAction;
    QString action;
    QColor brushColor;
    QColor penColor;
    qreal m_width;
    qreal m_height;
    int m_angle;
    qreal m_oldX;
    qreal m_oldY;
    QString itemNom;
    QString itemText;
    QString itemComment;
    int typeShape;
    QString pixmapfileName;
    int m_etat;
    int m_mode;     //0 modification - 1 utilisation
};

#endif // PLANNING_ITEM_H
