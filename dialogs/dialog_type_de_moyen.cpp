#include "dialog_type_de_moyen.h"
#include "ui_dialog_type_de_moyen.h"
#include "mainwindow.h"
#include <QDialog>
#include <QMessageBox>
#include "utilitaires/util.h"

Dialog_type_de_moyen::Dialog_type_de_moyen(const int&IdTypeMoyen, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_type_de_moyen)
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
    setIdtypeMoyen(IdTypeMoyen);
    init_base();
}

Dialog_type_de_moyen::~Dialog_type_de_moyen()
{
    delete ui;
}

void Dialog_type_de_moyen::init_base()
{   /***********************initialisation des bases de données sqlite******************/
    QSqlQueryModel *modeltype_de_moyen = new QSqlQueryModel;
    QString         IdtypeMoyen        = QString::number(getIdtypeMoyen());

    modeltype_de_moyen->setQuery(
        "SELECT designation FROM type_de_moyen ORDER BY id ASC");
    ui->comboBox_Type_de_moyen->setModel(modeltype_de_moyen);
    QString   designation;
    QString   commentaires;
    QSqlQuery query;
    query.exec(QString("select designation, commentaires from type_de_moyen where id =" + IdtypeMoyen));

    if (query.first())
    {
        designation  = query.value(0).toString();
        commentaires = query.value(1).toString();
        ui->lineEdit_Designation->setText(designation);
        ui->lineEdit_idType_de_moyen->setText(IdtypeMoyen);
        ui->textEdit_Commentaires->setText(commentaires);
    }

    if (IdtypeMoyen == "0")
    {
        ui->comboBox_Type_de_moyen->setCurrentIndex(-1);
    }
}

void Dialog_type_de_moyen::on_pushButton_nouveau_clicked()
{
    ui->lineEdit_idType_de_moyen->setText("");
    ui->textEdit_Commentaires->clear();
    ui->lineEdit_Designation->setText("");
}

void Dialog_type_de_moyen::on_pushButton_Supprimer_clicked()
{
}

void Dialog_type_de_moyen::on_pushButton_Valider_clicked()
{
    QSqlQuery query;
    QString   id_type_de_moyen = ui->lineEdit_idType_de_moyen->text();
    QString   designation      = util::apos(ui->lineEdit_Designation->text());
    QString   commentaires     = util::apos(ui->textEdit_Commentaires->document()->toPlainText());

    if (ui->lineEdit_idType_de_moyen->text() == "")
    {   //mode nouvel enregistrement
        QSqlQuery query;
        QString   str =
            "insert into type_de_moyen (designation,commentaires)"
            "values('" + designation + "','" + commentaires + "')";
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
            qDebug() << "Nouvel enregistrement terminé avec succès";
        }
    }
    else
    {
        //mode modification²

        QString strQuery = "update type_de_moyen set designation = '" + designation + "',commentaires='" + commentaires +
                           "' where id=" + id_type_de_moyen;
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
            qDebug() << "Modification de l'enregistrement terminée";
        }
    }
}

void Dialog_type_de_moyen::on_pushButton_Annuler_clicked()
{
    close();
}
