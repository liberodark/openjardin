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

#include "cultures.h"
#include "ui_cultures.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphic/myitem.h"
#include "dialogs/fiche_plantes.h"
#include "dialogs/dialog_taches.h"
#include "utilitaires/util.h"
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QtSql>
#include <QtWidgets>
#include <QDebug>
#include <QMessageBox>

Cultures::Cultures(const int&IdItem, const int&IdCulture, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Cultures)
{
    // translator
    QTranslator translator;
    QString     fichier = ":/translations/open-jardin_" + util::getLocale();
    QSettings settings;
    QFile        iniFile(settings.fileName());
    if (iniFile.exists())
    {
        QSettings settings(iniFile.fileName(), QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        QString langue = settings.value("langue").toString();
        if(langue == "english")
            {
            // forcer la langue anglaise
                 fichier = ":/translations/open-jardin_en.ts";

             }

    }
    translator.load(fichier);
    qApp->installTranslator(&translator);

    ui->setupUi(this);
    setIdItem(IdItem);     // id de la parcelle
    setIdCulture(IdCulture);
    ui->lineEditIdParcelle->setText(QString::number(m_idItem));
    ui->lineEdit_id_cultures->setText(QString::number(m_idCulture));
    ui->lineEdit_id_tache->hide();
    // mise à la date du jour du dateEdit de la fiche
    QDate dateDuJour;
    dateDuJour = dateDuJour.currentDate();
    ui->dateEdit->setDate(dateDuJour);
    ui->dateEdit_tache->setDate(dateDuJour);
    // mise à jour date de récolte selon durée prévisionnelle
    QString str1  = ui->dateEdit->date().toString("yyyy.MM.dd");
    QDate   dateS = QDate::fromString(str1, "yyyy.MM.dd");
    QDate   dateR = dateS.addDays(ui->lineEdit_duree->text().toInt());
    ui->dateEdit_fin_culture->setDate(dateR);

    //remplissage combobox espèces type de plantes
    QSqlQueryModel *modelM = new QSqlQueryModel;
    modelM->setQuery("SELECT designation,type_lune FROM plantes ORDER BY designation ASC");
    ui->comboBox_plante->setModel(modelM);

    //remplissage combobox type taches
    QSqlQueryModel *modelTache = new QSqlQueryModel;
    modelTache->setQuery("SELECT designation FROM taches ORDER BY designation ASC");
    ui->comboBox_type_tache->setModel(modelTache);
    init_models(IdCulture);
}

void Cultures::init_models(int idculture)
{
    if (idculture == 0)
    {
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT id, designation, parcelle, date_semis, type_plante , commentaires , etat,duree,date_recolte FROM cultures WHERE parcelle=" + QString::number(
                            m_idItem) + " ORDER BY id DESC");
        model->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        model->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        model->setHeaderData(3, Qt::Vertical, QObject::tr("parcelle"));
        ui->tableViewCultures->setModel(model);
        ui->tableViewCultures->setColumnWidth(1, 200);
        ui->tableViewCultures->setColumnWidth(5, 400);
        for (int i = 2; i < 9; i++)
        {
            ui->tableViewCultures->setColumnHidden(i, true);
        }
        ui->tableViewCultures->setColumnHidden(0, true);
        ui->tableViewCultures->setColumnHidden(3, false);
        ui->tableViewCultures->setColumnHidden(5, false);
        QSqlQueryModel *modelObs = new QSqlQueryModel;
        modelObs->setQuery("SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + QString::number(
                               idculture) + " ORDER BY id DESC");
        modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
        ui->tableView_taches->setModel(modelObs);
        ui->tableView_taches->setColumnWidth(1, 200);
        ui->tableView_taches->setColumnWidth(4, 400);
        ui->tableView_taches->setColumnHidden(0, true);
        ui->tableView_taches->setColumnHidden(3, true);
        ui->tableView_taches->setColumnHidden(5, true);
    }
    else
    {
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT id, designation, parcelle, date_semis, type_plante , commentaires , etat,duree,date_recolte FROM cultures WHERE id =" + QString::number(
                            idculture) + " ORDER BY id DESC");
        model->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        model->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        model->setHeaderData(3, Qt::Vertical, QObject::tr("parcelle"));
        ui->tableViewCultures->setModel(model);
        ui->tableViewCultures->setColumnWidth(1, 200);
        ui->tableViewCultures->setColumnWidth(5, 400);
        for (int i = 2; i < 9; i++)
        {
            ui->tableViewCultures->setColumnHidden(i, true);
        }
        ui->tableViewCultures->setColumnHidden(0, true);
        ui->tableViewCultures->setColumnHidden(3, false);
        ui->tableViewCultures->setColumnHidden(5, false);

        QSqlQueryModel *modelObs = new QSqlQueryModel;
        modelObs->setQuery("SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + QString::number(
                               idculture) + " ORDER BY id DESC");
        modelObs->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
        ui->tableView_taches->setModel(modelObs);
        ui->tableView_taches->setColumnWidth(1, 200);
        ui->tableView_taches->setColumnWidth(4, 400);
        ui->tableView_taches->setColumnHidden(0, true);
        ui->tableView_taches->setColumnHidden(3, true);
        ui->tableView_taches->setColumnHidden(5, true);

        int     row   = 0;
        QString strId = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 0)).toString();
        QString str   = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 1)).toString();
        QString str1  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 3)).toString();
        QString str2  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 4)).toString();
        QString str3  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 5)).toString();
        QString str4  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 6)).toString();
        QString str5  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 7)).toString();
        QString str6  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 8)).toString();
        ui->lineEditDesignation->setText(str);
        QDate date = QDate::fromString(str1, "yyyy.MM.dd");
        ui->dateEdit->setDate(date);
        QDate date2 = QDate::fromString(str6, "yyyy.MM.dd");
        ui->dateEdit_fin_culture->setDate(date2);
        ui->lineEdit_duree->setText(str5);
        //selection plante dans combobox
        QSqlQuery query;
        QString   resultat;
        query.exec(QString("select designation from plantes where id =" + str2));
        if (query.first())
        {
            resultat = query.value(0).toString();

            ui->comboBox_plante->setCurrentIndex(ui->comboBox_plante->findText(resultat));
        }
        ui->comboBox_etat_culture->setCurrentIndex(str4.toInt() - 1);
        ui->plainTextEdit_commentaires->setPlainText(str3);
    }
}

