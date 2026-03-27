#include "fiche_plantes.h"
#include "ui_fiche_plantes.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cultures.h"
#include <QMainWindow>
#include <QDialog>
#include <QColorDialog>
#include <QLabel>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>
#include "utilitaires/util.h"

Fiche_plantes::Fiche_plantes(const int&IdPlante, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Fiche_plantes)
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
    // ui->pushButton_enregistrer_plantes->setStyleSheet("QPushButton {Qt::AlignVCenter}");
    ui->lineEdit_Id_plantes->setText(QString::number(IdPlante));
    init_base();
    mise_a_jourFiche(IdPlante);
    ui->lineEdit_valeur_rotation_familles->hide();
    ui->lineEdit_valeur_rotation_familles_2->hide();
    if (IdPlante == 0)
    {
        on_pushButton_Nouveau_plantes_clicked();
    }
}

Fiche_plantes::~Fiche_plantes()
{
    delete ui;
}

void Fiche_plantes::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    setIdPlante(ui->lineEdit_Id_plantes->text().toInt());
    close();
    accept();
}

void Fiche_plantes::init_base()
{   /***********************initialisation des bases de données sqlite******************/
    //ESPECES
    QSqlQueryModel *model1 = new QSqlQueryModel;

    model1->setQuery(
        "SELECT id, designation, famille, positif, negatif, commentaires, compost, rotation_ans,rotation_familles FROM especes ORDER BY designation ASC");
    model1->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model1->setHeaderData(0, Qt::Vertical, QObject::tr("designation"));
    model1->setHeaderData(1, Qt::Vertical, QObject::tr("famille"));
    ui->tableView_especes->setModel(model1);
    ui->tableView_especes->setColumnWidth(0, 30);
    ui->tableView_especes->setColumnWidth(1, 350);
    for (int i = 2; i < 9; i++)
    {
        ui->tableView_especes->setColumnHidden(i, true);
    }
    ui->tableView_especes->setColumnHidden(0, true);
    //FAMILLES
    QSqlQueryModel *model2 = new QSqlQueryModel;
    model2->setQuery("SELECT id, designation,couleur FROM FAMILLES ORDER BY designation ASC");
    model2->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model2->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model2->setHeaderData(2, Qt::Vertical, QObject::tr("couleur"));
    ui->tableView_familles->setModel(model2);
    ui->tableView_familles->setColumnWidth(0, 30);
    ui->tableView_familles->setColumnWidth(1, 300);
    ui->tableView_familles->setColumnWidth(2, 50);
    for (int i = 2; i < 9; i++)
        {
            ui->tableView_familles->setColumnHidden(i, true);
        }
        ui->tableView_familles->setColumnHidden(0, true);
    //PLANTES
    QSqlQueryModel *model3 = new QSqlQueryModel;
    model3->setQuery(
         "SELECT id, designation,type_lune,nom_latin,espece,commentaires,semis_printemps,semis_ete,semis_automne FROM plantes ORDER BY designation ASC");
    model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("espece"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("commentaires"));

    ui->tableView_plantes->setModel(model3);
    ui->tableView_plantes->setColumnWidth(0, 30);
    ui->tableView_plantes->setColumnWidth(1, 300);
   //masquer certaines colonnes
    for (int i = 2; i < 9; i++)
    {
        ui->tableView_plantes->setColumnHidden(i, true);
    }
    ui->tableView_plantes->setColumnHidden(0, true);

    //remplissage combobox familles
    QSqlQueryModel *modelF = new QSqlQueryModel;
    modelF->setQuery("SELECT designation FROM familles ORDER BY designation ASC ");
    ui->comboBox_familles_de_plantes->setModel(modelF);
    ui->comboBox_familles_de_especes->setModel(modelF);
    //remplissage combobox especes
    QSqlQueryModel *modelE = new QSqlQueryModel;
    modelE->setQuery("SELECT designation FROM especes ORDER BY designation ASC");
    ui->comboBox_especes_de_plantes->setModel(modelE);

    /**************initialisation des tables des semis récoltes********************/

    for (int x = 0; x < ui->tableWidget_culture_printemps->rowCount(); x++)
    {
        for (int y = 0; y < ui->tableWidget_culture_printemps->columnCount(); y++)
        {
            ui->tableWidget_culture_printemps->setItem(x, y, new QTableWidgetItem);
            ui->tableWidget_culture_printemps->item(x, y)->setBackground(Qt::white);
        }
    }

    for (int x = 0; x < ui->tableWidget_culture_automne->rowCount(); x++)
    {
        for (int y = 0; y < ui->tableWidget_culture_automne->columnCount(); y++)
        {
            ui->tableWidget_culture_automne->setItem(x, y, new QTableWidgetItem);
            ui->tableWidget_culture_automne->item(x, y)->setBackground(Qt::white);
        }
    }
    for (int x = 0; x < ui->tableWidget_rotation->rowCount(); x++)
    {
        for (int y = 0; y < ui->tableWidget_rotation->columnCount(); y++)
        {
            ui->tableWidget_rotation->setItem(x, y, new QTableWidgetItem);
            ui->tableWidget_rotation->item(x, y)->setBackground(Qt::white);
        }
    }
    for (int x = 0; x < ui->tableWidget_rotation_2->rowCount(); x++)
    {
        for (int y = 0; y < ui->tableWidget_rotation_2->columnCount(); y++)
        {
            ui->tableWidget_rotation_2->setItem(x, y, new QTableWidgetItem);
            ui->tableWidget_rotation_2->item(x, y)->setBackground(Qt::white);
        }
    }
}

