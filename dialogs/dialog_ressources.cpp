#include "dialog_ressources.h"
#include "ui_dialog_ressources.h"
#include "mainwindow.h"
#include "dialogs/dialog_coordonnees.h"
#include "dialog_taches.h"
#include "dialog_ajout_moyen.h"
#include "dialogs/dialog_moyens.h"
#include "utilitaires/util.h"

Dialog_ressources::Dialog_ressources(const int&IdRessource, const int&IdTache, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_ressources)
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
    setIdRessource(IdRessource);
    setIdTache(IdTache);
    init_base();
}

Dialog_ressources::~Dialog_ressources()
{
    delete ui;
}

void Dialog_ressources::init_base()
{   /***********************initialisation des bases de données sqlite******************/
    majModelListeMoyens(getIdRessource());

    QSqlQuery query;



    QSqlQueryModel *modelRessources = new QSqlQueryModel;
    QString         idRessource     = QString::number(getIdRessource());

    modelRessources->setQuery(
        "SELECT designation FROM ressources ORDER BY id ASC");
    ui->comboBox_Ressource->setModel(modelRessources);

    if (getIdRessource() != 0)
    {
        QString designation_ressource;
        query.exec(QString("select designation from ressources where id =" + idRessource));
        if (query.first())
        {
            designation_ressource = query.value(0).toString();
        }
        ui->lineEdit_idRessource->setText(idRessource);
        ui->comboBox_Ressource->setCurrentText(designation_ressource);
    }
    else
    {
        ui->lineEdit_idRessource->setText(idRessource);
        ui->comboBox_Ressource->setCurrentIndex(-1);
        QString designation;
        query.exec(QString("select designation from tasks where id =" + QString::number(getIdtache())));

        if (query.first())
        {
            designation = query.value(0).toString();
            ui->lineEdit_Designation_tache->setText(designation);
            ui->lineEdit_IdTache->setText(QString::number(getIdtache()));
        }
    }


    //remplissage comnbobox unités
    QSqlQueryModel *modelUnites = new QSqlQueryModel;

    modelUnites->setQuery(
        "SELECT designation FROM unites ORDER BY id ASC");
    ui->comboBox_unite->setModel(modelUnites);
}

void Dialog_ressources::majModelListeMoyens(int id_ressource)
{
    QSqlQueryModel *modelListeMoyens = new QSqlQueryModel;
    QString         id_Ressource     = QString::number(id_ressource);

    modelListeMoyens->setQuery(
        "SELECT id,id_moyen,designation,quantite_prevu,quantite_realise,unite FROM liste_moyens WHERE id_ressource= " + id_Ressource +
        " ORDER BY id ASC");
    ui->tableView_moyens_alloues->setModel(modelListeMoyens);
}

void Dialog_ressources::on_comboBox_Ressource_currentTextChanged(const QString&arg1)
{
    QSqlQuery query;
    QString   id_ressource;
    QString   commentaires;
    QString   tache;
    QString   cout_prevu;
    QString   cout_final;

    majModelListeMoyens(getIdRessource());

    ui->lineEdit_Designation->setText(arg1);
    query.exec(QString("select id from ressources where designation ='" + util::apos(arg1) + "'"));
    if (query.first())
    {
        id_ressource = query.value(0).toString();

        ui->lineEdit_idRessource->setText(id_ressource);
        majModelListeMoyens(id_ressource.toInt());

        query.exec(QString("select commentaires from ressources where id=" + id_ressource));
        if (query.first())
        {
            commentaires = query.value(0).toString();
            ui->textEdit_Commentaires->setPlainText(commentaires);
        }
        else
        {
            qWarning("ne peut récupérer la valeur commentaires");
        }

        query.exec(QString("select tache from ressources where id=" + id_ressource));
        if (query.first())
        {
            tache = query.value(0).toString();
            ui->lineEdit_IdTache->setText(tache);
            query.exec(QString("select designation from tasks where id=" + tache));
            if (query.first())
            {
                QString designation_tache = query.value(0).toString();
                if (ui->lineEdit_idRessource->text() != "0" || ui->lineEdit_idRessource->text() != "")
                {
                    ui->lineEdit_Designation_tache->setText(designation_tache);
                }
            }
        }
        else
        {
            qWarning("ne peut récupérer la valeur tache");
        }

        query.exec(QString("select cout_prevu from ressources where id=" + id_ressource));
        if (query.first())
        {
            QString cout_prevu = query.value(0).toString();
            ui->lineEdit_coutPrevu->setText(cout_prevu);
        }
        else
        {
            qWarning("ne peut récupérer la valeur cout_prevu");
        }

        query.exec(QString("select cout_actualise from ressources where id=" + id_ressource));
        if (query.first())
        {
            QString cout_actualise = query.value(0).toString();
            ui->lineEdit_coutTotal->setText(cout_actualise);
        }
        else
        {
            qWarning("ne peut récupérer la valeur cout_actualise");
        }
    }
    calculer();
}

