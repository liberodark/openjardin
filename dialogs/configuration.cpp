#include "configuration.h"
#include "ui_configuration.h"
#include "mainwindow.h"
#include "utilitaires/util.h"
#include <QDebug>
#include <QMouseEvent>
#include <QtWidgets>
#include <QDialog>
#include <QDir>
//xml
#include <QDomDocument>


Configuration::Configuration(const QString&fileNameXML, const QString&fileNameSQL, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuration)
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
    init_base();
    ui->lineEdit_config_nom_Xml->setText(fileNameXML);
    ui->lineEdit_config_nom_base->setText(fileNameSQL);
    setXmlFilName(fileNameXML);
    setSqlFileName(fileNameSQL);
}

Configuration::~Configuration()
{
    delete ui;
}

void Configuration::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    accept();
    close();
}

void Configuration::on_pushButton_fermer_clicked()
{
    close();
}

/****************remplacement des apostrophes dans les QString pour les requêtes sql*************/


void Configuration::createConnection(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        ui->plainTextMessages->insertPlainText("Erreur de lecture du fichier : " + fileName + "\n");
        ui->plainTextMessages->insertPlainText("Vérifier le chemin d'accès au fichier !!\n");
        return;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName(fileName);
    if (!db.open())
    {
        qDebug() << "connection database erreur " << fileName;
    }
    else
    {
        qDebug() << "database open " << fileName;
        ui->lineEdit_config_nom_base->setText(fileName);
        ui->plainTextMessages->insertPlainText("Base de données ouverte : " + fileName + "\n");
    }
    setSqlFileName(fileName);
}

void Configuration::init_base()
{   /***********************initialisation des bases de données sqlite******************/
    //TACHES
    QSqlQueryModel *model4 = new QSqlQueryModel;

    model4->setQuery("SELECT id,designation FROM taches ORDER BY designation ASC");
    model4->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model4->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    ui->tableView_taches->setModel(model4);
}

void Configuration::on_pushButton_changeDataBase_clicked()
{
    // changer la base de données active
    QSqlQuery query;
    QString   repertoire = QDir::homePath() + "/openjardin/";

    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Ouverture d'une base de données"),
                                     repertoire,
                                     tr("SQLI files (*.sqli *.db);; All files (*.*) "));

    // tr("SQLI Files (*.sqli *.db)"));

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        ui->plainTextMessages->insertPlainText("Erreur de lecture du fichier : " + fileName + "\n");
        return;
    }
    else
    {
        ui->plainTextMessages->insertPlainText("Base de données ouverte :" + fileName + "\n");
        createConnection(fileName);
    }
}

void Configuration::enregistrerDataBase()
{
    // enregistrer le nom de la base de données dans le fichier XML
    QString      cellvalue;
    QString      champ;
    QDomDocument document;
    QDomNode     xmlNode = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

    document.insertBefore(xmlNode, document.firstChild());
    QDomElement root = document.createElement("root");
    document.appendChild(root);
    // Création de l'élément <base>
    QDomElement base = document.createElement("base");

    // On ajoute l'élément <background> en tant que premier enfant de l'élément <root>
    root.appendChild(base);
    //Ecriture du nom du fichier de la base de données sqlite
    QDomElement fichier_base = document.createElement("fichier_base");
    cellvalue = ui->lineEdit_config_nom_base->text();
    champ     = "fichier";
    fichier_base.setAttribute(champ, cellvalue);
    base.appendChild(fichier_base);
    // Ecriture dans le fichier
    QString repertoire = QDir::homePath() + "/openjardin";
    QString fileName   =
        QFileDialog::getSaveFileName(this, tr("Sauvegarde du tableau des objets"),
                                     repertoire,
                                     tr("XML Files (*.xml)"));
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ERREUR D'ÉCRITURE");
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
    }
}

