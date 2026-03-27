#include "dialog_moyens.h"
#include "ui_dialog_moyens.h"
#include "dialog_taches.h"
#include "dialog_ressources.h"
#include "mainwindow.h"
#include "dialogs/dialog_coordonnees.h"
#include "dialogs/dialog_type_de_moyen.h"
#include "utilitaires/util.h"

Dialog_moyens::Dialog_moyens(const int&IdMoyen, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_moyens)
{
    // translator
    QTranslator translator;
    QString     fichier = ":/translations/open-jardin_" + util::getLocale();
    QSettings settings;
    QFile        iniFile(settings.fileName());
    if (iniFile.exists())
    {
        QSettings settings(iniFile.fileName(), QSettings::IniFormat);
        QString langue = settings.value("langue").toString();
        if(langue == "english")
            {
            // forcer la langue anglaise
                 fichier = ":/translations/open-jardin_en.ts";

             }


     }
    if (!translator.load(fichier)) {
        qWarning() << "Impossible de charger la traduction:" << fichier;
    }
    qApp->installTranslator(&translator);

    ui->setupUi(this);
    setIdMoyen(IdMoyen);
    init_base();
    if (IdMoyen == 0)
    {
        vider_fiche();
    }
}

Dialog_moyens::~Dialog_moyens()
{
    delete ui;
}

void Dialog_moyens::init_base()
{   /***********************initialisation des bases de données sqlite******************/
    QSqlQueryModel *modelMoyens = new QSqlQueryModel;

    modelMoyens->setQuery(
        "SELECT designation FROM moyens ORDER BY id ASC");
    ui->comboBox_Moyens->setModel(modelMoyens);

    ui->lineEdit_idMoyens->setText(QString::number(getIdMoyen()));


    //remplissage comnbobox unités
    QSqlQueryModel *modelUnites = new QSqlQueryModel;
    modelUnites->setQuery(
        "SELECT designation FROM unites ORDER BY id ASC");
    ui->comboBox_unite->setModel(modelUnites);

    //remplissage comnbobox type de moyen
    QSqlQueryModel *modeltype_de_moyen = new QSqlQueryModel;
    modeltype_de_moyen->setQuery(
        "SELECT designation FROM type_de_moyen ORDER BY id ASC");
    ui->comboBox_Type_moyen->setModel(modeltype_de_moyen);

    //remplissage comnbobox coordonnees
    QSqlQueryModel *modelCoordonnees = new QSqlQueryModel;
    modelCoordonnees->setQuery(
        "SELECT societe FROM coordonnees ORDER BY societe ASC");
    ui->comboBox_coordonnees->setModel(modelCoordonnees);

    QSqlQuery query;
    query.exec(QString("select designation from moyens where id =" + ui->lineEdit_idMoyens->text()));

    if (query.first())
    {
        QString designation_moyen = query.value(0).toString();
        ui->comboBox_Moyens->setCurrentText(designation_moyen);
        on_comboBox_Moyens_currentTextChanged(designation_moyen);
    }
}

void Dialog_moyens::on_pushButton_nouveau_clicked()
{
    vider_fiche();
}

void Dialog_moyens::vider_fiche()
{
    //vider les champs pour la création d'une nouvelle tâche
    ui->lineEdit_idMoyens->setText("");
    ui->lineEdit_Designation->clear();
    ui->textEdit_Commentaires->clear();
    ui->comboBox_Moyens->setCurrentIndex(-1);
    ui->comboBox_Type_moyen->setCurrentIndex(-1);
    ui->comboBox_coordonnees->setCurrentIndex(-1);
    ui->lineEdit_coutUnitaire->clear();
}

void Dialog_moyens::on_pushButton_Supprimer_clicked()
{
    // supprimer le moyen

    QSqlQuery query;
    QString   id_moyen = ui->lineEdit_idMoyens->text();

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
            QString str = "delete from moyens where id=" + id_moyen;

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
        QMessageBox::information(this, tr("Pas de moyen sélectionné"),
                                 tr("Veuillez choisir un moyen avant de le supprimer svp !"));
    }
    QSqlQueryModel *modelMoyens = new QSqlQueryModel;

    modelMoyens->setQuery(
        "SELECT designation FROM moyens ORDER BY id ASC");
    ui->comboBox_Moyens->setModel(modelMoyens);
}

void Dialog_moyens::on_pushButton_Valider_clicked()
{   //enregistrer dans la base
    QSqlQuery query;
    QString   IdMoyen       = ui->lineEdit_idMoyens->text();
    QString   designation   = util::apos(ui->lineEdit_Designation->text());
    QString   commentaires  = util::apos(ui->textEdit_Commentaires->document()->toPlainText());
    QString   cout_unitaire = ui->lineEdit_coutUnitaire->text();
    QString   coordonnees   = ui->lineEdit_coordonnees_moyen->text();
    QString   type          = ui->lineEdit_Type->text();
    QString   unite         = ui->lineEdit_unite->text();

    if (ui->lineEdit_idMoyens->text() == "")
    {   //mode nouvel enregistrement
        QSqlQuery query;

        QString str =
            "insert into moyens (designation,commentaires,cout_unitaire,coordonnees,type,unite)"
            "values('" + designation + "','" + commentaires + "','" + cout_unitaire + "'," + coordonnees +
            "," + type + "," + unite + ")";
        query.exec(str);
        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.lastQuery();
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

        QString strQuery = "update moyens set designation = '" + designation + "',commentaires='" + commentaires +
                           +"',cout_unitaire='" + cout_unitaire + "',coordonnees=" +
                           coordonnees + ",type=" + type + ", unite =" + unite + " where id=" +
                           IdMoyen;
        query.exec(strQuery);
        qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.lastQuery();
            QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                     tr("Veuillez vérifier que tous les champs soient bien remplis"));
        }
        else
        {
            qDebug() << "enregistrement terminé";
        }
    }
    QSqlQueryModel *modelMoyen = new QSqlQueryModel;

    modelMoyen->setQuery(
        "SELECT id, designation FROM moyens ORDER BY id ASC");
    accept();
    close();
}