void Dialog_ressources::on_pushButton_ajout_ressource_clicked()
{   //ajouter la ressource à la tache
    valider();
    QSqlQuery query;
    QString   id;
    QString   id_tache     = QString::number(getIdtache());
    QString   id_ressource = ui->lineEdit_idRessource->text();
    QString   designation  = util::apos(ui->lineEdit_Designation->text());

    query.exec(QString("select id from liste_ressources where id_ressource =" + id_ressource));
    if (query.first())
    {
        id = query.value(0).toString();
    }
    if (id.toInt() > 0)
    {
        QString strQuery = "update liste_ressources set designation = '" + designation + "',id_task=" + id_tache +
                           ",id_ressource=" + id_ressource +
                           " where id=" +
                           id;
        query.exec(strQuery);
        qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver() << query.lastQuery();
            QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                     tr("Veuillez vérifier que tous les champs soient bien remplis"));
        }
        else
        {
            qDebug() << "modification liste_ressource terminée";
        }

        QMessageBox::information(this, tr("Ressource déja attribuée"),
                                 tr("Modification de la ressource prise en compte"));
    }
    else
    {
        QString str =
            "insert into liste_ressources (designation,id_task, id_ressource)"
            "values('" + designation + "'," + id_tache + "," + id_ressource + ")";

        query.exec(str);
        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver() << query.lastQuery();
        }
        else
        {
            qDebug() << "Ressource ajoutée avec succès";
        }
    }
    accept();
    close();
}

void Dialog_ressources::valider()
{
    calculer();
    //enregistrer dans la base
    QSqlQuery query;
    QString   id_ressource = ui->lineEdit_idRessource->text();
    QString   designation  = util::apos(ui->lineEdit_Designation->text());
    QString   commentaires = util::apos(ui->textEdit_Commentaires->document()->toPlainText());
    QString   cout_prevu   = ui->lineEdit_coutPrevu->text();
    QString   tache;
    QString   cout_actualise = ui->lineEdit_coutTotal->text();



    query.exec(QString("select id from tasks where designation = '" + ui->lineEdit_Designation_tache->text() + "'"));

    if (query.first())
    {
        tache = query.value(0).toString();
        setIdTache(tache.toInt());
    }
    else
    {
        qWarning("ne peut récupérer la valeur id de tasks");
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
            query.driver() << query.lastQuery();
    }

    if (cout_prevu == "")
    {
        cout_prevu = "0";
    }
    if (cout_actualise == "")
    {
        cout_actualise = "0";
    }

    if (ui->lineEdit_idRessource->text() == "" || ui->lineEdit_idRessource->text() == "0")
    {      //mode nouvel enregistrement
        QSqlQuery query;

        QString str =
            "insert into ressources (designation,commentaires,tache,cout_prevu,cout_actualise)"
            "values('" + designation + "','" + commentaires + "'," + tache + "," +
            cout_prevu + "," + cout_actualise + ")";
        query.exec(str);
        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver() << query.lastQuery();
            QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                     tr("Veuillez vérifier que tous les champs soient bien remplis"));
        }
        else
        {
            qDebug() << "enregistrement terminé avec succès";
        }
    }
    else
    {
        //mode modification

        QString strQuery = "update ressources set designation = '" + designation + "',commentaires='" + commentaires +
                           "',tache=" + tache + ",cout_prevu= " + cout_prevu + ",cout_actualise= " + cout_actualise +
                           " where id=" +
                           id_ressource;
        query.exec(strQuery);
        qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver() << query.lastQuery();
            QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                     tr("Veuillez vérifier que tous les champs soient bien remplis"));
        }
        else
        {
            qDebug() << "enregistrement terminé";
        }
    }
    QSqlQueryModel *modelRessource = new QSqlQueryModel;

    modelRessource->setQuery(
        "SELECT designation, tache FROM ressources ORDER BY id ASC");
    ui->comboBox_Ressource->setModel(modelRessource);
    ui->comboBox_Ressource->setCurrentIndex(ui->comboBox_Ressource->count() - 1);
    close();
}

void Dialog_ressources::on_pushButton_nouveau_clicked()
{
    ui->lineEdit_idRessource->setText("0");
    ui->comboBox_Ressource->setCurrentIndex(-1);
    ui->textEdit_Commentaires->clear();
    ui->lineEdit_coutPrevu->setText("0");
    ui->lineEdit_coutTotal->setText("0");
    ui->lineEdit_IdTache->setText(QString::number(getIdtache()));
}