void Fiche_plantes::mise_a_jourFiche(int row)
{
    QString id_plante = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row, 0)).toString();
    QString str       = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row, 1)).toString();
    QString str1      = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row, 3)).toString();
    QString str2      = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row, 4)).toString();
    QString str3      = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row, 5)).toString();
    QString str4      = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row, 6)).toString();
    QString str5      = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row, 7)).toString();
    QString str6      = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row, 8)).toString();
    int     str7      = ui->tableView_plantes->model()->data(ui->tableView_plantes->model()->index(row, 2)).toString().toInt();

    m_printemps = str4;
    m_ete       = str5;
    m_automne   = str6;
    //   ui->lineEdit_valeur_printemps->setText(str4);
    //   ui->lineEdit_valeur_ete->setText(str5);
    //   ui->lineEdit_valeur_automne->setText(str6);
    ui->lineEdit_nom_latin->setText(str1);
    ui->lineEdit_designation_plantes->setText(str);
    ui->comboBox_lunaire_de_plantes->setCurrentIndex(str7 - 1);
    //selection espece dans combobox
    QSqlQuery query;
    QString   resultat;
    query.exec(QString("select designation from especes where id =" + str2));
    if (query.first())
    {
        resultat = query.value(0).toString();
        // qDebug() << resultat;
        ui->comboBox_especes_de_plantes->setCurrentIndex(ui->comboBox_especes_de_plantes->findText(resultat));
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    ui->textEdit_commentaires_plantes->setText(str3);
    ui->lineEdit_Id_plantes->setText(id_plante);
    mise_a_jour_tables_semis(m_printemps, m_ete, m_automne);
}

void Fiche_plantes::on_tableView_plantes_clicked(const QModelIndex&index)
{
    int row = index.row();

    mise_a_jourFiche(row);
}

void Fiche_plantes::on_comboBox_especes_de_plantes_currentTextChanged(const QString&arg1)
{
    QSqlQuery query;
    QString   resultat;

    query.exec(QString("select id from especes where designation ='" + util::apos(arg1) + "'"));
    if (query.first())
    {
        resultat = query.value(0).toString();
        // qDebug() << resultat <<" nom "<<arg1;

        for (int index = 0; index < ui->tableView_especes->model()->rowCount(); index++)
        {
            QString id = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(index, 0)).toString();
            if (id == resultat)
            {
                QString str3 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(index, 3)).toString();
                QString str4 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(index, 4)).toString();
                QString str8 = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(index, 8)).toString();
                ui->textEdit_plante_positif->setText(str3);
                ui->textEdit_plante_negatif->setText(str4);

                //mise à jour du tableau des rotations
                if (str8 == "")
                {
                    ui->lineEdit_valeur_rotation_familles_2->setText("0000000000000000000000");
                }
                else
                {
                    ui->lineEdit_valeur_rotation_familles_2->setText(str8);//rotation familles précédentes
                }
            }
        }
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }

    query.exec(QString("select famille from especes where designation ='" + util::apos(arg1) + "'"));
    if (query.first())
    {
        resultat = query.value(0).toString();


        for (int index = 0; index < ui->tableView_familles->model()->rowCount(); index++)
        {
            QString id = ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(index, 0)).toString();
            QString designation_famille =
                ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(index, 1)).toString();
            if (id == resultat)
            {
                ui->comboBox_familles_de_plantes->setCurrentIndex(ui->comboBox_familles_de_plantes->findText(designation_famille));
            }
        }
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
}