void Dialog_moyens::on_pushButton_Annuler_clicked()
{
    accept();
    close();
}

void Dialog_moyens::on_comboBox_Moyens_currentTextChanged(const QString&arg1)
{
    QSqlQuery query;
    QString   id_moyen;
    QString   commentaires;
    QString   cout_unitaire;
    QString   coordonnees;
    QString   type;
    QString   unite;

    ui->lineEdit_Designation->setText(arg1);
    query.exec(QString("select id from moyens where designation ='" + util::apos(arg1) + "'"));

    if (query.first())
    {
        id_moyen = query.value(0).toString();
        ui->lineEdit_idMoyens->setText(id_moyen);
        query.exec(QString("select commentaires, cout_unitaire,coordonnees, type, unite from moyens where id=" +
                           id_moyen));
        if (query.first())
        {
            commentaires  = query.value(0).toString();
            cout_unitaire = query.value(1).toString();
            coordonnees   = query.value(2).toString();
            type          = query.value(3).toString();
            unite         = query.value(4).toString();

            ui->textEdit_Commentaires->setPlainText(commentaires);
            ui->lineEdit_coutUnitaire->setText(cout_unitaire);
            ui->lineEdit_coordonnees_moyen->setText(coordonnees);
            ui->lineEdit_Type->setText(type);
            ui->lineEdit_unite->setText(unite);
        }
        else
        {
            qWarning("ne peut récupérer la valeur commentaires");
        }
        // synchronisation des combobox
        query.exec(QString("select designation from type_de_moyen where id =" + type));

        if (query.first())
        {
            QString designation_type = query.value(0).toString();
            ui->comboBox_Type_moyen->setCurrentText(designation_type);
        }

        query.exec(QString("select societe from coordonnees where id =" + coordonnees));

        if (query.first())
        {
            QString societe = query.value(0).toString();
            ui->comboBox_coordonnees->setCurrentText(societe);
        }

        query.exec(QString("select designation from unites where id =" + unite));

        if (query.first())
        {
            QString designation_unite = query.value(0).toString();
            ui->comboBox_unite->setCurrentText(designation_unite);
        }
    }
}

void Dialog_moyens::on_pushButton_Edit_coord_moyen_clicked()
{
    int id_Coord = ui->lineEdit_coordonnees_moyen->text().toInt();

    Dialog_coordonnees *Fiche = new Dialog_coordonnees(id_Coord, this);

    int resultat = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void Dialog_moyens::on_pushButton_Edit_Type_clicked()
{
    int id_Typedemoyen = ui->lineEdit_Type->text().toInt();

    Dialog_type_de_moyen *Fiche = new Dialog_type_de_moyen(id_Typedemoyen, this);

    int resultat = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void Dialog_moyens::on_pushButton_Add_coord_moyen_clicked()
{
    Dialog_coordonnees *Fiche = new Dialog_coordonnees(0, this);

    int resultat = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
        QSqlQueryModel *modelCoordonnees = new QSqlQueryModel;
        modelCoordonnees->setQuery(
            "SELECT societe FROM coordonnees ORDER BY societe ASC");
        ui->comboBox_coordonnees->setModel(modelCoordonnees);
    }
}

void Dialog_moyens::on_comboBox_coordonnees_currentTextChanged(const QString&arg1)
{
    QSqlQuery query;

    query.exec(QString("select id from coordonnees where societe ='" + util::apos(arg1) + "'"));

    if (query.first())
    {
        QString id = query.value(0).toString();
        ui->lineEdit_coordonnees_moyen->setText(id);
    }
}

void Dialog_moyens::on_comboBox_Type_moyen_currentTextChanged(const QString&arg1)
{
    QSqlQuery query;

    query.exec(QString("select id from type_de_moyen where designation ='" + util::apos(arg1) + "'"));

    if (query.first())
    {
        QString id = query.value(0).toString();
        ui->lineEdit_Type->setText(id);
    }
}

void Dialog_moyens::on_comboBox_unite_currentTextChanged(const QString&arg1)
{
    QSqlQuery query;

    query.exec(QString("select id from unites where designation ='" + util::apos(arg1) + "'"));

    if (query.first())
    {
        QString id = query.value(0).toString();
        ui->lineEdit_unite->setText(id);
    }
}

void Dialog_moyens::on_toolButton_clicked()
{   // trier le combobox par ordre alphabétique
    QString         Active_moyen = ui->lineEdit_idMoyens->text();
    QSqlQueryModel *modelMoyens  = new QSqlQueryModel;

    modelMoyens->setQuery(
        "SELECT designation FROM moyens ORDER BY designation COLLATE NOCASE ASC");
    ui->comboBox_Moyens->setModel(modelMoyens);

    QSqlQuery query;
    query.exec(QString("select designation from moyens where id =" + Active_moyen));

    if (query.first())
    {
        QString designation_moyen = query.value(0).toString();
        ui->comboBox_Moyens->setCurrentText(designation_moyen);
        on_comboBox_Moyens_currentTextChanged(designation_moyen);
    }
}