void Dialog_ressources::calculer()
{   // calculer le total actualisé
    QString   idressource = ui->lineEdit_idRessource->text();
    QSqlQuery query("SELECT quantite_prevu,quantite_realise, id_moyen FROM liste_moyens WHERE id_ressource =" + idressource);
    QString   qPrevu;
    QString   qRealise;
    QString   idMoyen;
    QString   coutUnitaire;
    double    coutPrevu        = 0;
    double    coutRealise      = 0;
    int       quantite_prevu   = 0;
    int       quantite_realise = 0;

    while (query.next())
    {
        qPrevu           = query.value(0).toString();
        qRealise         = query.value(1).toString();
        idMoyen          = query.value(2).toString();
        quantite_prevu   = qPrevu.toInt();
        quantite_realise = qRealise.toInt();


        QSqlQuery query2("select cout_unitaire from moyens where id =" + idMoyen);
        if (query2.first())
        {
            coutUnitaire = util::afficheZeros(query2.value(0).toString());
            coutPrevu    = coutPrevu + (quantite_prevu * coutUnitaire.toDouble());
            coutRealise  = coutRealise + (quantite_realise * coutUnitaire.toDouble());
        }
        if (!query2.isActive())
        {
            qDebug() << "erreur query :" << query2.lastError().text() << "  " << query2.lastError().databaseText() <<
                query2.driver() << query2.lastQuery();
        }
    }
    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
            query.driver() << query.lastQuery();
    }



    ui->lineEdit_coutPrevu->setText(util::afficheZeros(QString::number(coutPrevu, 'f', 1)));
    ui->lineEdit_coutTotal->setText(util::afficheZeros(QString::number(coutRealise, 'f', 1)));
}

void Dialog_ressources::on_pushButton_Annuler_clicked()
{
    accept();
    close();
}

void Dialog_ressources::on_toolButton_addMoyen_clicked()
{   //ajouter un moyen
    valider();
    int id_ressource = ui->lineEdit_idRessource->text().toInt();
    Dialog_ajout_Moyen *FicheAjoutMoyen = new Dialog_ajout_Moyen(id_ressource, this);
    int resultat = FicheAjoutMoyen->exec();

    if (resultat == QDialog::Accepted)
    {
        QSqlQueryModel *modelListeMoyens = new QSqlQueryModel;
        QString         id_Ressource     = QString::number(getIdRessource());
        modelListeMoyens->setQuery(
            "SELECT id_moyen,designation,quantite_prevu,quantite_realise,unite FROM liste_moyens WHERE id_ressource= " + id_Ressource +
            " ORDER BY id ASC");
        ui->tableView_moyens_alloues->setModel(modelListeMoyens);
    }


    on_comboBox_Ressource_currentTextChanged(ui->comboBox_Ressource->currentText());
}

void Dialog_ressources::on_toolButton_supprimeMoyen_clicked()
{   //supprimer un moyen sélectionné
    QSqlQuery query;
    QString   id_moyen = ui->lineEdit_idListeRessource->text();

    if (id_moyen.toInt() > 0)
    {
        QString str = "delete from liste_moyens where id=" + id_moyen;

        query.exec(str);
        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver() << query.lastQuery();
        }
        else
        {
            qDebug() << "Moyen supprimé avec succès";
        }
    }
    else
    {
        QMessageBox::information(this, tr("Pas de moyen sélectionné"),
                                 tr("Veuillez choisir un moyen avant de le supprimer svp !"));
    }
    on_comboBox_Ressource_currentTextChanged(ui->comboBox_Ressource->currentText());
}

void Dialog_ressources::on_btnMoyenEdit_clicked()
{   //éditer le moyen sélectionné
    int idMoyen = ui->lineEdit_idMoyen->text().toInt();

    if (idMoyen > 0)
    {
        Dialog_moyens *Fiche    = new Dialog_moyens(idMoyen, this);
        int            resultat = Fiche->exec();

        if (resultat == QDialog::Accepted)
        {
        }
    }
    else
    {
        QMessageBox::information(this, tr("Pas de moyen sélectionné"),
                                 tr("Veuillez choisir un moyen avant de l'éditer svp !"));
    }
}