Cultures::~Cultures()
{
    delete ui;
}

void Cultures::on_pushButton_nouvelleOperation_clicked()
{   //création d'une fiche de culture vierge
    ui->lineEditDesignation->setText("");
    ui->plainTextEdit_commentaires->setPlainText("");
    ui->comboBox_plante->setCurrentIndex(0);
    ui->lineEdit_id_cultures->setText("");
}

void Cultures::on_pushButton_validerData_clicked()
{   //ajouter un enregistrement de la table "cultures" valider les données et enregistrer dans la base
    QSqlQuery query;
    QString   nom_plante = util::apos(ui->comboBox_plante->currentText());
    QString   id_plante;

    query.exec(QString("select id from plantes where designation ='" + nom_plante + "'"));
    if (query.first())
    {
        id_plante = query.value(0).toString();
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                 tr("Veuillez vérifier que tous les champs soient bien remplis"));
    }
    QString str1 = ui->dateEdit->date().toString("yyyy.MM.dd");
    QString str2 = id_plante;
    QString str3 = util::apos(ui->plainTextEdit_commentaires->document()->toPlainText());
    QString str4 = QString::number(ui->comboBox_etat_culture->currentIndex() + 1);
    QString str5 = ui->lineEdit_duree->text();
    QString str6 = ui->dateEdit_fin_culture->date().toString("yyyy.MM.dd");
    QString str  = "insert into cultures (designation,parcelle,date_semis,type_plante,commentaires,etat,duree,date_recolte)"
                   "values('" + util::apos(ui->lineEditDesignation->text()) + "'," + ui->lineEditIdParcelle->text() + ",'" +
                   str1 +
                   "'," + str2 + ",'" + str3 + "'," + str4 + "," + str5 + ",'" + str6 + "')";
    query.exec(str);
    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                 tr("Veuillez vérifier que tous les champs soient bien remplis"));
    }
    else
    {
        qDebug() << "enregistrement terminé";
    }
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT id, designation, parcelle, date_semis, type_plante , commentaires,etat,duree,date_recolte FROM cultures WHERE parcelle=" + QString::number(
                        m_idItem) + " ORDER BY id DESC");
    model->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model->setHeaderData(3, Qt::Vertical, QObject::tr("parcelle"));
    ui->tableViewCultures->setModel(model);
    ui->tableViewCultures->setColumnWidth(1, 200);
    ui->tableViewCultures->setColumnWidth(5, 400);
    for (int i = 2; i < 9; i++)
    {
        ui->tableViewCultures->setColumnHidden(i, true);
    }
    ui->tableViewCultures->setColumnHidden(0, true);
    ui->tableViewCultures->setColumnHidden(3, false);
    ui->tableViewCultures->setColumnHidden(5, false);
}