void Fiche_plantes::on_tableView_familles_clicked(const QModelIndex&index)
{
    int     row        = index.row();
    QString id_famille = ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(row, 0)).toString();
    QString str        = ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(row, 1)).toString();
    QString couleur    = ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(row, 2)).toString();

    ui->lineEdit_Designation_famille->setText(str);
    ui->lineEdit_Id_familles->setText(id_famille);
    ui->label_couleur->setText(couleur);
    ui->label_couleur->setStyleSheet("QLabel { background-color : " + couleur + "}");
}

void Fiche_plantes::on_pushButton_Modifier_plantes_clicked()
{
    // enregister la fiche modifiée
    QSqlQuery query;
    QString   nom_espece = util::apos(ui->comboBox_especes_de_plantes->currentText());
    QString   id_espece;

    query.exec(QString("select id from especes where designation ='" + nom_espece + "'"));
    if (query.first())
    {
        id_espece = query.value(0).toString();
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }


    QString str1 = util::apos(ui->lineEdit_designation_plantes->text());
    QString str2 = id_espece;
    QString str3 = util::apos(ui->textEdit_commentaires_plantes->document()->toPlainText());
    QString str4 = m_printemps;
    QString str5 = m_ete;
    QString str6 = m_automne;
    QString str7 = util::apos(ui->lineEdit_nom_latin->text());
    QString str8 = QString::number(ui->comboBox_lunaire_de_plantes->currentIndex() + 1);

    QString str = "update plantes set designation = '" + str1 + "',type_lune=" + str8 + ",nom_latin='" + str7 + "',espece=" +
                  str2 + ",commentaires= '" + str3 + "',semis_printemps= '" + str4 + "',semis_ete= '" + str5 +
                  "',semis_automne= '" + str6 +
                  "' where id=" + ui->lineEdit_Id_plantes->text();

    query.exec(str);
    qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    else
    {
        qDebug() << "enregistrement terminé";
    }
    QSqlQueryModel *model3 = new QSqlQueryModel;
    model3->setQuery(
        "SELECT id, designation,type_lune, nom_latin,espece,commentaires,semis_printemps,semis_ete,semis_automne FROM plantes ORDER BY designation ASC");
    model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("espece"));
    ui->tableView_plantes->setModel(model3);
    on_pushButton_Nouveau_plantes_clicked();
}

void Fiche_plantes::on_pushButton_enregistrerVariete_clicked()
{
    //enregistrer la nouvelle fiche plante
    QSqlQuery query;
    QString   nom_espece = util::apos(ui->comboBox_especes_de_plantes->currentText());
    QString   id_espece;

    query.exec(QString("select id from especes where designation ='" + nom_espece + "'"));
    if (query.first())
    {
        id_espece = query.value(0).toString();
        // qDebug() << nom_espece <<" id "<<id_espece;
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }


    QString str1 = util::apos(ui->lineEdit_designation_plantes->text());
    QString str2 = id_espece;
    QString str3 = util::apos(ui->textEdit_commentaires_plantes->document()->toPlainText());
    QString str4 = m_printemps;
    QString str5 = m_ete;
    QString str6 = m_automne;
    QString str7 = util::apos(ui->lineEdit_nom_latin->text());
    QString str8 = QString::number(ui->comboBox_lunaire_de_plantes->currentIndex() + 1);

    QString str =
        "insert into plantes (designation,type_lune,nom_latin,espece,commentaires,semis_printemps,semis_ete,semis_automne) values('"
        +
        str1 + "'," + str8 + ",'" + str7 + "'," + str2 + ",'" + str3 + "','" + str4 + "','" + str5 + "','" + str6 + "')";
    query.exec(str);
    qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    else
    {
        qDebug() << "enregistrement terminé";
    }
    QSqlQueryModel *model3 = new QSqlQueryModel;
    model3->setQuery(
        "SELECT id, designation,type_lune,nom_latin, espece,commentaires,semis_printemps,semis_ete,semis_automne FROM plantes ORDER BY designation ASC");
    model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("espece"));
    ui->tableView_plantes->setModel(model3);
    on_pushButton_Nouveau_plantes_clicked();
}

