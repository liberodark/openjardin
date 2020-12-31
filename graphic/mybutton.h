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
#ifndef MYBUTTON_H
#define MYBUTTON_H


#include <QToolButton>
#include <QEvent>

class Button : public QToolButton
{
    Q_OBJECT

public:
    explicit Button(const QString&text, QWidget *parent = 0);

    QSize sizeHint() const Q_DECL_OVERRIDE;


    void setNom(const QString nom)
    {
        m_BoutonNom = nom;
    }

    QString getNom()
    {
        return m_BoutonNom;
    }

    void setFamille(const int&famille)
    {
        m_itemFamille = famille;
    }

    int getFamille()
    {
        return m_itemFamille;
    }

    void setCodeArticle(const int&codeArticle)
    {
        m_codeArticle = codeArticle;
    }

    int getCodeArticle()
    {
        return m_codeArticle;
    }

    void setPV(const float&PrixVente)
    {
        m_prixVente = PrixVente;
    }

    float getPV()
    {
        return m_prixVente;
    }

    void setUnite(const int&unite)
    {
        m_unite = unite;
    }

    int getunite()
    {
        return m_unite;
    }

    void setTaux(const int&taux)
    {
        m_taux = taux;
    }

    int getTaux()
    {
        return m_taux;
    }

    void setId(const int&Id)
    {
        m_Id = Id;
    }

    int getId()
    {
        return m_Id;
    }

private slots:


protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);


private:
    QString m_BoutonNom;
    int     m_itemFamille;
    int     m_codeArticle;
    float   m_prixVente;
    int     m_unite;
    int     m_taux;
    int     m_Id;
};



#endif // MYBUTTON_H
