#include "nouveau_projet.h"
#include "ui_nouveau_projet.h"
#include "mainwindow.h"
#include <QDialog>
#include "consts.h"
#include <QDir>
#include <QFileDialog>
#include "utilitaires/util.h"
#include <QDebug>
#include <QMessageBox>
//xml
#include <QDomDocument>


nouveau_Projet::nouveau_Projet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::nouveau_Projet)
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
    ui->lineEdit_filename->setStyleSheet(
        "QLineEdit { background-color: rgb(255,255,217);border-width: 2px;border-style: solid;border-color: red }");
}

nouveau_Projet::~nouveau_Projet()
{
    delete ui;
}

void nouveau_Projet::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    accept();
    close();
}

void nouveau_Projet::on_toolButton_quitter_clicked()
{   // fermer la fenêtre
    close();
}

void nouveau_Projet::on_toolButton_repertoire_clicked()
{   // choisir le repertoire
    QString repertoire = QDir::homePath() + "/openjardin";

    QString dir = QFileDialog::getExistingDirectory(this, tr("Choisir le répertoire"), repertoire);

    ui->lineEdit_repertoire->setText(dir);
}

void nouveau_Projet::on_pushButton_clicked()
{   // valider le nouveau projet
    //  créer une base préremplie

    QString repertoire = ui->lineEdit_repertoire->text();
    QString fileName   = repertoire + "/" + ui->lineEdit_filename->text() + ".sqli";

    if (fileName.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Nom de fichier vide");
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
        }
        QString queryStr;
        QFile   file(":/sql/restore.sql");
        if (ui->checkBoxEnglish->isChecked() == true){
                    file.setFileName(":/sql/restore_en.sql");
         }

        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("ERREUR lecture");
        //    qDebug() << "117 fichier sql database " << file.fileName() <<"erreur";
            return;
        }
        else
        {
            QTextStream stream(&file);

            queryStr = stream.readAll();

            file.close();
         //   qDebug() << "131 fichier sql database" << file.fileName() << " lu";
        }

        QSqlQuery query;
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
                     //   qDebug() << "erreur 172 " << query.lastError().text() << query.lastQuery();

                        db.rollback();                         //<== rollback la transaction s'il y a un probême
                    }
                    else
                    {
                       // qDebug() << "178 " << query.lastQuery();
                    }
                }
            }

            if (!isStartedWithTransaction)
            {
                db.commit();
                qDebug() << "187 ok";
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
                     qDebug() << "erreur 204 " << query.lastQuery();
                }
                else
                {
                }
                qDebug() << "Importation des données terminée ";
            }
        }
    }
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
    cellvalue = repertoire + "/" + ui->lineEdit_filename->text() + ".sqli";
    champ     = "fichier";
    fichier_base.setAttribute(champ, cellvalue);
    base.appendChild(fichier_base);
    // Ecriture dans le fichier
    repertoire = ui->lineEdit_repertoire->text();
    fileName   = repertoire + "/" + ui->lineEdit_filename->text() + ".xml";
    if (fileName.isEmpty())
    {

        return;
    }

    QFile file2(fileName);
    if (!file2.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ERREUR D'ÉCRITURE");
        qDebug() << "247 erreur écriture xml";
        return;
    }
    else
    {
        QTextStream stream(&file2);
        stream << document.toString();
        file.close();
        setXmlFilName(fileName);
        close();
    }
}

void nouveau_Projet::on_lineEdit_filename_textChanged(const QString&arg1)
{
    Q_UNUSED(arg1)
    if (ui->lineEdit_filename->text() == "")
    {
        ui->lineEdit_filename->setStyleSheet(
            "QLineEdit { background-color: rgb(255,255,217);border-width: 2px;border-style: solid;border-color: red }");
    }
    else
    {
        ui->lineEdit_filename->setStyleSheet(
            "QLineEdit { background-color: rgb(255,255,217); }");
    }
}

void nouveau_Projet::on_checkBoxFrench_toggled(bool checked)
{
    ui->checkBoxEnglish->setChecked(false);
}

void nouveau_Projet::on_checkBoxEnglish_toggled(bool checked)
{
    ui->checkBoxFrench->setChecked(false);
}
