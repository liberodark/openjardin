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
#ifndef PARCELLE_H
#define PARCELLE_H

#include <QGraphicsItem>
#include <QColor>
#include <QUuid>
#include <QString>

class Parcelle
{
public:
    Parcelle();

    int getType() const;
    void setType(const int&type);

    int getSizeX() const;
    void setSizeX(const int&sizeX);

    int getSizeY() const;
    void setSizeY(const int&sizeY);

    int getPosX() const;
    void setPosX(const int&posX);

    int getPosY() const;
    void setPosY(const int&posY);

    QString getChemin() const;
    void setChemin(const QString&chemin);


    QString getPen() const;
    void setPen(const QString&pen);

    QString getBrush() const;
    void setBrush(const QString&brush);

    int getThickness() const;
    void setThickness(const int&thickness);

    QString getComment() const;
    void setComment(const QString&commentaire);

    QString getNom() const;
    void setNom(const QString&nom);

    int getId() const;
    void setId(const int&id);

    int getForme() const;
    void setForme(const int&forme);

    int getMode() const;
    void setMode(const int&mode);

    int getEtat() const;
    void setEtat(const int&etat);

    QString getTexte() const;
    void setTexte(const QString&texte);

    void setTypeLine(const int&typeLine);
    int getTypeLine();

    void setWidthLine(const int&widthLine);
    int getWidthLine();


private:
    int mId              = 0;
    QString mNom         = "";
    int mType            = -1;
    int mSizeX           = 0;
    int mSizeY           = 0;
    int mPosX            = -1;
    int mPosY            = -1;
    int mForme           = 1;
    QString mCommentaire = "";
    QString mTexte       = "";
    int mMode            = 1;
    int mEtat            = 1;
    QString mChemin      = "";
    QString mPen;
    QString mBrush;
    int m_typeLine;
    int m_widthLine;
};

#endif // PARCELLE_H