void Cultures::on_tableViewCultures_clicked(const QModelIndex&index)
{
    int     row   = index.row();
    QString strId = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 0)).toString();
    QString str   = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 1)).toString();
    QString str1  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 3)).toString();
    QString str2  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 4)).toString();
    QString str3  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 5)).toString();
    QString str4  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 6)).toString();
    QString str5  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 7)).toString();
    QString str6  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 8)).toString();

    ui->lineEditDesignation->setText(str);
    QDate date = QDate::fromString(str1, "yyyy.MM.dd");
    ui->dateEdit->setDate(date);
    QDate date2 = QDate::fromString(str6, "yyyy.MM.dd");
    ui->dateEdit_fin_culture->setDate(date2);
    ui->lineEdit_duree->setText(str5);
    //selection plante dans combobox
    QSqlQuery query;
    QString   resultat;
    query.exec(QString("select designation from plantes where id =" + str2));
    if (query.first())
    {
        resultat = query.value(0).toString();
        ui->comboBox_plante->setCurrentIndex(ui->comboBox_plante->findText(resultat));
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                 tr("Veuillez vérifier que tous les champs soient bien remplis"));
    }
    ui->comboBox_etat_culture->setCurrentIndex(str4.toInt() - 1);
    ui->plainTextEdit_commentaires->setPlainText(str3);
    ui->lineEdit_id_cultures->setText(strId);
}

void Cultures::on_comboBox_plante_currentIndexChanged(const QString&arg1)
{
    QSqlQuery query;
    QString   id_plante;
    QString   resultat;
    QString   id_espece;
    QString   design_espece;
    QString   id_famille;
    QString   design_famille;

    query.exec(QString("select id from plantes where designation ='" + util::apos(arg1) + "'"));
    if (query.first())
    {
        id_plante = query.value(0).toString();
        ui->lineEditTypeLune->clear(); //effacer
        query.exec(QString("select type_lune from plantes where id=" + id_plante));
        if (query.first())
        {
            resultat = query.value(0).toString();
        }
        else
        {
            qWarning("ne peut récupérer la valeur type lune");
        }

        ui->lineEditTypeLune->setText(resultat);

        query.exec(QString("select espece from plantes where id=" + id_plante));
        if (query.first())
        {
            id_espece = query.value(0).toString();
        }
        else
        {
            qWarning("ne peut récupérer la valeur espece");
        }

        query.exec(QString("select designation from especes where id=" + id_espece));
        if (query.first())
        {
            design_espece = query.value(0).toString();
        }
        else
        {
            qWarning("ne peut récupérer la valeur designation espece");
        }

        query.exec(QString("select famille from especes where id=" + id_espece));
        if (query.first())
        {
            id_famille = query.value(0).toString();
        }
        else
        {
            qWarning("ne peut récupérer la valeur famille espece");
        }

        query.exec(QString("select designation from familles where id=" + id_famille));
        if (query.first())
        {
            design_famille = query.value(0).toString();
        }
        else
        {
            qWarning("ne peut récupérer la valeur designation famille");
        }

        ui->lineEdit_Fiche_culture_espece->setText(design_espece);
        ui->lineEdit_Fiche_culture_famille->setText(design_famille);
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
}

void Cultures::on_lineEditTypeLune_textChanged(const QString&arg1)
{
    QSqlQuery query;
    QString   resultat;
    int       row = arg1.toInt();

    ui->lineEditDesign_Lune->clear(); //effacer
    query.exec(QString("select designation from lune where id=" + QString::number(row)));
    if (query.first())
    {
        resultat = query.value(0).toString();
    }


    ui->lineEditDesign_Lune->setText(resultat);

    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
}

void Cultures::on_pushButton_modifier_clicked()
{
    //enregister la fiche modifiée
    QSqlQuery query;
    QString   nom_plante = util::apos(ui->comboBox_plante->currentText());
    QString   id_plante;

    query.exec(QString("select id from plantes where designation ='" + nom_plante + "'"));
    if (query.first())
    {
        id_plante = query.value(0).toString();
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                 tr("Veuillez vérifier que tous les champs soient bien remplis"));
    }
    QString str1 = ui->dateEdit->date().toString("yyyy.MM.dd");
    QString str  = util::apos(ui->lineEditDesignation->text());
    QString str2 = id_plante;
    QString str3 = util::apos(ui->plainTextEdit_commentaires->document()->toPlainText());
    QString str4 = ui->lineEdit_id_cultures->text();
    QString str5 = ui->lineEditIdParcelle->text();
    QString str6 = QString::number(ui->comboBox_etat_culture->currentIndex() + 1);
    QString str7 = ui->lineEdit_duree->text();
    QString str8 = ui->dateEdit_fin_culture->date().toString("yyyy.MM.dd");

    QString strQuery = "update cultures set designation = '" + str + "',parcelle=" + str5 + ",date_semis= '" + str1 +
                       "',type_plante= " + str2 + ",commentaires='" + str3 + "',etat= " + str6 + ",duree=" + str7 +
                       ",date_recolte='" + str8 +
                       "' where id=" + str4;

    query.exec(strQuery);

    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                 tr("Veuillez vérifier que tous les champs soient bien remplis"));
    }
    else
    {
        qDebug() << "enregistrement terminé";
    }

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT id, designation, parcelle, date_semis, type_plante , commentaires,etat,duree,date_recolte FROM cultures WHERE parcelle=" + QString::number(
                        m_idItem) + " ORDER BY id DESC");
    model->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model->setHeaderData(3, Qt::Vertical, QObject::tr("parcelle"));
    ui->tableViewCultures->setModel(model);
    ui->tableViewCultures->setColumnWidth(1, 200);
    ui->tableViewCultures->setColumnWidth(5, 400);
    for (int i = 2; i < 9; i++)
    {
        ui->tableViewCultures->setColumnHidden(i, true);
    }
    ui->tableViewCultures->setColumnHidden(0, true);
    ui->tableViewCultures->setColumnHidden(3, false);
    ui->tableViewCultures->setColumnHidden(5, false);
}