void Dialog_ressources::on_tableView_moyens_alloues_clicked(const QModelIndex&index)
{   //"SELECT id,id_ressource,designation,quantite_prevu,quantite_realise,unite
    int     row   = index.row();
    QString strId =
        ui->tableView_moyens_alloues->model()->data(ui->tableView_moyens_alloues->model()->index(row, 0)).toString();
    QString strIdMoyen =
        ui->tableView_moyens_alloues->model()->data(ui->tableView_moyens_alloues->model()->index(row, 1)).toString();
    QString strDesignation =
        ui->tableView_moyens_alloues->model()->data(ui->tableView_moyens_alloues->model()->index(row, 2)).toString();
    QString strQp =
        ui->tableView_moyens_alloues->model()->data(ui->tableView_moyens_alloues->model()->index(row, 3)).toString();
    QString strQr =
        ui->tableView_moyens_alloues->model()->data(ui->tableView_moyens_alloues->model()->index(row, 4)).toString();
    QString strU =
        ui->tableView_moyens_alloues->model()->data(ui->tableView_moyens_alloues->model()->index(row, 5)).toString();

    //calage designation_moyens sur valeur strId
    QSqlQuery query;
    QString   resultat;

    query.exec(QString("select designation from liste_moyens where id =" + strId));
    if (query.first())
    {
        resultat = query.value(0).toString();
        ui->lineEdit_designation_moyen->setText(resultat);
    }

    ui->lineEdit_idListeRessource->setText(strId);
    ui->lineEdit_idMoyen->setText(strIdMoyen);
    ui->lineEdit_quantitePrevue->setText(strQp);
    ui->lineEdit_quantiteUtilisee->setText(strQr);
    query.exec(QString("select designation from unites where id =" + strU));
    if (query.first())
    {
        resultat = query.value(0).toString();
        ui->comboBox_unite->setCurrentText(resultat);
    }
}

void Dialog_ressources::on_toolButton_validerQuantiteMoyen_clicked()
{   //valider les quantités etc des moyens
    QSqlQuery query;
    QString   id_liste_moyen    = ui->lineEdit_idListeRessource->text();
    QString   quantite_prevu    = ui->lineEdit_quantitePrevue->text();
    QString   quantite_realise  = ui->lineEdit_quantiteUtilisee->text();
    QString   designation_unite = util::apos(ui->comboBox_unite->currentText());

    QString unite;

    query.exec(QString("select id from unites where designation ='" + designation_unite + "'"));
    if (query.first())
    {
        unite = query.value(0).toString();
    }
    QString str =
        "update liste_moyens set quantite_prevu = '" + quantite_prevu + "',quantite_realise = '" + quantite_realise +
        "',unite = " + unite + " where id =" + id_liste_moyen;

    query.exec(str);
    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
            query.driver() << query.lastQuery();
        if (id_liste_moyen == "")
        {
            QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                     tr("Veuillez choisir un moyen avant de valider svp !"));
        }
    }
    else
    {
        qDebug() << "Moyen modifié avec succès" << query.lastQuery();
    }
    on_comboBox_Ressource_currentTextChanged(ui->comboBox_Ressource->currentText());
    calculer();
}

void Dialog_ressources::on_lineEdit_idRessource_textChanged(const QString&arg1)
{
    if (arg1 == "0")
    {
        // ui->toolButton_addMoyen->setEnabled(false);
        ui->toolButton_supprimeMoyen->setEnabled(false);
        ui->toolButton_validerQuantiteMoyen->setEnabled(false);
        ui->btnMoyenEdit->setEnabled(false);
    }
    else
    {
        //  ui->toolButton_addMoyen->setEnabled(true);
        ui->toolButton_supprimeMoyen->setEnabled(true);
        ui->toolButton_validerQuantiteMoyen->setEnabled(true);
        ui->btnMoyenEdit->setEnabled(true);
    }
}

void Dialog_ressources::on_pushButton_Supprimer_clicked()
{
    // supprimer la ressource

    QSqlQuery query;
    QString   id_moyen = ui->lineEdit_idRessource->text();

    if (id_moyen.toInt() > 0)
    {
        int ret = QMessageBox::warning(this, tr("Suppression"),
                                       tr("Cette fiche peut être supprimer.\n"
                                          "Confirmer la suppression de la fiche?"),
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Cancel);

        switch (ret)
        {
        case QMessageBox::Ok:
        {
            QString str = "delete from ressources where id=" + id_moyen;

            query.exec(str);
            if (!query.isActive())
            {
                qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                    query.driver() << query.lastQuery();
            }
            else
            {
                qDebug() << "Moyen supprimé avec succès";
            }
            qDebug() << " OK";
            break;
        }

        case QMessageBox::Cancel:
        {
            qDebug() << " Cancel";
            break;
        }

        default:
            break;
        }
    }
    else
    {
        QMessageBox::information(this, tr("Pas de ressource sélectionnée"),
                                 tr("Veuillez choisir une ressource avant de le supprimer svp !"));
    }
    QSqlQueryModel *modelRessources = new QSqlQueryModel;

    modelRessources->setQuery(
        "SELECT designation FROM ressources ORDER BY id ASC");
    ui->comboBox_Ressource->setModel(modelRessources);
}
