#include "dialogrequetefamille.h"
#include "ui_dialogrequetefamille.h"
#include "mainwindow.h"
#include "utilitaires/util.h"
#include <QMessageBox>
#include <QtSql>

DialogRequeteFamille::DialogRequeteFamille(const int&IdFamille, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRequeteFamille)
{
    ui->setupUi(this);

    //remplissage combobox famille
    QSqlQueryModel *modelFamille = new QSqlQueryModel;
    modelFamille->setQuery(
        "SELECT designation FROM FAMILLES ORDER BY designation ASC");
    ui->comboBoxFamille->setModel(modelFamille);
}

DialogRequeteFamille::~DialogRequeteFamille()
{
    delete ui;
}

void DialogRequeteFamille::on_comboBoxFamille_currentTextChanged(const QString &arg1)
{ // choix d'une famille et modification combobox des espèces
    QSqlQuery query;
   // QString   nom_famille = util::apos(ui->comboBoxFamille->currentText());
    QString   nom_famille = util::apos(arg1);
    QString   id_famille;

    query.exec(QString("select id from familles where designation ='" + nom_famille + "'"));
    if (query.first())
    {
        id_famille = query.value(0).toString();
        //remplissage combobox espece
        QSqlQueryModel *modelEspece = new QSqlQueryModel;
        modelEspece->setQuery(
            "SELECT designation FROM especes WHERE famille = '" + id_famille + "'    ORDER BY designation ASC");
        ui->comboBoxEspece->setModel(modelEspece);
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
        QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                 tr("Veuillez vérifier que tous les champs soient bien remplis"));
    }


}

void DialogRequeteFamille::on_comboBoxEspece_currentTextChanged(const QString &arg1)
{

    // choix d'une famille et modification combobox des espèces
        QSqlQuery query;
       // QString   nom_famille = util::apos(ui->comboBoxFamille->currentText());
        QString   nom_espece = util::apos(arg1);
        QString   id_espece;

        query.exec(QString("select id from especes where designation ='" + nom_espece + "'"));
        if (query.first())
        {
            id_espece = query.value(0).toString();
            setIdEspece(id_espece);
        }
        else
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
            QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                     tr("Veuillez vérifier que tous les champs soient bien remplis"));
        }

}

void DialogRequeteFamille::on_buttonBox_accepted()
{
    accept();
    close();
}