/*************** base de données des taches et observations*************************/

void Cultures::on_pushButton_modifier_tache_clicked()
{
    //enregister la fiche modifiée
    QSqlQuery query;
    QString   nom_type_tache = ui->comboBox_type_tache->currentText();
    QString   id_type_tache;

    query.exec(QString("select id from taches where designation ='" + nom_type_tache + "'"));
    if (query.first())
    {
        id_type_tache = query.value(0).toString();
        qDebug() << nom_type_tache << " id " << id_type_tache;
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                 tr("Veuillez vérifier que tous les champs soient bien remplis"));
    }

    QString str1     = ui->dateEdit_tache->date().toString("yyyy.MM.dd");                            //date tache
    QString str      = util::apos(ui->lineEdit_designation_tache->text());                           //designation tache
    QString str2     = id_type_tache;                                                                // id du type de tache
    QString str3     = util::apos(ui->plainTextEdit_observations_taches->document()->toPlainText()); //observations
    QString str4     = ui->lineEdit_id_tache->text();                                                //id
    QString str5     = ui->lineEdit_id_cultures->text();                                             // id culture
    QString strQuery = "update observations set designation = '" + str + "',date='" + str1 + "',type= " + str2 +
                       ",commentaires='" + str3 + "',id_culture=" + str5 + " where id=" + str4;

    query.exec(strQuery);
    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                 tr("Veuillez vérifier que tous les champs soient bien remplis"));
    }
    else
    {
        qDebug() << "enregistrement terminé";
    }
    QSqlQueryModel *modelObs = new QSqlQueryModel;
    modelObs->setQuery(
        "SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + str5 +
        " ORDER BY id DESC");
    modelObs->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
    ui->tableView_taches->setModel(modelObs);
    ui->tableView_taches->setColumnWidth(1, 200);
    ui->tableView_taches->setColumnWidth(4, 400);
    ui->tableView_taches->setColumnHidden(0, true);
    ui->tableView_taches->setColumnHidden(3, true);
    ui->tableView_taches->setColumnHidden(5, true);
}