void Configuration::on_pushButton_NewdatabaseFull_clicked()
{
    //  créer une base vide et importer les données dans cette base de données
    ui->plainTextMessages->insertPlainText("Début de l'importation des données\n");
    QString repertoire = QDir::homePath() + "/openjardin";
    QString fileName   =
        QFileDialog::getSaveFileName(this, tr("Créer une nouvelle base de données préremplie"),
                                     repertoire,
                                     tr("SQLI Files (*.sqli *.db)"));
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ERREUR D'ÉCRITURE");
        return;
    }
    else
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("");
        db.setDatabaseName(fileName);
        if (!db.open())
        {
            qDebug() << "connection database erreur " << fileName;
        }
        else
        {
            qDebug() << "database open " << fileName;
            ui->plainTextMessages->insertPlainText("Base de données ouverte :" + fileName + "\n");
        }
        QString queryStr;
        QFile   file(":/sql/restore.sql");
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("ERREUR D'ÉCRITURE");
            return;
        }
        else
        {
            QTextStream stream(&file);

            queryStr = stream.readAll();

            file.close();
        }

        QSqlQuery query;



        ui->plainTextMessages->insertPlainText("Données importée dans la nouvelle base \n");
        //Vérifier si le driver SQL supporte les Transactions
        if (db.driver()->hasFeature(QSqlDriver::Transactions))
        {
            //Remplace les commentaires, tabulations et nouvelles lignes avec des espaces
            queryStr =
                queryStr.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                                    QRegularExpression::CaseInsensitiveOption |
                                                    QRegularExpression::MultilineOption), " ");
            //Supprimer les espaces inutiles
            queryStr = queryStr.trimmed();

            //Extraction des requêtes
            QStringList qList = queryStr.split(';', Qt::SkipEmptyParts);
            //Initialise les expressions pour détecter les requêts spéciales(`begin transaction` and `commit`).
            QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
            QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

            bool isStartedWithTransaction = re_transaction.match(qList.at(0)).hasMatch();
            if (!isStartedWithTransaction)
            {
                db.transaction();
            }
            //Executer chaque requête individuellement

            for(const QString &s : qList)
            {
                if (re_transaction.match(s).hasMatch())        //<== detection des requêtes spéciales
                {
                    db.transaction();
                }
                else if (re_commit.match(s).hasMatch())        //<== detection des requêtes spéciales
                {
                    db.commit();
                }
                else
                {
                    query.exec(s);                             //<== executer les requêtes normales
                    if (query.lastError().type() != QSqlError::NoError)
                    {
                        // qDebug() << "erreur 262 " << query.lastError().text() << query.lastQuery();

                        db.rollback();                         //<== rollback la transaction s'il y a un probême
                        ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
                    }
                    else
                    {
                        //  qDebug() << "269 " << query.lastQuery();
                    }
                }
            }
            ui->plainTextMessages->insertPlainText("Importation des données terminée \n");

            if (!isStartedWithTransaction)
            {
                db.commit();
            }
        }
        else
        {         //si le driver sql ne support pas les transactions
                  //...il est nécessaire de supprimer les requêtes spéciales(`begin transaction` and `commit`)
            queryStr =
                queryStr.replace(QRegularExpression(
                                     "(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                     QRegularExpression::CaseInsensitiveOption |
                                     QRegularExpression::MultilineOption), " ");
            queryStr = queryStr.trimmed();

            //Executer chaque requête individuellement
            QStringList qList = queryStr.split(';', Qt::SkipEmptyParts);
            for(const QString &s : qList)
            {
                query.exec(s);
                if (query.lastError().type() != QSqlError::NoError)
                {
                    ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
                }
                else
                {
                }
                ui->plainTextMessages->insertPlainText("Importation des données terminée \n");
            }
        }
    }
}

