#include "dialog_type_de_tache.h"
#include "ui_dialog_type_de_tache.h"
#include "mainwindow.h"
#include "dialog_taches.h"
#include <QDialog>
#include <QColorDialog>
#include "utilitaires/util.h"

Dialog_type_de_tache::Dialog_type_de_tache(const int&IdTypeTache, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_type_de_tache)
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
    setIdTypeDeTache(IdTypeTache);
    init_base();
}

Dialog_type_de_tache::~Dialog_type_de_tache()
{
    delete ui;
}

void Dialog_type_de_tache::init_base()
{   /***********************initialisation des bases de données sqlite******************/
    QSqlQueryModel *modelTypeTache = new QSqlQueryModel;
    QString         id_Type_Tache  = QString::number(getIdTypeDeTache());

    modelTypeTache->setQuery(
        "SELECT designation FROM type_de_tache ORDER BY id ASC");
    ui->comboBox_Type_de_tache->setModel(modelTypeTache);

    if (id_Type_Tache != "")
    {
        ui->lineEdit_idType_de_tache->setText(id_Type_Tache);
        ui->comboBox_Type_de_tache->setCurrentIndex(id_Type_Tache.toInt() - 1);
    }
}

void Dialog_type_de_tache::on_pushButton_nouveau_clicked()
{
    ui->lineEdit_idType_de_tache->setText("");
    ui->textEdit_Commentaires->clear();
    ui->lineEdit_Designation->clear();
    ui->label_couleur->clear();
}

void Dialog_type_de_tache::on_pushButton_Supprimer_clicked()
{
}

void Dialog_type_de_tache::on_pushButton_Valider_clicked()
{
    if (ui->comboBox_Type_de_tache->currentIndex() == 0)
    {
        QMessageBox::information(this, tr("Information "),
                                 tr("La fiche PHASES ne peut être modifiée !"));
    }
    else
    {
        QSqlQuery query;
        QString   id_type_de_tache = ui->lineEdit_idType_de_tache->text();
        QString   designation      = util::apos(ui->lineEdit_Designation->text());
        QString   couleur          = ui->label_couleur->text();
        QString   forme            = QString::number(ui->comboBox_forme->currentIndex() + 1);;

        if (ui->lineEdit_idType_de_tache->text() == "")
        {   //mode nouvel enregistrement
            QSqlQuery query;
            QString   str =
                "insert into type_de_tache (designation,couleur, forme)"
                "values('" + designation + "','" + couleur + "'," + forme + ")";
            query.exec(str);
            if (!query.isActive())
            {
                qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                    query.driver();
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

            QString strQuery = "update type_de_tache set designation = '" + designation + "',couleur='" + couleur +
                               "',forme= " +
                               forme +
                               " where id=" + id_type_de_tache;
            query.exec(strQuery);
            qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

            if (!query.isActive())
            {
                qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                    query.driver();
                QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                         tr("Veuillez vérifier que tous les champs soient bien remplis"));
            }
            else
            {
                qDebug() << "enregistrement terminé";
            }
        }
    }
}

void Dialog_type_de_tache::on_pushButton_Annuler_clicked()
{
    close();
}

void Dialog_type_de_tache::on_comboBox_Type_de_tache_currentTextChanged(const QString&arg1)
{
    QSqlQuery query;
    QString   id_type_de_tache;
    QString   designation;
    QString   couleur;
    QString   forme;

    ui->lineEdit_Designation->setText(arg1);
    query.exec(QString("select id from type_de_tache where designation ='" + util::apos(arg1) + "'"));
    if (query.first())
    {
        id_type_de_tache = query.value(0).toString();
        ui->lineEdit_idType_de_tache->setText(id_type_de_tache);
        query.exec(QString("select couleur from type_de_tache where id=" + id_type_de_tache));
        if (query.first())
        {
            couleur = query.value(0).toString();
            ui->label_couleur->setText(couleur);
            ui->label_couleur->setStyleSheet("QLabel { background-color : " + couleur + "}");
        }
        else
        {
            qWarning("ne peut récupérer la valeur couleur");
        }
        query.exec(QString("select forme from type_de_tache where id=" + id_type_de_tache));
        if (query.first())
        {
            forme = query.value(0).toString();
            ui->comboBox_forme->setCurrentIndex(forme.toInt() - 1);
        }
        else
        {
            qWarning("ne peut récupérer la valeur forme");
        }
    }
}

void Dialog_type_de_tache::on_toolButton_CouleurFond_clicked()
{
    QColorDialog *dialog = new QColorDialog(this);

    dialog->show();
    int resultCode = dialog->exec();
    if (resultCode == QDialog::Accepted)
    {
        QColor   color   = dialog->currentColor();
        QVariant variant = color;
        QString  rgb     = variant.toString();
        ui->label_couleur->setText(rgb);
        ui->label_couleur->setStyleSheet("QLabel { background-color : " + rgb + "}");
    }
}
