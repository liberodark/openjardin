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

#include "mygrilleitem.h"

mygrilleitem::mygrilleitem(int largeur, int hauteur)
{
    caseL = largeur;
    caseH = hauteur; //initialisation des valeurs largeur - hauteur
}

mygrilleitem::~mygrilleitem()
{
};



// Retourne la forme de la case (item)
QPainterPath mygrilleitem::shape() const
{
    QPainterPath path;

    path.addRect(QRectF(-caseL / 2, -caseH / 2, caseL, caseH));
    return path;
}

// Zone du rectangle à représenter
QRectF mygrilleitem::boundingRect() const
{
    return QRectF(-caseL / 2 - 1, -caseH / 2 - 1, caseL, caseH);
}

// Fonction virtuelle de QGraphicsItem appelée par QGraphicsView
void mygrilleitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF rect = boundingRect();
    QFont  f    = painter->font();
    f.setPixelSize(8);
    painter->setFont(f);
    painter->setBrush(caseColor);
    QPen pen(penColor, 1);
    painter->setPen(pen);
    painter->drawRect(QRectF(-caseL / 2 - 1, -caseH / 2 - 1, caseL, caseH));
    painter->drawText(rect, Qt::AlignCenter, getText());
}
