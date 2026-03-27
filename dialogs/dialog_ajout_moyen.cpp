#include "dialog_ajout_moyen.h"
#include "ui_dialog_ajout_moyen.h"
#include "dialogs/dialog_moyens.h"
#include "dialog_ressources.h"
#include "utilitaires/util.h"
#include "mainwindow.h"
#include <QMessageBox>

Dialog_ajout_Moyen::Dialog_ajout_Moyen(const int&IdRessource, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_ajout_Moyen)
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
    setIdRessource(IdRessource);
    QSqlQueryModel *modelMoyens = new QSqlQueryModel;
    modelMoyens->setQuery(
        "SELECT id, designation FROM moyens ORDER BY id ASC");
    ui->tableView_moyens->setModel(modelMoyens);
    ui->tableView_moyens->setColumnWidth(0, 50);
    ui->tableView_moyens->setColumnWidth(1, 250);
}

Dialog_ajout_Moyen::~Dialog_ajout_Moyen()
{
    delete ui;
}

void Dialog_ajout_Moyen::on_buttonBox_accepted()
{
    QSqlQuery query;
    QString   id_ressource = QString::number(getIdRessource());
    QString   id_moyen     = ui->lineEdit_idMoyen->text();
    QString   designation  = util::apos(ui->lineEdit_designation_moyen->text());
    QString   str          =
        "insert into liste_moyens (designation,id_ressource, id_moyen)"
        "values('" + designation + "'," + id_ressource + "," + id_moyen + ")";

    query.exec(str);
    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
            query.driver() << query.lastQuery();
    }
    else
    {
        qDebug() << "Moyen ajouté avec succès";
    }
    accept();
    close();
}

void Dialog_ajout_Moyen::on_tableView_moyens_clicked(const QModelIndex&index)
{
    int     row   = index.row();
    QString strId =
        ui->tableView_moyens->model()->data(ui->tableView_moyens->model()->index(row, 0)).toString();
    QString strDesignation =
        ui->tableView_moyens->model()->data(ui->tableView_moyens->model()->index(row, 1)).toString();

    ui->lineEdit_idMoyen->setText(strId);
    ui->lineEdit_designation_moyen->setText(strDesignation);
}

void Dialog_ajout_Moyen::on_pushButton_addMoyen_clicked()
{   // Nouveau moyen ouverture de la fiche des moyens
    Dialog_moyens *Fiche    = new Dialog_moyens(0, this);
    int            resultat = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
        QSqlQueryModel *modelMoyens = new QSqlQueryModel;
        modelMoyens->setQuery(
            "SELECT id, designation FROM moyens ORDER BY id ASC");
        ui->tableView_moyens->setModel(modelMoyens);
    }
}

void Dialog_ajout_Moyen::on_toolButton_editMoyen_clicked()
{
    //éditer le moyen sélectionné
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