void Cultures::on_pushButton_creer_tache_clicked()
{
    //ajouter un enregistrement de la table "OBSERVATIONS" valider les données et enregistrer dans la base

    QSqlQuery query;
    QString   nom_type_tache = ui->comboBox_type_tache->currentText();
    QString   id_type_tache;

    query.exec(QString("select id from taches where designation ='" + nom_type_tache + "'"));
    if (query.first())
    {
        id_type_tache = query.value(0).toString();
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                 tr("Veuillez vérifier que tous les champs soient bien remplis"));
    }
    QString str1 = ui->dateEdit_tache->date().toString("yyyy.MM.dd");                            //date tache
    QString str  = util::apos(ui->lineEdit_designation_tache->text());                           //designation tache
    QString str2 = id_type_tache;                                                                // id du type de tache
    QString str3 = util::apos(ui->plainTextEdit_observations_taches->document()->toPlainText()); //observations
    QString str4 = ui->lineEdit_id_tache->text();                                                //id
    QString str5 = ui->lineEdit_id_cultures->text();                                             // id culture
    QString strQ = "insert into observations (designation, date,type , commentaires,id_culture)"
                   "values('" + str + "','" + str1 + "'," + str2 + ",'" + str3 + "'," + str5 + ")";
    query.exec(strQ);


    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                 tr("Veuillez vérifier que tous les champs soient bien remplis"));
    }
    else
    {
        qDebug() << "enregistrement terminé";
    }
    QSqlQueryModel *modelObs = new QSqlQueryModel;
    modelObs->setQuery(
        "SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + str5 +
        " ORDER BY id DESC");
    modelObs->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
    ui->tableView_taches->setModel(modelObs);
    ui->tableView_taches->setColumnWidth(1, 200);
    ui->tableView_taches->setColumnWidth(4, 400);
    ui->tableView_taches->setColumnHidden(0, true);
    ui->tableView_taches->setColumnHidden(3, true);
    ui->tableView_taches->setColumnHidden(5, true);
}

void Cultures::on_tableView_taches_clicked(const QModelIndex&index)
{
    int     row   = index.row();
    QString strId = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 0)).toString();
    QString str   = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 1)).toString();
    QString str1  = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 2)).toString();
    QString str2  = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 3)).toString();
    QString str3  = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 4)).toString();

    ui->lineEdit_id_tache->setText(strId);
    ui->lineEdit_designation_tache->setText(str);
    ui->plainTextEdit_observations_taches->document()->setPlainText(str3);
    QDate date = QDate::fromString(str1, "yyyy.MM.dd");
    ui->dateEdit_tache->show();
    ui->dateEdit_tache->setDate(date);

    //calage combo_type_tache sur valeur id
    QSqlQuery query;
    QString   resultat;
    query.exec(QString("select designation from taches where id =" + str2));
    if (query.first())
    {
        resultat = query.value(0).toString();
        ui->comboBox_type_tache->setCurrentIndex(ui->comboBox_type_tache->findText(resultat));
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
}

