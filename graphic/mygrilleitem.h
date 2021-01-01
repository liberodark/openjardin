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
#ifndef MYGRILLEITEM_H
#define MYGRILLEITEM_H

#include <QtGui>
#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QPainterPath>

class mygrilleitem : public QGraphicsRectItem
{
public:
    enum { Type = UserType + 4 };     //65540
    // userType = 65536 valeur par défaut

    mygrilleitem(int caseL, int caseH);

    ~mygrilleitem();

    // Couleur de la grille
    QColor caseColor;
    QColor penColor;
    QColor brushColor;
    // Coordonnées de la case de la grille
    int caseL;
    int caseH;
    QString itemText;

    /* Méthodes */

    // Retourne la forme de la case (item)
    QPainterPath shape() const;

    // Zone du rectangle à représenter
    QRectF boundingRect() const;

    // Fonction virtuelle de QGraphicsItem appelée par QGraphicsView
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

protected:

public:
    // Fonction qui renvoie la couleur de la case
    QColor getColor()
    {
        return caseColor;
    }

    // Fonction qui définie une nouvelle couleur pour la case et mets à jour l'affichage
    void setColor(const QColor&color)
    {
        caseColor = color;
        update();
    }

    // Fonction qui définie une nouvelle couleur du crayon de la case
    void setPenColor(const QColor&color)
    {
        penColor = color;
    }

    // Fonction qui renvoie la couleur du crayon de la case
    QColor getPenColor()
    {
        return penColor;
    }

    void setText(const QString texte)
    {
        itemText = texte;
    }

    QString getText()
    {
        return itemText;
    }
};

#endif // MYGRILLEITEM_H