void Fiche_plantes::on_pushButton_supprimer_plantes_clicked()
{
    QMessageBox msgBox;

    msgBox.setInformativeText("Voulez-vous supprimer cette plante ?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Ok:
        QString   strId = ui->lineEdit_Id_plantes->text();
        QString   str   = "delete from plantes where id=" + strId;
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

        QSqlQueryModel *model3 = new QSqlQueryModel;
        model3->setQuery(
            "SELECT id, designation,type_lune, nom_latin,espece,commentaires,semis_printemps,semis_ete,semis_automne FROM plantes ORDER BY designation ASC");
        model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        model3->setHeaderData(1, Qt::Vertical, QObject::tr("espece"));
        ui->tableView_plantes->setModel(model3);
        on_pushButton_Nouveau_plantes_clicked();


        break;
    }
}

void Fiche_plantes::on_pushButton_Nouveau_plantes_clicked()
{
    //nouvelle fiche plantes vide
    ui->lineEdit_designation_plantes->setText("");
    ui->comboBox_especes_de_plantes->setCurrentIndex(0);
    ui->textEdit_commentaires_plantes->setText("");
    ui->lineEdit_Id_plantes->setText("");
    ui->lineEdit_nom_latin->setText("");
    ui->textEdit_plante_positif->setText("");
    ui->textEdit_plante_negatif->setText("");
}

void Fiche_plantes::on_tableWidget_culture_printemps_cellClicked(int row, int column)
{
    QColor case_color = ui->tableWidget_culture_printemps->item(row, column)->background().color();

    if (row == 0)
    {
        if (case_color == Qt::green)
        {
            ui->tableWidget_culture_printemps->item(row, column)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_culture_printemps->item(row, column)->setBackground(Qt::green);
        }
    }
    else
    {
        if (case_color == Qt::gray)
        {
            ui->tableWidget_culture_printemps->item(row, column)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_culture_printemps->item(row, column)->setBackground(Qt::gray);
        }
    }
    test_tables_semis();
}


void Fiche_plantes::on_tableWidget_culture_automne_cellClicked(int row, int column)
{
    QColor case_color = ui->tableWidget_culture_automne->item(row, column)->background().color();

    if (row == 0)
    {
        if (case_color == Qt::green)
        {
            ui->tableWidget_culture_automne->item(row, column)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_culture_automne->item(row, column)->setBackground(Qt::green);
        }
    }
    else
    {
        if (case_color == Qt::gray)
        {
            ui->tableWidget_culture_automne->item(row, column)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_culture_automne->item(row, column)->setBackground(Qt::gray);
        }
    }
    test_tables_semis();
}