void Cultures::on_lineEdit_id_cultures_textChanged(const QString&arg1)
{
    QSqlQueryModel *modelObs = new QSqlQueryModel;

    modelObs->setQuery("SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + arg1);
    modelObs->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
    ui->tableView_taches->setModel(modelObs);
    ui->tableView_taches->setColumnWidth(1, 200);
    ui->tableView_taches->setColumnWidth(4, 400);
    ui->tableView_taches->setColumnHidden(0, true);
    ui->tableView_taches->setColumnHidden(3, true);
    ui->tableView_taches->setColumnHidden(5, true);

    //effacer les zones texte des tâches
    ui->lineEdit_designation_tache->setText("");
    ui->plainTextEdit_observations_taches->document()->setPlainText("");
    ui->lineEdit_id_tache->setText("");
    ui->dateEdit_tache->hide();

}

/*******************************IMPRESSION FICHE***********************/

void Cultures::on_pushButton_print_fiche_clicked()
{
    QString   titre   = " FICHE DE CULTURE ";
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);

    printer->setPaperSize(QPrinter::A4);
    // printer->setOutputFormat(QPrinter::NativeFormat);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOrientation(QPrinter::Portrait);
    printer->setFullPage(true);

    QPrintDialog printDialog(printer, this);
    if (printDialog.exec() == 1)
    {
        QTextBrowser *editor = new QTextBrowser;
        //creation de formats d'écriture
        QTextCharFormat NormalFormat;

        QTextCharFormat ItalicFormat;
        ItalicFormat.setFontItalic(true);

        //On insere la date et l'heure actuelle au début de la premiere page
        QDate date;
        QTime time;
        date = date.currentDate();
        time = time.currentTime();
        QString modif = "Fait le : " + date.toString("dddd dd MMMM yyyy") + " à " + time.toString();

        //changement du format d'ecriture
        editor->setCurrentCharFormat(ItalicFormat);
        editor->setAlignment(Qt::AlignLeft);

        //ajout de la QString a l'endroit du curseur
        editor->append(modif);
        editor->setCurrentCharFormat(NormalFormat);

        //on insere le titre du tableau
        QTextCharFormat format_gros_titre;
        format_gros_titre.setFontPointSize(20);
        format_gros_titre.setFontWeight(QFont::Bold);
        format_gros_titre.setVerticalAlignment(QTextCharFormat::AlignMiddle);
        //  format_gros_titre.setUnderlineStyle(QTextCharFormat::SingleUnderline);

        QString title = QString::fromUtf8(titre.toStdString().c_str());
        editor->setCurrentCharFormat(format_gros_titre);
        editor->setAlignment(Qt::AlignCenter);

        editor->append(title);

        QString parcelle = "Parcelle N° : " + ui->lineEditIdParcelle->text() + " \n";
        editor->append(parcelle);

        editor->setCurrentCharFormat(NormalFormat);
        //on crée un curseur a l'endroit du curseur actuel
        QTextCursor cursor = editor->textCursor();
        cursor.beginEditBlock();

        //Creation du format du tableau qui sera imprimer
        QTextTableFormat tableFormat;
        tableFormat.setAlignment(Qt::AlignHCenter);
        tableFormat.setAlignment(Qt::AlignLeft);
        tableFormat.setBackground(QColor("#ffffff"));
        tableFormat.setCellPadding(1);
        tableFormat.setCellSpacing(1);

        //Creation du tableau qui sera imprimé avec le nombre de colonne
        //et de ligne que contient le tableau mis en parametre
        QTextTable *tableau = cursor.insertTable(ui->tableViewCultures->model()->rowCount() + 3,
                                                 ui->tableViewCultures->model()->columnCount(), tableFormat);

        QTextFrame *     frame       = cursor.currentFrame();
        QTextFrameFormat frameFormat = frame->frameFormat();
        frameFormat.setBorder(0);
        frame->setFrameFormat(frameFormat);

        //Format des HEADER du tableau
        QTextCharFormat format_entete_tableau;
        format_entete_tableau.setFontPointSize(10);
        format_entete_tableau.setFontWeight(QFont::Bold);

        //Format du texte des cellules du tableau
        QTextCharFormat format_cellule;
        format_cellule.setFontPointSize(10);


        QTextTableCell cell;
        QTextCursor    cellCursor;
        //on ecrit les HEADERS du tableaux dans le tableau a imprimer
        for (int col = 0; col < tableau->columns(); col++)
        {
            QString texte = ui->tableViewCultures->model()->headerData(col, Qt::Horizontal).toString();
            cell       = tableau->cellAt(0, col);
            cellCursor = cell.firstCursorPosition();
            cellCursor.insertText(texte, format_cellule);
        }

        for (int row = 1; row < tableau->rows() - 2; row++)
        {
            for (int col = 0; col < tableau->columns(); col++)
            {
                QString texte1 = "";
                if (col == 1 || col == 2)
                {
                    texte1 = ui->tableViewCultures->model()->index(row - 1, col).data().toString();
                }
                else
                {
                    texte1 = QString::number(ui->tableViewCultures->model()->index(row - 1, col).data().toDouble());
                }
                if (col == 3)
                {
                    texte1 = ui->tableViewCultures->model()->index(row - 1, col).data().toDate().toString("dd/MM/yyyy");
                }
                if (col == 5)
                {
                    texte1 = ui->tableViewCultures->model()->index(row - 1, col).data().toString();
                }
                cell       = tableau->cellAt(row, col);
                cellCursor = cell.firstCursorPosition();
                cellCursor.insertText(texte1, format_cellule);
            }
        }

        //fin de l'edition
        cursor.endEditBlock();
        //impression de editor dans le QPrinter initialisé au début de la fonction
        editor->print(printer);
    }
}