void Configuration::on_pushButton_import_clicked()
{
    //  créer une base vide et importer les données dans cette base de données
    ui->plainTextMessages->insertPlainText("Début de l'importation des données\n");
    QString repertoire = QDir::homePath() + "/openjardin";
    QString fileName   =
        QFileDialog::getSaveFileName(this, tr("Nom de la nouvelle base de données"),
                                     repertoire,
                                     tr("SQLI Files (*.sqli *.db)"));
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ERREUR D'ÉCRITURE");
        return;
    }
    else
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("");
        db.setDatabaseName(fileName);
        if (!db.open())
        {
            qDebug() << "connection database erreur " << fileName;
        }
        else
        {
            qDebug() << "database open " << fileName;
            ui->plainTextMessages->insertPlainText("Base de données ouverte :" + fileName + "\n");
        }

        QSqlQuery query;
        QString   repertoire = QDir::homePath() + "/openjardin/";

        QString fileNameImport =
            QFileDialog::getOpenFileName(this, tr("Choisir le fichier .sql à importer dans la base de données"),
                                         repertoire,
                                         tr("fichier SQL (*.sql)"));
        if (fileNameImport.isEmpty())
        {
            return;
        }

        QFile fileImport(fileNameImport);
        if (!fileImport.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("ERREUR DE LECTURE");
            return;
        }
        else
        {
            //lecture du fichier .sql
            QTextStream Str(&fileImport);
            QString     queryStr;
            queryStr = Str.readAll();
            fileImport.close();
            ui->plainTextMessages->insertPlainText("Fichier importé :" + fileNameImport + "\n");
            //Vérifier si le driver SQL supporte les Transactions
            if (db.driver()->hasFeature(QSqlDriver::Transactions))
            {
                //Remplace les commentaires, tabulations et nouvelles lignes avec des espaces
                queryStr =
                    queryStr.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                                        QRegularExpression::CaseInsensitiveOption |
                                                        QRegularExpression::MultilineOption), " ");
                //Supprimer les espaces inutiles
                queryStr = queryStr.trimmed();

                //Extraction des requêtes
                QStringList qList = queryStr.split(';', Qt::SkipEmptyParts);
                //Initialise les expressions pour détecter les requêts spéciales(`begin transaction` and `commit`).
                QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
                QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

                bool isStartedWithTransaction = re_transaction.match(qList.at(0)).hasMatch();
                if (!isStartedWithTransaction)
                {
                    db.transaction();
                }
                //Executer chaque requête individuellement
                for(const QString &s : qList)
                {
                    if (re_transaction.match(s).hasMatch())    //<== detection des requêtes spéciales
                    {
                        db.transaction();
                    }
                    else if (re_commit.match(s).hasMatch())    //<== detection des requêtes spéciales
                    {
                        db.commit();
                    }
                    else
                    {
                        query.exec(s);                        //<== executer les requêtes normales
                        if (query.lastError().type() != QSqlError::NoError)
                        {
                            qDebug() << query.lastError().text();
                            qDebug() << s;
                            db.rollback();                    //<== rollback la transaction s'il y a un probême
                            ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
                        }
                    }
                }
                ui->plainTextMessages->insertPlainText("Importation des données terminée \n");

                if (!isStartedWithTransaction)
                {
                    db.commit();
                }
            }
            else  //si le driver sql ne support pas les transactions

            {     //...il est nécessaire de supprimer les requêtes spéciales(`begin transaction` and `commit`)
                queryStr =
                    queryStr.replace(QRegularExpression(
                                         "(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                         QRegularExpression::CaseInsensitiveOption |
                                         QRegularExpression::MultilineOption), " ");
                queryStr = queryStr.trimmed();

                //Executer chaque requête individuellement
                QStringList qList = queryStr.split(';', Qt::SkipEmptyParts);
                for(const QString &s : qList)
                {
                    query.exec(s);
                    if (query.lastError().type() != QSqlError::NoError)
                    {
                        qDebug() << query.lastError().text();
                        ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
                    }
                    else
                    {
                    }
                    ui->plainTextMessages->insertPlainText("Importation des données terminée \n");
                }
            }
        }
    }
}