void Fiche_plantes::test_tables_semis()
{
    QString printemps_semis   = "";
    QString printemps_recolte = "";

    for (int y = 0; y < ui->tableWidget_culture_printemps->columnCount(); y++)
    {
        if (ui->tableWidget_culture_printemps->item(0, y)->background().color() == Qt::green)
        {
            printemps_semis = printemps_semis + "1";
        }
        else
        {
            printemps_semis = printemps_semis + "0";
        }
    }

    for (int y = 0; y < ui->tableWidget_culture_printemps->columnCount(); y++)
    {
        if (ui->tableWidget_culture_printemps->item(1, y)->background().color() == Qt::gray)
        {
            printemps_semis = printemps_semis + "1";
        }
        else
        {
            printemps_semis = printemps_semis + "0";
        }
    }





    QString automne_semis   = "";
    QString automne_recolte = "";
    for (int y = 0; y < ui->tableWidget_culture_automne->columnCount(); y++)
    {
        if (ui->tableWidget_culture_automne->item(0, y)->background().color() == Qt::green)
        {
            automne_semis = automne_semis + "1";
        }
        else
        {
            automne_semis = automne_semis + "0";
        }
    }

    for (int y = 0; y < ui->tableWidget_culture_automne->columnCount(); y++)
    {
        if (ui->tableWidget_culture_automne->item(1, y)->background().color() == Qt::gray)
        {
            automne_semis = automne_semis + "1";
        }
        else
        {
            automne_semis = automne_semis + "0";
        }
    }

    m_printemps = printemps_semis;
    m_automne   = automne_semis;
}

void Fiche_plantes::mise_a_jour_tables_semis(const QString&printemps, const QString&ete, const QString&automne)
{
    int pos = 0;

    for (int y = 0; y < ui->tableWidget_culture_printemps->columnCount(); y++)
    {
        if (printemps.mid(y, 1) == "0")
        {
            ui->tableWidget_culture_printemps->item(0, y)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_culture_printemps->item(0, y)->setBackground(Qt::green);
        }
        pos = y;
    }


    for (int z = 0; z < ui->tableWidget_culture_printemps->columnCount(); z++)
    {
        if (printemps.mid(pos + z + 1, 1) == "0")
        {
            ui->tableWidget_culture_printemps->item(1, z)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_culture_printemps->item(1, z)->setBackground(Qt::gray);
        }
    }


    //table automne
    pos = 0;
    for (int y = 0; y < ui->tableWidget_culture_automne->columnCount(); y++)
    {
        if (automne.mid(y, 1) == "0")
        {
            ui->tableWidget_culture_automne->item(0, y)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_culture_automne->item(0, y)->setBackground(Qt::green);
        }
        pos = y;
    }


    for (int z = 0; z < ui->tableWidget_culture_automne->columnCount(); z++)
    {
        if (automne.mid(pos + z + 1, 1) == "0")
        {
            ui->tableWidget_culture_automne->item(1, z)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_culture_automne->item(1, z)->setBackground(Qt::gray);
        }
    }
}

/**********************************fiche especes*****************************/
void Fiche_plantes::on_tableView_especes_clicked(const QModelIndex&index)
{
    int     row       = index.row();
    QString id_espece = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row, 0)).toString();
    QString str       = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row, 1)).toString();
    QString str2      = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row, 2)).toString();
    QString str3      = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row, 3)).toString();
    QString str4      = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row, 4)).toString();
    QString str5      = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row, 5)).toString();
    QString str6      = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row, 6)).toString();
    QString str7      = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row, 7)).toString();
    QString str8      = ui->tableView_especes->model()->data(ui->tableView_especes->model()->index(row, 8)).toString();

    ui->lineEdit_designation_espece->setText(str);
    //selection famille dans combobox
    QSqlQuery query;
    QString   resultat;
    query.exec(QString("select famille from especes where designation ='" + util::apos(str) + "'"));
    if (query.first())
    {
        resultat = query.value(0).toString();
        // qDebug() << resultat;
        for (int index = 0; index < ui->tableView_familles->model()->rowCount(); index++)
        {
            QString id = ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(index, 0)).toString();
            QString designation_famille =
                ui->tableView_familles->model()->data(ui->tableView_familles->model()->index(index, 1)).toString();
            if (id == resultat)
            {
                ui->comboBox_familles_de_especes->setCurrentIndex(ui->comboBox_familles_de_especes->findText(designation_famille));
            }
        }
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }

    ui->lineEdit_Id_especes->setText(id_espece);
    ui->textEdit_positif->setText(str3);
    ui->textEdit_negatif->setText(str4);
    ui->textEdit_especes_commentaires->setText(str5);                 //commentaires
    ui->comboBox_rotation_compost->setCurrentIndex(str6.toInt() - 1); //compost
    ui->comboBox_rotation_annees->setCurrentIndex(str7.toInt() - 1);  //rotation années
    if (str8 == "")
    {
        ui->lineEdit_valeur_rotation_familles->setText("0000000000000000000000");
    }
    else
    {
        ui->lineEdit_valeur_rotation_familles->setText(str8);//rotation familles précédentes
    }
}