void Cultures::on_pushButton_supprimer_culture_clicked()
{   // suppression d'une fiche de culture
    int ret = QMessageBox::warning(this, tr("Suppression d'une culture"),
                                   tr("Cette culture peut être supprimer.\n"
                                      "Confirmer la suppression de la culture"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel);

    switch (ret)
    {
    case QMessageBox::Ok:
    {
        QString   strId = ui->lineEdit_id_cultures->text();
        QString   str   = "delete from cultures where id=" + strId;
        QSqlQuery query;

        query.exec(str);
        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver();
        }
        else
        {
            qDebug() << "suppression terminée";
        }
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("SELECT id, designation, parcelle, date_semis, type_plante , commentaires,etat,duree,date_recolte FROM cultures WHERE parcelle=" + QString::number(
                            m_idItem) + " ORDER BY id DESC");
        model->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        model->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        model->setHeaderData(3, Qt::Vertical, QObject::tr("parcelle"));
        ui->tableViewCultures->setModel(model);
        ui->tableViewCultures->setColumnWidth(1, 200);
        ui->tableViewCultures->setColumnWidth(5, 400);
        for (int i = 2; i < 9; i++)
        {
            ui->tableViewCultures->setColumnHidden(i, true);
        }
        ui->tableViewCultures->setColumnHidden(0, true);
        ui->tableViewCultures->setColumnHidden(3, false);
        ui->tableViewCultures->setColumnHidden(5, false);
    }

    case QMessageBox::Cancel:
    {
        break;
    }

    default:
        break;
    }
}

void Cultures::on_pushButton_supprimer_tache_clicked()
{   //suppression d'une fiche de taches et observations
    QString   strId = ui->lineEdit_id_tache->text();
    QString   str   = "delete from observations where id=" + strId;
    QString   str5  = ui->lineEdit_id_cultures->text();  // id culture
    QSqlQuery query;

    query.exec(str);

    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    else
    {
        qDebug() << "suppression terminée";
    }

    QSqlQueryModel *modelObs = new QSqlQueryModel;
    modelObs->setQuery(
        "SELECT id, designation, date,type , commentaires,id_culture FROM observations where id_culture= " + str5 +
        " ORDER BY id DESC");
    modelObs->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    modelObs->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    modelObs->setHeaderData(3, Qt::Vertical, QObject::tr("date"));
    ui->tableView_taches->setModel(modelObs);
    ui->tableView_taches->setColumnWidth(1, 200);
    ui->tableView_taches->setColumnWidth(4, 400);
    ui->tableView_taches->setColumnHidden(0, true);
    ui->tableView_taches->setColumnHidden(3, true);
    ui->tableView_taches->setColumnHidden(5, true);
}

void Cultures::on_lineEdit_duree_textChanged(const QString&arg1)
{
    QString str1  = ui->dateEdit->date().toString("yyyy.MM.dd");
    QDate   dateS = QDate::fromString(str1, "yyyy.MM.dd");
    QDate   dateR = dateS.addDays(arg1.toInt());

    ui->dateEdit_fin_culture->setDate(dateR);
}

void Cultures::on_toolButton_FichePlantes_clicked()
{   // affichage de la fiche des variétés de plantes
    int Id          = ui->comboBox_plante->currentIndex();
    int Num_culture = ui->lineEdit_id_cultures->text().toInt();

    if (Id >= 0)
    {
        Fiche_plantes *fenetre_plante = new Fiche_plantes(Id);
        int            resultat       = fenetre_plante->exec();

        if (resultat == QDialog::Accepted)
        {
            QSqlQueryModel *modelM = new QSqlQueryModel;
            modelM->setQuery("SELECT designation,type_lune FROM plantes ORDER BY designation ASC");
            ui->comboBox_plante->setModel(modelM);
            init_models(Num_culture);
        }
    }
}

void Cultures::on_toolButton_NouvellePlante_clicked()
{   //ajouter une nouvelle variété
    // affichage de la fiche des variétés de plantes
    int Id          = 0;
    int Num_culture = ui->lineEdit_id_cultures->text().toInt();

    Fiche_plantes *fenetre_plante = new Fiche_plantes(Id);
    int            resultat       = fenetre_plante->exec();

    if (resultat == QDialog::Accepted)
    {
        Id = fenetre_plante->getIdPlante();

        QSqlQueryModel *modelM = new QSqlQueryModel;
        modelM->setQuery("SELECT designation,type_lune FROM plantes ORDER BY designation ASC");
        ui->comboBox_plante->setModel(modelM);
        init_models(Num_culture);

        QSqlQuery query;
        QString   nom_plante;
        QString   id_plante = QString::number(Id);

        query.exec(QString("select designation from plantes where id =" + id_plante));
        if (query.first())
        {
            nom_plante = query.value(0).toString();

            ui->comboBox_plante->setCurrentText(nom_plante);
        }
        else
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver();
            QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                     tr("Veuillez vérifier que tous les champs soient bien remplis"));
        }
    }
}