/**********************************EXPORT VERS UN FICHIER TEXTE DES REQUETTES SQL************************/
void Configuration::on_pushButton_export_clicked()
{
    // export des données dans un fichier de requêtes .sql
    QString repertoire = QDir::homePath() + "/openjardin";
    QString fileName   =
        QFileDialog::getSaveFileName(this, tr("Création d'un export de la base de données"),
                                     repertoire,
                                     tr("Fichier SQL (*.sql)"));

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ERREUR D'ÉCRITURE");
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << "BEGIN TRANSACTION;\n";
        QSqlQuery   query;
        QStringList tables;
        query.exec("SELECT * FROM sqlite_master ");
        int     i2 = 0;
        QString valeur_tables;
        //requêtes CREATE des tables
        while (query.next())
        {
            if (i2 > 0)
            {
                if (query.value("type").toString() == "table")
                {
                    tables << query.value("name").toString();
                }
                valeur_tables = query.value("sql").toString();
                valeur_tables.replace(QString("\""), QString("`"));
                valeur_tables.replace(QString("\n"), QString(""));
                valeur_tables.replace(QString("\t"), QString(" "));
                qDebug().noquote() << valeur_tables.toUtf8() << ";\n";;
                stream << valeur_tables.toUtf8().constData() << ";\n";
            }
            i2++;
        }
        //requêtes INSERT des données
        QString    values;
        QString    columns;
        QString    insert;
        QSqlRecord record;

        for(const QString &table : tables)
        {
            query.exec(QString("SELECT * FROM [%1]").arg(table));
            while (query.next())
            {
                record  = query.record();
                columns = "";
                values  = "";
                for (int i = 0; i < record.count(); i++)
                {
                    if (columns == "")
                    {
                        columns = record.fieldName(i);
                    }
                    else
                    {
                        columns = columns + "," + record.fieldName(i);
                    }

                    if (record.field(i).metaType() == QMetaType::fromType<QString>())
                    {
                        if (values == "")
                        {
                            values = "'" + util::apos(record.value(i).toString()) + "'";
                        }
                        else
                        {
                            values = values + " , " + "'" + util::apos(record.value(i).toString()) + "'";
                        }
                    }
                    else
                    {
                        if (values == "")
                        {
                            values = record.value(i).toString();
                        }
                        else
                        {
                            if (record.value(i).toString() == "")
                            {
                                values = values + " , " + "NULL";
                            }
                            else
                            {
                                values = values + " , " + record.value(i).toString();
                            }
                        }
                    }
                }
                insert = "INSERT INTO `" + table + "` (" + columns + ") VALUES (" + values + ");\n";
                stream << insert;
            }
        }
        stream << "COMMIT;";
        file.close();
    }
}

void Configuration::on_pushButton_Modifier_operations_clicked()
{
    QSqlQuery query;
    QString   tache = util::apos(ui->lineEdit_designation_taches->text());
    QString   str   = "update taches set designation = '" + tache + "' where id=" + ui->lineEdit_Id_taches->text();

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

    QSqlQueryModel *model4 = new QSqlQueryModel;
    model4->setQuery("SELECT id, designation FROM taches");
    model4->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model4->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));

    ui->tableView_taches->setModel(model4);
    //  init_base();
}

void Configuration::on_pushButton_Supprimer_operations_clicked()
{
    QMessageBox msgBox;

    msgBox.setInformativeText("Voulez-vous supprimer cette opération ?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Ok:
        QString   strId = ui->lineEdit_Id_taches->text();
        QString   str   = "delete from taches where id=" + strId;
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
        QSqlQueryModel *model4 = new QSqlQueryModel;
        model4->setQuery("SELECT id, designation FROM taches");
        model4->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
        model4->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
        ui->tableView_taches->setModel(model4);
        on_pushButton_Nouveau_operations_clicked();
        //    init_base();

        break;
    }
}

void Configuration::on_pushButton_Nouveau_operations_clicked()
{
    ui->lineEdit_designation_taches->setText("");
    ui->lineEdit_Id_taches->setText("");
}

void Configuration::on_pushButton_enregistrer_operations_clicked()
{
    QSqlQuery query;
    QString   tache = util::apos(ui->lineEdit_designation_taches->text());
    QString   str   = "insert into taches (designation)"
                      "values('" + tache + "')";

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

    QSqlQueryModel *model4 = new QSqlQueryModel;
    model4->setQuery("SELECT id, designation FROM taches");
    model4->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model4->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));

    ui->tableView_taches->setModel(model4);
    // init_base();
}

