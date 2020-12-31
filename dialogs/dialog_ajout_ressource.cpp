#include "dialog_ajout_ressource.h"
#include "ui_dialog_ajout_ressource.h"
#include "dialog_ressources.h"
#include "mainwindow.h"
#include "dialog_taches.h"
#include "utilitaires/util.h"

Dialog_ajout_ressource::Dialog_ajout_ressource(const int&IdTache, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_ajout_ressource)
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
    setIdTache(IdTache);
    QSqlQueryModel *modelRessources = new QSqlQueryModel;
    modelRessources->setQuery(
        "SELECT id, designation FROM ressources ORDER BY id ASC");
    ui->tableView_ressources->setModel(modelRessources);
}

Dialog_ajout_ressource::~Dialog_ajout_ressource()
{
    delete ui;
}

QString Dialog_ajout_ressource::apos(QString texte)
{
    texte.replace(QString("'"), QString("''")); //remplacement des apostrophes
    QString texteModif = texte.toUtf8();        //conversion
    return texteModif;
}

void Dialog_ajout_ressource::on_buttonBox_accepted()
{
    QSqlQuery query;
    QString   id_tache     = QString::number(getIdTache());
    QString   id_ressource = ui->lineEdit_Id_Ressource->text();
    QString   designation  = apos(ui->lineEdit_designation_ressource->text());
    QString   str          =
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
    accept();
    close();
}

void Dialog_ajout_ressource::on_tableView_ressources_clicked(const QModelIndex&index)
{
    int     row   = index.row();
    QString strId =
        ui->tableView_ressources->model()->data(ui->tableView_ressources->model()->index(row, 0)).toString();
    QString strDesignation =
        ui->tableView_ressources->model()->data(ui->tableView_ressources->model()->index(row, 1)).toString();

    ui->lineEdit_Id_Ressource->setText(strId);
    ui->lineEdit_designation_ressource->setText(strDesignation);
}