void Fiche_plantes::on_pushButton_enregistrer_especes_clicked()
{
    QString   str1 = util::apos(ui->lineEdit_designation_espece->text());
    QSqlQuery query;
    QString   nom_famille = util::apos(ui->comboBox_familles_de_especes->currentText());
    QString   id_famille;

    query.exec(QString("select id from familles where designation ='" + nom_famille + "'"));
    if (query.first())
    {
        id_famille = query.value(0).toString();
        // qDebug() << nom_famille <<" id "<<id_famille;
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    QString str2 = id_famille;
    QString str3 = util::apos(ui->textEdit_positif->document()->toPlainText());
    QString str4 = util::apos(ui->textEdit_negatif->document()->toPlainText());
    QString str5 = util::apos(ui->textEdit_especes_commentaires->document()->toPlainText()); //commentaires
    QString str6 = QString::number(ui->comboBox_rotation_compost->currentIndex() + 1);       //compost
    QString str7 = QString::number(ui->comboBox_rotation_annees->currentIndex() + 1);        //rotation années
    QString str8 = ui->lineEdit_valeur_rotation_familles->text();                            //rotation familles précédentes


    QString str =
        "insert into especes (designation,famille,positif,negatif,commentaires, compost, rotation_ans,rotation_familles)"
        "values('" + str1 + "'," + str2 + ",'" + str3 + "','" + str4 + "','" + str5 + "'," + str6 + "," + str7 + ",'" +
        str8 + "')";
    query.exec(str);
    qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    else
    {
        qDebug() << "enregistrement terminé";
    }

    QSqlQueryModel *model1 = new QSqlQueryModel;
    model1->setQuery(
        "SELECT id, designation, famille ,positif,negatif,commentaires, compost, rotation_ans,rotation_familles FROM especes ORDER BY designation ASC");
    model1->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model1->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model1->setHeaderData(1, Qt::Vertical, QObject::tr("famille"));
    ui->tableView_especes->setModel(model1);
    init_base();
}

void Fiche_plantes::on_pushButton_Modifier_especes_clicked()
{
    QString   str1 = ui->lineEdit_designation_espece->text();
    QSqlQuery query;
    QString   nom_famille = util::apos(ui->comboBox_familles_de_especes->currentText());
    QString   id_famille;

    query.exec(QString("select id from familles where designation ='" + nom_famille + "'"));
    if (query.first())
    {
        id_famille = query.value(0).toString();
        // qDebug() << nom_famille <<" id "<<id_famille;
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }



    QString str2 = id_famille;
    QString str3 = util::apos(ui->textEdit_positif->document()->toPlainText());
    QString str4 = util::apos(ui->textEdit_negatif->document()->toPlainText());
    QString str5 = util::apos(ui->textEdit_especes_commentaires->document()->toPlainText()); //commentaires
    QString str6 = QString::number(ui->comboBox_rotation_compost->currentIndex() + 1);       //compost
    QString str7 = QString::number(ui->comboBox_rotation_annees->currentIndex() + 1);        //rotation années
    QString str8 = ui->lineEdit_valeur_rotation_familles->text();                            //rotation familles précédentes

    QString str = "update especes set designation = '" + str1 + "',famille=" + str2 + ",positif='" + str3 + "',negatif='" + str4 +
                  "',commentaires='" + str5.toHtmlEscaped() + "',compost=" + str6 + ",rotation_ans=" + str7 +
                  ",rotation_familles='" + str8 +
                  "' where id=" + ui->lineEdit_Id_especes->text();

    query.exec(str);
    qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    else
    {
        qDebug() << "enregistrement terminé";
    }

    QSqlQueryModel *model1 = new QSqlQueryModel;
    model1->setQuery(
        "SELECT id, designation, famille,positif,negatif, commentaires, compost, rotation_ans,rotation_familles FROM especes ORDER BY designation ASC");
    model1->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model1->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model1->setHeaderData(1, Qt::Vertical, QObject::tr("famille"));
    ui->tableView_especes->setModel(model1);
    init_base();
}

void Fiche_plantes::on_pushButton_Nouveau_especes_clicked()
{
    ui->lineEdit_designation_espece->setText("");
    ui->comboBox_familles_de_especes->setCurrentIndex(0);
    ui->lineEdit_Id_especes->setText("");
    ui->textEdit_positif->setText("");
    ui->textEdit_negatif->setText("");
    ui->textEdit_especes_commentaires->setText("");
    ui->comboBox_rotation_compost->setCurrentIndex(0);
    ui->comboBox_rotation_annees->setCurrentIndex(0);
    ui->lineEdit_valeur_rotation_familles->setText("0000000000000000000000");
}

void Fiche_plantes::on_pushButton_Supprimer_especes_clicked()
{
    QMessageBox msgBox;

    msgBox.setInformativeText("Voulez-vous supprimer cette espèce ?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Ok:
        QString   strId = ui->lineEdit_Id_especes->text();
        QString   str   = "delete from especes where id=" + strId;
        QSqlQuery query;
        query.exec(str);
        qDebug() << "delete: " << query.lastQuery().toUtf8();

        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver();
        }
        else
        {
            qDebug() << "suppression terminée";
        }
        QSqlQueryModel *model1 = new QSqlQueryModel;
        model1->setQuery(
            "SELECT id, designation, famille,positif,negatif, commentaires, compost, rotation_ans,rotation_familles FROM especes ORDER BY designation ASC");
        model1->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        model1->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        model1->setHeaderData(1, Qt::Vertical, QObject::tr("famille"));
        ui->tableView_especes->setModel(model1);
        on_pushButton_Nouveau_especes_clicked();
        init_base();

        break;
    }
}