void Configuration::on_tableView_taches_clicked(const QModelIndex&index)
{
    int     row      = index.row();
    QString id_tache = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 0)).toString();
    QString str      = ui->tableView_taches->model()->data(ui->tableView_taches->model()->index(row, 1)).toString();

    ui->lineEdit_designation_taches->setText(str);
    ui->lineEdit_Id_taches->setText(id_tache);
}

void Configuration::on_pushButton_mise_a_jour_clicked()
{   // Mise à jour d'une ancienne base de données vers la version 1.06
    QString      fileName(getSqlFileName());
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName(fileName);
    if (!db.open())
    {
        qDebug() << "connection database erreur " << fileName;
    }
    else
    {
        qDebug() << "database open " << fileName;
        ui->plainTextMessages->insertPlainText("Base de données ouverte :" + fileName + "\n");
    }

    QString queryStr;
    QFile   file(":/sql/planner.sql");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ERREUR D'ÉCRITURE");
        return;
    }
    else
    {
        QTextStream stream(&file);

        queryStr = stream.readAll();

        file.close();
    }

    QSqlQuery query;



    ui->plainTextMessages->insertPlainText("Données importée dans la nouvelle base \n");
    //Vérifier si le driver SQL supporte les Transactions
    if (db.driver()->hasFeature(QSqlDriver::Transactions))
    {
        //Remplace les commentaires, tabulations et nouvelles lignes avec des espaces
        queryStr =
            queryStr.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                                QRegularExpression::CaseInsensitiveOption |
                                                QRegularExpression::MultilineOption), " ");
        //Supprimer les espaces inutiles
        queryStr = queryStr.trimmed();

        //Extraction des requêtes
        QStringList qList = queryStr.split(';', Qt::SkipEmptyParts);
        //Initialise les expressions pour détecter les requêts spéciales(`begin transaction` and `commit`).
        QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
        QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

        bool isStartedWithTransaction = re_transaction.match(qList.at(0)).hasMatch();
        if (!isStartedWithTransaction)
        {
            db.transaction();
        }
        //Executer chaque requête individuellement

        for(const QString &s : qList)
        {
            if (re_transaction.match(s).hasMatch())        //<== detection des requêtes spéciales
            {
                db.transaction();
            }
            else if (re_commit.match(s).hasMatch())        //<== detection des requêtes spéciales
            {
                db.commit();
            }
            else
            {
                query.exec(s);                             //<== executer les requêtes normales
                if (query.lastError().type() != QSqlError::NoError)
                {
                    qDebug() << "erreur 292 " << query.lastError().text() << query.lastQuery();

                    db.rollback();                         //<== rollback la transaction s'il y a un probême
                    ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
                }
                else
                {
                    //  qDebug() << "298 " << query.lastQuery();
                }
            }
        }
        ui->plainTextMessages->insertPlainText("Mise à jour de la base de données terminée \n");

        if (!isStartedWithTransaction)
        {
            db.commit();
        }
    }
    else
    {         //si le driver sql ne support pas les transactions
              //...il est nécessaire de supprimer les requêtes spéciales(`begin transaction` and `commit`)
        queryStr =
            queryStr.replace(QRegularExpression(
                                 "(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                 QRegularExpression::CaseInsensitiveOption |
                                 QRegularExpression::MultilineOption), " ");
        queryStr = queryStr.trimmed();

        //Executer chaque requête individuellement
        QStringList qList = queryStr.split(';', Qt::SkipEmptyParts);
        for(const QString &s : qList)
        {
            query.exec(s);
            if (query.lastError().type() != QSqlError::NoError)
            {
                ui->plainTextMessages->insertPlainText(query.lastError().databaseText() + " \n");
            }
            else
            {
            }
            ui->plainTextMessages->insertPlainText("Mise à jour de la base de données terminée \n");
        }
    }
}
