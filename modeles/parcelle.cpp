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

#include "parcelle.h"


#include <QtGlobal>


Parcelle::Parcelle()
{
}

int Parcelle::getType() const
{
    return mType;
}

void Parcelle::setType(const int&type)
{
    mType = type;
}

int Parcelle::getSizeX() const
{
    return mSizeX;
}

void Parcelle::setSizeX(const int&sizeX)
{
    mSizeX = sizeX;
}

int Parcelle::getSizeY() const
{
    return mSizeY;
}

void Parcelle::setSizeY(const int&sizeY)
{
    mSizeY = sizeY;
}

int Parcelle::getPosX() const
{
    return mPosX;
}

void Parcelle::setPosX(const int&posX)
{
    mPosX = posX;
}

int Parcelle::getPosY() const
{
    return mPosY;
}

void Parcelle::setPosY(const int&posY)
{
    mPosY = posY;
}

QString Parcelle::getChemin() const
{
    return mChemin;
}

void Parcelle::setChemin(const QString&chemin)
{
    mChemin = chemin;
}

QString Parcelle::getPen() const
{
    return mPen;
}

void Parcelle::setPen(const QString&pen)
{
    mPen = pen;
}

QString Parcelle::getBrush() const
{
    return mBrush;
}

void Parcelle::setBrush(const QString&brush)
{
    mBrush = brush;
}

void Parcelle::setTypeLine(const int&typeLine)
{
    m_typeLine = typeLine;
}

int Parcelle::getTypeLine()
{
    return m_typeLine;
}

void Parcelle::setWidthLine(const int&widthLine)
{
    m_widthLine = widthLine;
}

int Parcelle::getWidthLine()
{
    return m_widthLine;
}

QString Parcelle::getComment() const
{
    return mCommentaire;
}

void Parcelle::setComment(const QString&commentaire)
{
    mCommentaire = commentaire;
}

QString Parcelle::getNom() const
{
    return mNom;
}

void Parcelle::setNom(const QString&nom)
{
    mNom = nom;
}

int Parcelle::getId() const
{
    return mId;
}

void Parcelle::setId(const int&id)
{
    mId = id;
}

int Parcelle::getForme() const
{
    return mForme;
}

void Parcelle::setForme(const int&forme)
{
    mForme = forme;
}

int Parcelle::getMode() const
{
    return mMode;
}

void Parcelle::setMode(const int&mode)
{
    mMode = mode;
}

int Parcelle::getEtat() const
{
    return mEtat;
}

void Parcelle::setEtat(const int&etat)
{
    mEtat = etat;
}

QString Parcelle::getTexte() const
{
    return mTexte;
}

void Parcelle::setTexte(const QString&texte)
{
    mTexte = texte;
}