/***********************************fiche familles***************************/



void Fiche_plantes::on_pushButton_enregistrer_familles_clicked()
{
    QSqlQuery query;
    QString   famille = util::apos(ui->lineEdit_Designation_famille->text());
    QString   couleur = ui->label_couleur->text();
    QString   str     = "insert into familles (designation,couleur)"
                        "values('" + famille + "','" + couleur + "')";

    query.exec(str);
    qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    else
    {
        qDebug() << "enregistrement terminé";
    }

    QSqlQueryModel *model3 = new QSqlQueryModel;
    model3->setQuery("SELECT id, designation FROM familles");
    model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    ui->tableView_familles->setModel(model3);
    init_base();
}

void Fiche_plantes::on_pushButton_Modifier_familles_clicked()
{
    QSqlQuery query;
    QString   famille = util::apos(ui->lineEdit_Designation_famille->text());
    QString   couleur = ui->label_couleur->text();
    QString   str     = "update familles set designation = '" + famille + "',couleur='" + couleur + "' where id=" +
                        ui->lineEdit_Id_familles->text();

    query.exec(str);
    qDebug() << "PREPARE: " << query.lastQuery().toUtf8();

    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    else
    {
        qDebug() << "enregistrement terminé";
    }

    QSqlQueryModel *model3 = new QSqlQueryModel;
    model3->setQuery("SELECT id, designation FROM familles");
    model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));

    ui->tableView_familles->setModel(model3);
    init_base();
}

void Fiche_plantes::on_pushButton_Nouveau_familles_clicked()
{
    ui->lineEdit_Designation_famille->setText("");
    ui->lineEdit_Id_familles->setText("");
}

void Fiche_plantes::on_pushButton_Supprimer_familles_clicked()
{
    QMessageBox msgBox;

    msgBox.setInformativeText("Voulez-vous supprimer cette famille ?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Ok:
        QString   strId = ui->lineEdit_Id_familles->text();
        QString   str   = "delete from familles where id=" + strId;
        QSqlQuery query;
        query.exec(str);
        qDebug() << "delete: " << query.lastQuery().toUtf8();

        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver();
        }
        else
        {
            qDebug() << "suppression terminée";
        }
        QSqlQueryModel *model3 = new QSqlQueryModel;
        model3->setQuery("SELECT id, designation FROM familles");
        model3->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        model3->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        ui->tableView_familles->setModel(model3);
        on_pushButton_Nouveau_familles_clicked();
        init_base();

        break;
    }
}

