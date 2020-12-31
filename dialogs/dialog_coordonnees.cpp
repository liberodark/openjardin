#include "dialog_coordonnees.h"
#include "ui_dialog_coordonnees.h"
#include "mainwindow.h"
#include "dialog_taches.h"
#include "utilitaires/util.h"

Dialog_coordonnees::Dialog_coordonnees(const int&IdCoord, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_coordonnees)
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
    setIdCoord(IdCoord);
    init_base();
}

Dialog_coordonnees::~Dialog_coordonnees()
{
    delete ui;
}

void Dialog_coordonnees::init_base()
{   /***********************initialisation des bases de données sqlite******************/
    QSqlQueryModel *modelCoordonnees = new QSqlQueryModel;
    QString         idCoord          = QString::number(getIdCoord());

    modelCoordonnees->setQuery(
        "SELECT societe FROM coordonnees ORDER BY id ASC");
    ui->comboBox_societe->setModel(modelCoordonnees);

    if (idCoord != "")
    {
        ui->lineEdit_idCoordonnees->setText(idCoord);
        ui->comboBox_societe->setCurrentIndex(idCoord.toInt() - 1);
    }
}

void Dialog_coordonnees::on_pushButton_Valider_clicked()
{
    QSqlQuery query;
    QString   id             = ui->lineEdit_idCoordonnees->text();
    QString   societe        = util::apos(ui->lineEdit_Societe->text());
    QString   commentaires   = util::apos(ui->textEdit_Commentaires->document()->toPlainText());
    QString   nom            = util::apos(ui->lineEdit_Nom->text());
    QString   prenom         = util::apos(ui->lineEdit_Prenom->text());
    QString   adresse        = util::apos(ui->lineEdit_Adresse->text());
    QString   code_postal    = ui->lineEdit_Code_postal->text();
    QString   ville          = util::apos(ui->lineEdit_Ville->text());
    QString   pays           = util::apos(ui->lineEdit_Pays->text());
    QString   telephone_fixe = ui->lineEdit_Fixe->text();
    QString   portable       = ui->lineEdit_portable->text();
    QString   fax            = ui->lineEdit_Fax->text();
    QString   email          = util::apos(ui->lineEdit_Email->text());

    if (ui->lineEdit_idCoordonnees->text() == "")
    {   //mode nouvel enregistrement
        QSqlQuery query;

        QString str =
            "insert into coordonnees (societe,commentaires,nom,prenom,adresse,code_postal,ville,pays,telephone_fixe,portable,fax,email)"
            "values('" + societe + "','" + commentaires + "','" + nom + "','" + prenom + "','" + adresse +
            "','" + code_postal + "','" + ville +
            "','" + pays + "','" + telephone_fixe +
            "','" + portable + "','" + fax +
            "','" + email + "')";
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
        QString strQuery = "update coordonnees set societe = '" + societe + "',commentaires='" + commentaires +
                           +"',nom='" + nom + "',prenom='" + prenom + "',adresse='" +
                           adresse + "',code_postal='" + code_postal + "', ville ='" + ville +
                           "',pays='" + pays + "',telephone_fixe='" + telephone_fixe + "', portable ='" + portable +
                           "',fax='" + fax + "',email='" + email + "' where id=" + id;
        query.exec(strQuery);
        qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.lastQuery();
        }
        else
        {
            qDebug() << "enregistrement terminé";
        }
    }
    QSqlQueryModel *modelCoordonnees = new QSqlQueryModel;

    modelCoordonnees->setQuery(
        "SELECT id, societe FROM coordonnees ORDER BY id ASC");
    ui->comboBox_societe->setModel(modelCoordonnees);
    accept();
    close();
}

void Dialog_coordonnees::on_pushButton_nouveau_clicked()
{
}

void Dialog_coordonnees::on_pushButton_Supprimer_clicked()
{
}

void Dialog_coordonnees::on_pushButton_Annuler_clicked()
{
    accept();
    close();
}

void Dialog_coordonnees::on_comboBox_societe_currentTextChanged(const QString&arg1)
{
    QSqlQuery query;
    QString   id = ui->lineEdit_idCoordonnees->text();
    QString   societe;
    QString   commentaires;
    QString   nom;
    QString   prenom;
    QString   adresse;
    QString   code_postal;
    QString   ville;
    QString   pays;
    QString   telephone_fixe;
    QString   portable;
    QString   fax;
    QString   email;

    ui->lineEdit_Societe->setText(arg1);
    query.exec(QString("select id from coordonnees where societe ='" + util::apos(arg1) + "'"));
    if (query.first())
    {
        id = query.value(0).toString();
        ui->lineEdit_idCoordonnees->setText(id);
        query.exec(QString(
                       "select commentaires,nom,prenom,adresse,code_postal,ville,pays,telephone_fixe,portable,fax,email from coordonnees where id="
                       +
                       id));
        if (query.first())
        {
            commentaires   = query.value(0).toString();
            nom            = query.value(1).toString();
            prenom         = query.value(2).toString();
            adresse        = query.value(3).toString();
            code_postal    = query.value(4).toString();
            ville          = query.value(5).toString();
            pays           = query.value(6).toString();
            telephone_fixe = query.value(7).toString();
            portable       = query.value(8).toString();
            fax            = query.value(9).toString();
            email          = query.value(10).toString();
            ui->textEdit_Commentaires->setText(commentaires);
            ui->lineEdit_Nom->setText(nom);
            ui->lineEdit_Prenom->setText(prenom);
            ui->lineEdit_Adresse->setText(adresse);
            ui->lineEdit_Code_postal->setText(code_postal);
            ui->lineEdit_Ville->setText(ville);
            ui->lineEdit_Pays->setText(pays);
            ui->lineEdit_Fixe->setText(telephone_fixe);
            ui->lineEdit_portable->setText(portable);
            ui->lineEdit_Fax->setText(fax);
            ui->lineEdit_Email->setText(email);
        }
        else
        {
            qWarning("ne peut récupérer les valeurs de la table coordonnees");
        }
    }
}