void Cultures::on_pushButton_Gantt_clicked()
{   // ouvrir la fiche des tâches du diagramme gantt
    Dialog_taches *FicheTaches = new Dialog_taches(0, this);

    connect(this, SIGNAL(sendText(const QString&)),
            FicheTaches, SLOT(getIdCulture(const QString&)));
    if (ui->lineEdit_id_cultures->text() != "0")
    {
        //  emit sendText(ui->lineEdit_id_cultures->text());
        //  FicheTaches->exec();
        creer_phase();
    }
    else
    {
        QMessageBox::warning(this, tr("Erreur "),
                             tr("Veuillez sélectionner une culture svp"));
    }
}

void Cultures::creer_phase()
{
    QSqlQuery query;
    QString   designation     = util::apos(ui->lineEditDesignation->text());
    QString   commentaires    = "";
    QString   depart          = ui->dateEdit->date().toString("dd-MM-yyyy");
    QString   fin             = ui->dateEdit->date().toString("dd-MM-yyyy");
    QString   duree           = "1";
    QString   id_culture      = ui->lineEdit_id_cultures->text();
    QString   contrainte_date = "0";
    QString   avancement      = "0";
    QString   type            = "1";
    QString   id_precedent    = "0";
    QString   id_PhaseParent;

    query.exec(QString("SELECT COUNT(*) FROM tasks"));
    if (query.first())
    {
        int NbRowTable = query.value(0).toInt();
        if (NbRowTable > 0)
        {
            query.exec(QString("SELECT id FROM tasks WHERE NOT EXISTS  (SELECT id FROM tasks WHERE id_culture = " + id_culture +
                               ")"));
            if (query.first())
            {
                int id = query.value(0).toInt();

                if (id > 0)
                {
                    query.exec(QString("SELECT MAX(id) FROM tasks "));
                    if (query.first())
                    {
                        int id_MAX = query.value(0).toInt();
                        id_PhaseParent = QString::number(id_MAX + 1);
                        qDebug() << "query 1:" << query.lastQuery();
                    }
                    else
                    {
                        qDebug() << "erreur query 1:" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                            query.driver() << query.lastQuery();
                    }

                    QString str =
                        "insert into tasks (designation,commentaires, depart, fin, duree,precedent, avancement,type,contrainte_date,phase_parent,id_culture)"
                        "values('" + designation + "','" + commentaires + "','" + depart + "','" + fin +
                        "'," + duree + "," + id_precedent + "," + avancement + "," + type + "," + contrainte_date + "," +
                        id_PhaseParent +
                        "," +
                        id_culture + ")";
                    query.exec(str);
                    if (!query.isActive())
                    {
                        qDebug() << "erreur query valider:" << query.lastError().text() << "  " <<
                            query.lastError().databaseText() <<
                            query.lastQuery().toUtf8();
                        QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                                 tr("Veuillez vérifier que tous les champs soient bien remplis"));
                    }
                    else
                    {
                        QMessageBox::information(this, tr("Enregistrement de la phase culture "),
                                                 tr("La phase de culture a été crée avec succès !"));
                    }
                }
                else
                {
                    qDebug() << "query 2:" << query.lastQuery();
                }
            }
            else
            {
                QMessageBox::warning(this, tr("Erreur "),
                                     tr("Cette phase de culture est existante !"));
                qDebug() << "erreur query 3:" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                    query.driver() << query.lastQuery();
            }
        }
        else
        {
            id_PhaseParent = QString::number(1);
            QString str =
                "insert into tasks (designation,commentaires, depart, fin, duree,precedent, avancement,type,contrainte_date,phase_parent,id_culture)"
                "values('" + designation + "','" + commentaires + "','" + depart + "','" + fin +
                "'," + duree + "," + id_precedent + "," + avancement + "," + type + "," + contrainte_date + "," + id_PhaseParent +
                "," +
                id_culture + ")";
            query.exec(str);
            if (!query.isActive())
            {
                qDebug() << "erreur query valider:" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                    query.lastQuery().toUtf8();
                QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                         tr("Veuillez vérifier que tous les champs soient bien remplis"));
            }
            else
            {
                QMessageBox::information(this, tr("Enregistrement de la phase culture "),
                                         tr("La phase de culture a été crée avec succès !"));
            }
        }
    }
}

void Cultures::on_toolButton_clicked()
{
    close();
}