/***********************************ROTATIONS******************************************/

void Fiche_plantes::on_tableWidget_rotation_cellClicked(int row, int column)
{
    // modification du tableau des rotations de cultures et mise à jour de la valeur à enregistre dans la base

    QColor case_color = ui->tableWidget_rotation->item(row, column)->background().color();

    if (row == 0)
    {
        if (case_color == Qt::green)
        {
            ui->tableWidget_rotation->item(row, column)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_rotation->item(row, column)->setBackground(Qt::green);
        }
    }
    else
    {
        if (case_color == Qt::red)
        {
            ui->tableWidget_rotation->item(row, column)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_rotation->item(row, column)->setBackground(Qt::red);
        }
    }
    //mise à jour de la valeur rotation
    QString rotation = "";
    for (int y = 0; y < ui->tableWidget_rotation->columnCount(); y++)
    {
        if (ui->tableWidget_rotation->item(0, y)->background().color() == Qt::green)
        {
            rotation = rotation + "1";
        }
        else
        {
            rotation = rotation + "0";
        }
    }

    for (int y = 0; y < ui->tableWidget_rotation->columnCount(); y++)
    {
        if (ui->tableWidget_rotation->item(1, y)->background().color() == Qt::red)
        {
            rotation = rotation + "1";
        }
        else
        {
            rotation = rotation + "0";
        }
    }
    ui->lineEdit_valeur_rotation_familles->setText(rotation);
}

void Fiche_plantes::on_lineEdit_valeur_rotation_familles_textChanged(const QString&arg1)
{
    //mise à jour du tableau des rotations de culture

    int pos = 0;

    for (int y = 0; y < ui->tableWidget_rotation->columnCount(); y++)
    {
        if (arg1.mid(y, 1) == "0")
        {
            ui->tableWidget_rotation->item(0, y)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_rotation->item(0, y)->setBackground(Qt::green);
        }
        pos = y;
    }


    for (int z = 0; z < ui->tableWidget_rotation->columnCount(); z++)
    {
        if (arg1.mid(pos + z + 1, 1) == "0")
        {
            ui->tableWidget_rotation->item(1, z)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_rotation->item(1, z)->setBackground(Qt::red);
        }
    }
}

void Fiche_plantes::on_lineEdit_valeur_rotation_familles_2_textChanged(const QString&arg1)
{   //mise à jour du tableau des rotations de culture fiche plante
    int pos = 0;

    for (int y = 0; y < ui->tableWidget_rotation_2->columnCount(); y++)
    {
        if (arg1.mid(y, 1) == "0")
        {
            ui->tableWidget_rotation_2->setItem(0, y, new QTableWidgetItem);
            ui->tableWidget_rotation_2->item(0, y)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_rotation_2->setItem(0, y, new QTableWidgetItem);
            ui->tableWidget_rotation_2->item(0, y)->setBackground(Qt::green);
        }
        pos = y;
    }

    for (int z = 0; z < ui->tableWidget_rotation_2->columnCount(); z++)
    {
        if (arg1.mid(pos + z + 1, 1) == "0")
        {
            ui->tableWidget_rotation_2->setItem(1, z, new QTableWidgetItem);
            ui->tableWidget_rotation_2->item(1, z)->setBackground(Qt::white);
        }
        else
        {
            ui->tableWidget_rotation_2->setItem(1, z, new QTableWidgetItem);
            ui->tableWidget_rotation_2->item(1, z)->setBackground(Qt::red);
        }
    }
}

void Fiche_plantes::on_pushButton_fermer_clicked()
{
    setIdPlante(ui->lineEdit_Id_plantes->text().toInt());
    close();
}

void Fiche_plantes::on_toolButton_clicked()
{
    setIdPlante(ui->lineEdit_Id_plantes->text().toInt());
    close();
}

void Fiche_plantes::on_toolButton_CouleurFond_clicked()
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
