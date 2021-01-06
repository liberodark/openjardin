/****************************************************************************
**
** Copyright (C) 2017 openjardin.eu.
** Contact: https://openjardin.eu
**
** You may use this file under the terms of the GNU GENERAL PUBLIC LICENSE
** Version 3, 29 June 2007  as follows:
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogs/cultures.h"
#include "ui_cultures.h"
#include "graphic/planning_item.h"
#include "dialogs/fiche_plantes.h"
#include "dialogs/dialog_aide.h"
#include "ui_dialog_aide.h"
#include "dialogs/configuration.h"
#include "dialogs/licenses.h"
#include "dialogs/planner.h"
#include "dialogs/dialog_taches.h"
#include "dialogs/dialog_type_de_tache.h"
#include "dialogs/dialog_type_de_moyen.h"
#include "dialogs/dialog_ressources.h"
#include "dialogs/dialog_coordonnees.h"
#include "dialogs/dialog_moyens.h"
#include "dialogs/detail_parcelle.h"
#include "dialogs/nouveau_projet.h"
#include "dialogs/planning.h"
#include "graphic/myitem.h"
#include "graphic/vignette.h"
#include "graphic/mygrilleitem.h"
#include "graphic/mypolygone.h"
#include "graphic/mypolyline.h"
#include "graphic/myvertex.h"
#include "graphic/mygraphicsscene.h"
#include "graphic/cote.h"
#include "graphic/sommet.h"
#include "graphic/background.h"
#include "utilitaires/util.h"
//#include "modeles/parcelle.h"

#include "consts.h"
#include <QMessageBox>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsGridLayout>
#include <QPoint>
#include <QDebug>
#include <QMouseEvent>
#include <QtWidgets>
#include <QDialog>
#include <QDir>
#include <QTimerEvent>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QCoreApplication>
#include <QProcess>
//xml
#include <QDomDocument>
#include <iostream>
#include <fstream>
#include <qtranslator.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mode_modifier = false;
    QCoreApplication::setOrganizationName("openjardin");
    QCoreApplication::setOrganizationDomain("openjardin.eu");
    QCoreApplication::setApplicationName("openjardin");
    QSettings settings;
    //base de données
    QString fileName     = QDir::homePath() + "/openjardin/jardin.sqli"; //emplacement de la base de données utilisée
    QString fileName_usr = "/usr/share/openjardin/jardin.sqli";          //emplacement base de données copiées à l'installation

    // translator
    QTranslator translator;
    QString     fichier = ":/translations/open-jardin_" + util::getLocale();

    translator.load(fichier);
    qApp->installTranslator(&translator);


    //test existance du fichier de la base dans homePath()/openjardin pour la première utilisation

    QFile file(fileName);
    //   if (!file.exists())
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "création du dossier openJardin en cours";
        QString path = QDir::homePath() + "/openjardin";
        QDir    dir(path);
        dir.mkdir(path);
        //copie des fichiers dans le repertoire personnel
        QFile::copy(fileName_usr, fileName);
        QFile::copy("/usr/share/openjardin/jardin.xml", path + "/jardin.xml");
        QFile::copy(":/images/message.png", path + "/message.png");
        QFile::copy("/usr/share/openjardin/jardin_type.png", path + "/jardin_type.png");
        QFile::copy("/usr/share/openjardin/notice_openJardin.pdf", path + "/notice_openJardin.pdf");
        QFile::copy("/usr/share/openjardin/jardinDemo.xml", path + "/jardinDemo.xml");
        QFile::copy("/usr/share/openjardin/jardinDemo.sqli", path + "/jardinDemo.sqli");
        QFile::copy("/usr/share/openjardin/jardin.sqli", path + "/jardin.sqli");
    }

    ui->setupUi(this);

    statusLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(statusLabel);
    ui->statusBar->setSizeGripEnabled(true); //positionnement à droite
    //initialisation des "QGraphicsScene"
    // scene = new QGraphicsScene(this);
    scene = new MyGraphicsScene(this);
    scene->setSceneRect(0, 0, 1900, 1200);
    scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex); // indexation des items de la scene

    MyGraphicsScene *scene2 = dynamic_cast <MyGraphicsScene *> (scene);
    scene2->setMode(MyGraphicsScene::Utilisation);

    scene_planning = new QGraphicsScene(this);
    scene_planning->setSceneRect(0, 0, 5205, 800);//5320
    scene_planning->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    scene_planning2 = new QGraphicsScene(this);
    scene_planning2->setSceneRect(0, 0, 100, 800);
    scene_planning2->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    scene_planning3 = new QGraphicsScene(this);
    scene_planning3->setSceneRect(0, 0, 5320, 100);
    scene_planning3->setItemIndexMethod(QGraphicsScene::BspTreeIndex);


    scene_rotation = new QGraphicsScene(this);
    scene_rotation->setSceneRect(0, 0, 5045, 800);//5140
    scene_rotation->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    scene_rotation2 = new QGraphicsScene(this);
    scene_rotation2->setSceneRect(0, 0, 100, 800);
    scene_rotation2->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    scene_rotation3 = new QGraphicsScene(this);
    scene_rotation3->setSceneRect(0, 0, 5160, 50);
    scene_rotation3->setItemIndexMethod(QGraphicsScene::BspTreeIndex);


    // stylesheets
    // ui->pushButton_Enregistrer_modif_item->setStyleSheet("background-color : #ffff00");
    ui->pushButton_recorPlan->hide();
    ui->pushButton_recorPlan->setStyleSheet("background-color : #ffff00; color: red");
    ui->pushButton_Affiches_fiche->setStyleSheet("background-color : #ffff00");

    qApp->setPalette(this->style()->standardPalette());
    qApp->setStyle(QStyleFactory::create("linux"));
    qApp->setStyleSheet("QLineEdit ,QTextEdit,QTableView { background-color: rgb(255,255,217) }");
    qApp->setStyleSheet("QSplitter::handle{background: cyan;}");

    QList <QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->graphicsView, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(ShowContextMenu(const QPoint&)));
    ui->graphicsView_planning->setScene(scene_planning);
    ui->graphicsView_Planning2->setScene(scene_planning2);
    ui->graphicsView_planning3->setScene(scene_planning3);

    ui->graphicsView_rotation->setScene(scene_rotation);
    ui->graphicsView_rotation2->setScene(scene_rotation2);
    ui->graphicsView_rotation3->setScene(scene_rotation3);

    //SPLITTERS PLANNING ET ROTATIONS
    ui->splitter_2->setCollapsible(1, false);
    ui->splitter_2->setCollapsible(0, false);
//   ui->splitter_2->setSizes(QList <int>() << 250 << 2000);

    ui->splitter->setCollapsible(1, false);
    ui->splitter->setCollapsible(0, false);


    // affichage du planning
    QDate   currentDate = QDate::currentDate();
    QString year        = QString::number(currentDate.year());
    QString strDateFev  = "01/02/" + year;
    //  QDate   dateFev       = QDate::fromString(strDateFev, "dd/MM/yyyy");
    QString strDate = "01/01/" + year;
    //  QDate   date          = QDate::fromString(strDate, "dd/MM/yyyy");
    ui->comboBox_AnneeEnCours->setCurrentText(year);
    affiche_rotation(year.toInt());
    ui->comboBox_AnneeEnCours_2->setCurrentText(year);


    /*******************************************************************************/


    //initialisation de la table des parcelles
    ui->tableWidget_parcelles->setRowCount(0);
    ui->tableWidget_parcelles->setColumnCount(2); //nb de colonnes
    QStringList headersParcelles;
    headersParcelles << "Id" << "Nom";
    ui->tableWidget_parcelles->setHorizontalHeaderLabels(headersParcelles);

    //initialisation de la table des fiches taches
    ui->tableWidget_taches->setRowCount(0);
    ui->tableWidget_taches->setColumnCount(6); //nb de colonnes
    QStringList headersTaches;
    headersTaches << "Id" << "Designation" << "id_Parcelle" << "dateDébut" << "duree" << "type_tache";
    ui->tableWidget_taches->setHorizontalHeaderLabels(headersTaches);

    // positionnement des scrollbar en haut et à gauche
    ui->graphicsView->horizontalScrollBar()->setSliderPosition(1);
    // si la selection de l'item change lancer la commande
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(Item_clicked()));
    connect(scene_planning, SIGNAL(selectionChanged()), this, SLOT(ItemPlanning_clicked()));

    // passer en mode utilisation
    on_actionUtilisation_triggered();



    /**********************fond d'écran standard**************************************/
    QString     fileName2 = QDir::homePath() + ":/images/message.png";
    QPixmap     pim(fileName2);
    background *pixmap = new background();
    pixmap->setPixmap(pim);
    scene->addItem(pixmap);
    //dimensionnement de la scene selon la taille de pixmap
    double sizeW = pixmap->boundingRect().width();
    double sizeH = pixmap->boundingRect().height();
    scene->setSceneRect(0, 0, sizeW, sizeH);


    pixmap->setPixFile(fileName2);
    setFileNameBackGround(fileName2);//ecriture des valeurs dans tableBackground

    //dessin de la grille (taille,transparence)
    setGrille(0);
    m_mode = 1;

    // positionnement des scrollbar en haut et à gauche
    QPointF zero = ui->graphicsView->mapFromScene(QPointF(0, 0));
    ui->graphicsView->horizontalScrollBar()->setValue(int(zero.x()));
    ui->graphicsView->verticalScrollBar()->setValue(int(zero.y()));



    // synchronisation des deux graphicsviews du planning
    connect(ui->graphicsView_planning->verticalScrollBar(), SIGNAL(valueChanged(
                                                                       int)),
            ui->graphicsView_Planning2->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui->graphicsView_planning->horizontalScrollBar(), SIGNAL(valueChanged(
                                                                         int)),
            ui->graphicsView_planning3->horizontalScrollBar(), SLOT(setValue(int)));
    connect(ui->graphicsView_rotation->verticalScrollBar(), SIGNAL(valueChanged(
                                                                       int)),
            ui->graphicsView_rotation2->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui->graphicsView_rotation->horizontalScrollBar(), SIGNAL(valueChanged(
                                                                         int)),
            ui->graphicsView_rotation3->horizontalScrollBar(), SLOT(setValue(int)));

    //repositionnement des scrollbar synchronisées
    ui->graphicsView_planning->verticalScrollBar()->setValue(5);
    ui->graphicsView_planning->verticalScrollBar()->setValue(0);
    ui->graphicsView_rotation->verticalScrollBar()->setValue(5);
    ui->graphicsView_rotation->verticalScrollBar()->setValue(0);


    //cacher l'onglets "objets" qui contient les tables des objets
    ui->tabWidget->removeTab(3);

    // cacher les outils de dessin car affichage uniquement en mode dessin
    ui->frame_ToolsCreate->hide();
    // cacher les champs id
    ui->lineEdit_Id_Item->hide();
    ui->lineEdit_id_culture->hide();


    m_ZoomRatio = 1; //variable du facteur de zoom (actual size =1 )



    loadIniSettings();
    on_pushButton_maj_planning_clicked();
}

/***********************************************************************************/

MainWindow::~MainWindow()
{
    delete ui;
}

/**************************a propos de **********************************/

void MainWindow::on_actionA_propos_de_triggered()
{
    QMessageBox::about(this, tr("A propos de Open_jardin"),
                       tr("Ce programme est utilisé pour gérer graphiquement les plantations d'un potager.\n"
                          "il utilise des fichier XML pour la configuration des plans\n"
                          "et une base sqlite pour les données de culture\n"
                          "Ce programme est compilé avec Qt 5.5.1 .\n"
                          "Openjardin version 1.07.009 license GNU GPL version 3.0"));
}

void MainWindow::on_actionA_propos_de_Qt_triggered()
{
    //QMessageBox::aboutQt(this,tr(QT_VERSION_STR));
    QMessageBox::aboutQt(this, tr("Ce programme utilise la version 5.5.1 de Qt"));
}

void MainWindow::on_actionQuitter_triggered()
{   //quitter l'application
    close();
}

void MainWindow::on_actionLangue_Anglais_triggered()
{
    // forcer la langue anglaise
    QTranslator translator;
    QString     fichier = ":/translations/open-jardin_en.ts";
    translator.load(fichier);
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
    QSettings       settings;
    settings.setValue("langue", "english"); // enregistre dans fichier .conf
}

void MainWindow::on_actionTraduire_en_francais_triggered()
{
    // forcer la langue française
    QTranslator translator;
    QString     fichier = ":/translations/open-jardin_fr.ts";
    translator.load(fichier);
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
    QSettings       settings;
    settings.setValue("langue", "francais"); // enregistre dans fichier .conf
}


/*************************************************************************/
void MainWindow::resizeEvent(QResizeEvent *event)
{   //redimensionne la zone principale du planning et des rotations selon la taille de la fenêtre principale
    int WidthMainWindow = event->size().width();

    ui->graphicsView_planning->setMinimumWidth(WidthMainWindow - 328);
    ui->graphicsView_rotation->setMinimumWidth(WidthMainWindow - 328);
}

/*****************************************************************/

void MainWindow::on_actionNouveau_projet_triggered()
{   // création d'un nouveau projet
    // editer une fiche de type de ressource
    QSettings       settings;
    QString         fileName;
    nouveau_Projet *FicheProjet = new nouveau_Projet();
    int             resultat    = FicheProjet->exec();


    if (resultat == QDialog::Accepted)
    {
        setFileNameXML(FicheProjet->getXmlFilName());
        fileName = getFileNameXML();
        qDebug() << " new filename " << getFileNameXML();
        if (fileName.isEmpty())
        {
            return;
        }
        else
        {
            setFileNameBackGround("");
            settings.setValue("lastfile", fileName); // enregistre dans fichier .conf
            ouvrir_FichierXML(fileName);
        }
    }
}

void MainWindow::loadIniSettings()
{
    QSettings settings;
    // création du document pour lire XML
    QDomDocument document;
    QString      champ;
    QFile        iniFile(settings.fileName());

    if (iniFile.exists())
    {
        QSettings settings(iniFile.fileName(), QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        QString langue = settings.value("langue").toString();
        if(langue == "english")
        {
            on_actionLangue_Anglais_triggered();

        }
        QString fileName = settings.value("lastfile").toString();
        ouvrir_FichierXML(fileName);
    }
    else
    {
        QString fileNameXML = QDir::homePath() + "/openjardin/jardin.xml";
        setFileNameXML(fileNameXML);
        ouvrir_FichierXML(fileNameXML);
        QString fileName = QDir::homePath() + "/openjardin/jardin.sqli";
        setfileNameSQL(fileName);
        createConnection(fileName);
    }
}

void MainWindow::createConnection(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        fileName = QDir::homePath() + "/openjardin/jardin.sqli";
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
        setfileNameSQL(fileName);
        qDebug() << "database open " << fileName;
        ui->statusBar->showMessage("Base de données ouverte : " + fileName, 5000);
    }
    testVersion();
    mise_a_jour_Titre();
}

void MainWindow::mise_a_jour_Titre()
{
    QString fichier(getFileNameXML());
    QString titre(Consts::APP_NAME);

    if (!getFileNameXML().isEmpty())
    {
        titre = titre + " 1.07 - " + fichier;
    }


    setWindowTitle(titre);
}

void MainWindow::testVersion()
{
    QSqlQuery query1;
    QString   strQuery1 = "SELECT name FROM sqlite_master WHERE name='especes'";

    query1.exec(strQuery1);

    if (query1.first())
    {
        QString resultat = query1.value(0).toString();
        qDebug() << "le fichier jardin.sqli existe";
    }
    else
    {
        mise_a_jour_DB(":/sql/restore.sql");
        qDebug() << "le fichier jardin.sqli a été créé ";
        setFileNameXML(QDir::homePath() + "/openjardin/jardin.xml");
    }

    QSqlQuery query;
    QString   strQuery = "SELECT name FROM sqlite_master WHERE name='tasks'";

    query.exec(strQuery);

    if (query.first())
    {
        QString resultat = query.value(0).toString();
        qDebug() << "version 1.06";
    }
    else
    {
        qDebug() << "ancienne version";
        int ret = QMessageBox::warning(this, tr("OPENJARDIN - Mise à jour de la base de données"),
                                       tr("La base de données doit être mise à jour.\n"
                                          "Confirmer la mise à jour en version 1.07 ?"),
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Cancel);

        switch (ret)
        {
        case QMessageBox::Ok:
        {
            // backup du fichier de la base de données
            backup_base();
            mise_a_jour_DB(":/sql/planner.sql");
            break;
        }

        case QMessageBox::Cancel:
        {
            break;
        }

        default:
            break;
        }
    }
}

void MainWindow::backup_base()
{   // backup du fichier sqli avec extension .bak
    QString fileName = getfileNameSQL();
    QFile   file(fileName);
    QString baseName = fileName.split(".", QString::SkipEmptyParts).at(0);

    baseName = baseName + ".bak";
    if (QFile::exists(baseName))
    {
        QFile::remove(baseName);
    }
    QFile::copy(fileName, baseName);
}

void MainWindow::mise_a_jour_DB(QString sqlfileName)
{   // Mise à jour d'une ancienne base de données vers la version 1.06
    QString      fileName(getfileNameSQL());
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
    QFile   file(sqlfileName);
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
    int       drap_erreur = 0;
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
        QStringList qList = queryStr.split(';', QString::SkipEmptyParts);
        //Initialise les expressions pour détecter les requêts spéciales(`begin transaction` and `commit`).
        QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
        QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

        bool isStartedWithTransaction = re_transaction.match(qList.at(0)).hasMatch();
        if (!isStartedWithTransaction)
        {
            db.transaction();
        }
        //Executer chaque requête individuellement

        foreach(const QString &s, qList)
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
                    qDebug() << "erreur 637 " << query.lastError().text() << query.lastQuery();

                    drap_erreur = 1;
                    db.rollback();                         //<== rollback la transaction s'il y a un probême
                }
            }
        }
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
        QStringList qList = queryStr.split(';', QString::SkipEmptyParts);

        foreach(const QString &s, qList)
        {
            query.exec(s);
            if (query.lastError().type() != QSqlError::NoError)
            {
                drap_erreur = 1;
                qDebug() << "erreur mise à jour :" << query.lastError().text() << "  " <<
                    query.lastError().databaseText() << query.lastQuery();
            }
        }
    }
    if (drap_erreur == 0)
    {
        qDebug() << "mise à jour ok ";
        QMessageBox::information(this, tr("Base de données "),
                                 tr("La base de données a été mise à jour en version 1.06"));
        return;
    }
    else
    {
        QMessageBox::information(this, tr("Erreur de mise à jour"),
                                 tr("La base de données n'a pas pu être mise à jour en version 1.06"));
        return;
    }
}

/*****************************************************************************/
/***** lire les données du fichier XML et les importer dans la table**********/
/*****************************************************************************/


void MainWindow::on_actionOuvrir_triggered()
{
    QSettings settings;
    QString   champ;
    // création du document pour lire XML
    QDomDocument document;

    // ouverture du fichier pour lecture
    QString repertoire = QDir::homePath() + "/openjardin/";

    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Ouverture du fichier des objets"),
                                     repertoire,
                                     tr("XML Files (*.xml);; All files (*.*)"));

    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        settings.setValue("lastfile", fileName); // enregistre dans fichier .conf
        ouvrir_FichierXML(fileName);
        setFileNameXML(fileName);
    }
}

void MainWindow::ouvrir_FichierXML(QString fileName)
{
    QDomDocument document;
    QString      champ;
    QFile        nomFichier(fileName);
    int          row_parcelles = 0;

    ui->tableWidget_parcelles->setRowCount(0);


    if (nomFichier.exists())
    {
        QFile file(nomFichier.fileName());
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("ERREUR DE LECTURE");
            return;
        }
        else
        {
            if (!document.setContent(&file))
            {
                //Erreur de lecture du fichier";
            }
            else
            {
                // Element root
                setFileNameXML(fileName);
                QDomElement  root  = document.firstChildElement();
                QDomNodeList base  = root.elementsByTagName("fichier_base");
                QDomNodeList fond  = root.elementsByTagName("fond");
                QDomNodeList objet = root.elementsByTagName("Objet");


                // nom du fichier de la base sqlite qui correspond à la configuration XML
                QDomNode basenode = base.at(0);
                if (basenode.isElement())
                {
                    QDomElement base        = basenode.toElement();
                    QString     fichierBase = base.attribute("fichier");
                    setfileNameSQL(fichierBase);
                    createConnection(fichierBase);
                    qDebug() << " connection base crée " << fichierBase;
                }

                // écriture dans le tableau de l'objet fond d'écran
                scene->clear();
                QDomNode fondnode = fond.at(0);
                if (fondnode.isElement())
                {
                    // qDebug() << "boucle fond element";
                    QDomElement fond = fondnode.toElement();
                    setFileNameBackGround(fond.attribute("chemin"));
                    QString fileNameFond = getFileNameBackGround();
                    QFile   filefond(fileNameFond);
                    if (!filefond.exists())
                    {
                        fileNameFond = QDir::homePath() + "/openjardin/message.png";
                    }
                    background *pixmap = new background();
                    QPixmap     pim(fileNameFond);
                    pixmap->setPixmap(pim);
                    pixmap->setZValue(-3);
                    scene->addItem(pixmap);
                    pixmap->setPixFile(fileNameFond);
                    double sizeW = pixmap->boundingRect().width();
                    double sizeH = pixmap->boundingRect().height();
                    scene->setSceneRect(0, 0, sizeW, sizeH);
                }
                // boucle d'écriture de chaque objet dans le tableau

                for (int i = 0; i < objet.count(); i++)
                {
                    QDomNode objetnode = objet.at(i);
                    if (objetnode.isElement())
                    {
                        QDomElement objet = objetnode.toElement();
                        if (objet.attribute("Type").toInt() == 65536) //parcelleItem
                        {
                            QColor  couleurfond = QRgb(objet.attribute("Brush").toDouble());
                            QColor  couleurpen  = QRgb(objet.attribute("Pen").toDouble());
                            MyItem *item        = new MyItem(objet.attribute("SizeX").toInt(), objet.attribute(
                                                                 "SizeY").toInt());
                            QPen pen(Qt::blue, 1);
                            item->setColor(QColor(couleurfond));
                            item->setPenColor(QColor(Qt::black));
                            item->setOpac(0.8);
                            item->setPos(objet.attribute("PositionX").toInt(), objet.attribute("PositionY").toInt());
                            item->setTexte(objet.attribute("Texte"));
                            item->setTypeShape(objet.attribute("Forme").toInt());// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
                            item->setComment(objet.attribute("Commentaire"));
                            item->setPenColor(QColor(couleurpen));
                            item->setPixFile(objet.attribute("Chemin"));
                            item->setId(objet.attribute("Id").toInt());
                            item->setNom(objet.attribute("Nom"));
                            item->setEtat(objet.attribute("Etat").toInt());
                            item->setMode(objet.attribute("Mode").toInt());
                            if (objet.attribute("TypeLine").toInt() == 0)
                            {
                                item->setTypeLine(1);
                            }
                            else
                            {
                                item->setTypeLine(objet.attribute("TypeLine").toInt());
                            }
                            if (objet.attribute("WidthLine").toInt() == 0)
                            {
                                item->setWidthLine(1);
                            }
                            else
                            {
                                item->setWidthLine(objet.attribute("WidthLine").toInt());
                            }
                            item->setZValue(objet.attribute("ZValue").toDouble());
                            item->setDetailFile(objet.attribute("FichierDetail"));
                            if (objet.attribute("Uuid") == "")
                            {
                                item->setUuid(QUuid::createUuid());
                            }
                            else
                            {
                                item->setUuid(objet.attribute("Uuid"));
                            }
                            scene->addItem(item);
                            if (item->getEtat() == 1)
                            {
                                ui->tableWidget_parcelles->setRowCount(row_parcelles + 1);
                                ui->tableWidget_parcelles->setItem(row_parcelles, 0,
                                                                   new QTableWidgetItem(QString::number(item->getId())));                   //Id
                                ui->tableWidget_parcelles->setItem(row_parcelles, 1, new QTableWidgetItem(item->getNom()));
                                row_parcelles++;
                            }
                        }
                        if (objet.attribute("Type").toInt() == 65537) // MyPolygone
                        {
                            MyPolygone *item = new MyPolygone(convertStrToPoly(objet.attribute("Chemin")));
                            QPen        pen(Qt::blue, 1);
                            QColor      couleurfond = QRgb(objet.attribute("Brush").toDouble());
                            QColor      couleurpen  = QRgb(objet.attribute("Pen").toDouble());
                            item->setColor(QColor(couleurfond));
                            item->setPenColor(QColor(Qt::black));
                            item->setOpac(0.8);
                            item->setPos(objet.attribute("PositionX").toInt(), objet.attribute("PositionY").toInt());
                            item->setTexte(objet.attribute("Texte"));
                            item->setTypeShape(objet.attribute("Forme").toInt());// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
                            item->setComment(objet.attribute("Commentaire"));
                            item->setPenColor(QColor(couleurpen));
                            item->setStrPoly(objet.attribute("Chemin"));
                            item->setId(objet.attribute("Id").toInt());
                            item->setNom(objet.attribute("Nom"));
                            item->setEtat(objet.attribute("Etat").toInt());
                            item->setMode(objet.attribute("Mode").toInt());
                            if (objet.attribute("TypeLine").toInt() == 0)
                            {
                                item->setTypeLine(1);
                            }
                            else
                            {
                                item->setTypeLine(objet.attribute("TypeLine").toInt());
                            }
                            if (objet.attribute("WidthLine").toInt() == 0)
                            {
                                item->setWidthLine(1);
                            }
                            else
                            {
                                item->setWidthLine(objet.attribute("WidthLine").toInt());
                            }
                            item->setZValue(objet.attribute("ZValue").toDouble());
                            item->setDetailFile(objet.attribute("FichierDetail"));
                            if (objet.attribute("Uuid") == "")
                            {
                                item->setUuid(QUuid::createUuid());
                            }
                            else
                            {
                                item->setUuid(objet.attribute("Uuid"));
                            }
                            scene->addItem(item);
                            if (item->getEtat() == 1)
                            {
                                ui->tableWidget_parcelles->setRowCount(row_parcelles + 1);
                                ui->tableWidget_parcelles->setItem(row_parcelles, 0,
                                                                   new QTableWidgetItem(QString::number(item->getId())));                   //Id
                                ui->tableWidget_parcelles->setItem(row_parcelles, 1, new QTableWidgetItem(item->getNom()));
                                row_parcelles++;
                            }
                        }
                        if (objet.attribute("Type").toInt() == 65539) // MyPolyline
                        {
                            MyPolyline *item = new MyPolyline(convertStrToPoly(objet.attribute("Chemin")));
                            QPen        pen(Qt::blue, 1);
                            QColor      couleurfond = QRgb(objet.attribute("Brush").toDouble());
                            QColor      couleurpen  = QRgb(objet.attribute("Pen").toDouble());
                            item->setColor(QColor(couleurfond));
                            item->setPenColor(QColor(Qt::black));
                            item->setOpac(0.8);
                            item->setPos(objet.attribute("PositionX").toInt(), objet.attribute("PositionY").toInt());
                            item->setTexte(objet.attribute("Texte"));
                            item->setTypeShape(objet.attribute("Forme").toInt());// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
                            item->setComment(objet.attribute("Commentaire"));
                            item->setPenColor(QColor(couleurpen));
                            item->setStrPoly(objet.attribute("Chemin"));
                            item->setId(objet.attribute("Id").toInt());
                            item->setNom(objet.attribute("Nom"));
                            item->setEtat(objet.attribute("Etat").toInt());
                            item->setMode(objet.attribute("Mode").toInt());
                            if (objet.attribute("TypeLine").toInt() == 0)
                            {
                                item->setTypeLine(1);
                            }
                            else
                            {
                                item->setTypeLine(objet.attribute("TypeLine").toInt());
                            }
                            if (objet.attribute("WidthLine").toInt() == 0)
                            {
                                item->setWidthLine(1);
                            }
                            else
                            {
                                item->setWidthLine(objet.attribute("WidthLine").toInt());
                            }
                            item->setZValue(objet.attribute("ZValue").toDouble());
                            if (objet.attribute("Uuid") == "")
                            {
                                item->setUuid(QUuid::createUuid());
                            }
                            else
                            {
                                item->setUuid(objet.attribute("Uuid"));
                            }
                            scene->addItem(item);
                        }
                    }
                }
                if (objet.count() > 0)
                {
                    // dessinerParcelles();
                    qDebug() << "Lecture du fichier XML terminée";
                }
            }
        }
        file.close();
    }

    // positionnement des scrollbar en haut et à gauche
    QPointF zero = ui->graphicsView->mapFromScene(QPointF(0, 0));
    ui->graphicsView->horizontalScrollBar()->setValue(int(zero.x()));
    ui->graphicsView->verticalScrollBar()->setValue(int(zero.y()));
    if (ui->actionUtilisation->isChecked())
    {
        on_actionUtilisation_triggered();
    }
    else
    {
        on_actionModification_plan_triggered();
    }
    resize_planning(row_parcelles); //redimensionner la hauteur du planning et rotation selon nombre de parcelles
    get_MaxId();

    // tester si la table des parcelles est vide
    tester_table_parcelles();
}

void MainWindow::tester_table_parcelles()
{
    //vérifier l'existance de la table des parcelles
    QSqlQuery query;
    QString   strQuery = "SELECT name FROM sqlite_master WHERE name='parcelles'";

    query.exec(strQuery);
    if (query.first())
    {
        QString resultat = query.value(0).toString();
        qDebug() << "version 1.07";
    }
    else
    {
        remplir_table_parcelles();
    }
    // vérifier si la table des parcelles est remplie
    strQuery = "SELECT COUNT(*) FROM parcelles";
    query.exec(strQuery);
    if (query.first())
    {
        QString resultat = query.value(0).toString();
        // qDebug() << " nombre de parcelles " << resultat;
        if (resultat.toInt() == 0)
        {
            //Debug() << "remplir table 947";
            remplir_table_parcelles();
        }
    }
}

void MainWindow::remplir_table_parcelles()
{
    QSqlQuery query;
    QString   strQuery =
        "CREATE TABLE IF NOT EXISTS parcelles (id INTEGER UNIQUE PRIMARY KEY,designation TEXT, type INTEGER)";

    query.exec(strQuery);
    qDebug() << "passage en version 1.07 effectué";
    if (!query.isActive())
    {
        qDebug() << "erreur query création table parcelles:" << query.lastError().text() << "  " <<
            query.lastError().databaseText() << query.lastQuery();
    }


    qDebug() << " debut remplissage table des parcelles";
    query.exec(QString("DELETE FROM parcelles"));
    QString designation;
    QString idparcelle;
    //remplir la table des parcelles
    for (int h = 0; h < ui->tableWidget_parcelles->rowCount(); h++)
    {
        if (ui->tableWidget_parcelles->rowCount() > -1)
        {
            idparcelle  = ui->tableWidget_parcelles->item(h, 0)->text();
            designation = ui->tableWidget_parcelles->item(h, 1)->text();
            query.exec(QString("INSERT INTO parcelles (id, designation) VALUES (" + idparcelle + ",'" +
                               designation + "')"));
            if (!query.isActive())
            {
                qDebug() << "erreur query parcelles:" << query.lastError().text() << "  " <<
                    query.lastError().databaseText() << query.lastQuery();
            }
        }
    }
    qDebug() << " fin remplissage table des parcelles";
}

void MainWindow::ajouter_parcelle(QString id, QString designation)
{
    QSqlQuery query;
    QString   strQuery = "INSERT OR REPLACE INTO parcelles (id, designation) VALUES (" + id + ",'" + designation + "')";

    //"INSERT INTO parcelles(id,designation) SELECT "+id+",'"+designation+"' WHERE NOT EXISTS(SELECT 1 FROM parcelles WHERE id = "+id+" AND designation = '"+ designation+"'";
    query.exec(strQuery);

    if (!query.isActive())
    {
        qDebug() << "erreur query ajout parcelle:" << query.lastError().text() << "  " <<
            query.lastError().databaseText() << query.lastQuery();
    }
}

void MainWindow::supprimer_parcelle(QString id)
{
    QSqlQuery query;
    QString   strQuery = "DELETE FROM parcelles WHERE id=" + id;

    query.exec(strQuery);
    if (!query.isActive())
    {
        qDebug() << "erreur query suppression parcelle:" << query.lastError().text() << "  " <<
            query.lastError().databaseText() << query.lastQuery();
    }
}

void MainWindow::on_actionSauver_triggered()
{   // sauver les données de la table dans un fichier XML
    QString cellvalue;
    QString champ;

    // XML
    QDomDocument document;
    QDomNode     xmlNode = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

    document.insertBefore(xmlNode, document.firstChild());
    QDomElement root = document.createElement("root");
    document.appendChild(root);
    QDomElement base = document.createElement("base");
    root.appendChild(base);
    QDomElement background = document.createElement("background");
    root.appendChild(background);
    QDomElement objets = document.createElement("objets");
    root.appendChild(objets);
    QDomElement fichier_base = document.createElement("fichier_base");
    cellvalue = getfileNameSQL();
    champ     = "fichier";
    fichier_base.setAttribute(champ, cellvalue);
    base.appendChild(fichier_base);
    //background
    QDomElement fond = document.createElement("fond");
    fond.setAttribute("chemin", getFileNameBackGround());
    fond.setAttribute("Type", "7");
    fond.setAttribute("PositionX", "0");
    fond.setAttribute("PositionY", "0");
    background.appendChild(fond);

//Ecriture des objets
    QString ObjetId;
    QString ObjetNom;
    QString ObjetType;
    QString ObjetForme;
    QString ObjetPosX;
    QString ObjetPosY;
    QString ObjetWidth;
    QString Objetheight;
    QString ObjetBrush;
    QString ObjetPen;
    QString ObjetComment;
    QString ObjetTexte;
    QString ObjetChemin;
    QString ObjetEtat;
    QString ObjetMode;
    QString ObjetTypeLine;
    QString ObjetWidthLine;
    QString ObjetZValue;
    QString ObjetUuid;
    QString ObjetFichierDetail = "";

    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->pos().x() > 0)
        {
            QDomElement objet = document.createElement("Objet");

            if (itemList[i]->type() == 65536)   // MyItem
            {
                MyItem *item     = dynamic_cast <MyItem *> (itemList[i]);
                QPointF location = itemList[i]->pos();
                int     valX     = int(location.x());
                int     valY     = int(location.y());
                int     sizeW    = int(itemList[i]->boundingRect().width());
                int     sizeH    = int(itemList[i]->boundingRect().height());
                QColor  clr      = item->getColor();
                QColor  clrPen   = item->getPenColor();

                ObjetId            = QString::number(item->getId());
                ObjetNom           = item->getNom();
                ObjetType          = QString::number(65536);
                ObjetForme         = QString::number(item->getTypeShape());
                ObjetPosX          = QString::number(valX);
                ObjetPosY          = QString::number(valY);
                ObjetWidth         = QString::number(sizeW);
                Objetheight        = QString::number(sizeH);
                ObjetBrush         = QString::number(clr.rgb());
                ObjetPen           = QString::number(clrPen.rgb());
                ObjetComment       = item->getComment();
                ObjetTexte         = item->getTexte();
                ObjetChemin        = item->getPixFile();
                ObjetEtat          = QString::number(item->getEtat());
                ObjetMode          = QString::number(item->getMode());
                ObjetTypeLine      = QString::number(item->getTypeLine());
                ObjetWidthLine     = QString::number(item->getWidthLine());
                ObjetZValue        = QString::number(item->zValue());
                ObjetUuid          = item->getUuid().toString();
                ObjetFichierDetail = item->getDetailFile();
            }
            if (itemList[i]->type() == 65537)   // MyPolygone
            {
                MyPolygone *item     = dynamic_cast <MyPolygone *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjetId            = QString::number(item->getId());
                ObjetNom           = item->getNom();
                ObjetType          = QString::number(65537);
                ObjetForme         = QString::number(item->getTypeShape());
                ObjetPosX          = QString::number(valX);
                ObjetPosY          = QString::number(valY);
                ObjetWidth         = QString::number(sizeW);
                Objetheight        = QString::number(sizeH);
                ObjetBrush         = QString::number(clr.rgb());
                ObjetPen           = QString::number(clrPen.rgb());
                ObjetComment       = item->getComment();
                ObjetTexte         = item->getTexte();
                ObjetChemin        = item->getStrPoly();
                ObjetEtat          = QString::number(item->getEtat());
                ObjetMode          = QString::number(item->getMode());
                ObjetTypeLine      = QString::number(item->getTypeLine());
                ObjetWidthLine     = QString::number(item->getWidthLine());
                ObjetZValue        = QString::number(item->zValue());
                ObjetUuid          = item->getUuid().toString();
                ObjetFichierDetail = item->getDetailFile();
            }
            if (itemList[i]->type() == 65539)   // MyPolyline
            {
                MyPolyline *item     = dynamic_cast <MyPolyline *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjetId        = QString::number(item->getId());
                ObjetNom       = item->getNom();
                ObjetType      = QString::number(65539);
                ObjetForme     = QString::number(item->getTypeShape());
                ObjetPosX      = QString::number(valX);
                ObjetPosY      = QString::number(valY);
                ObjetWidth     = QString::number(sizeW);
                Objetheight    = QString::number(sizeH);
                ObjetBrush     = QString::number(clr.rgb());
                ObjetPen       = QString::number(clrPen.rgb());
                ObjetComment   = item->getComment();
                ObjetTexte     = item->getTexte();
                ObjetChemin    = item->getStrPoly();
                ObjetEtat      = QString::number(item->getEtat());
                ObjetMode      = QString::number(item->getMode());
                ObjetTypeLine  = QString::number(item->getTypeLine());
                ObjetWidthLine = QString::number(item->getWidthLine());
                ObjetZValue    = QString::number(item->zValue());
                ObjetUuid      = item->getUuid().toString();
            }
            objet.setAttribute("Id", ObjetId);
            objet.setAttribute("Nom", ObjetNom);
            objet.setAttribute("Type", ObjetType);
            objet.setAttribute("PositionX", ObjetPosX);
            objet.setAttribute("PositionY", ObjetPosY);
            objet.setAttribute("SizeX", ObjetWidth);
            objet.setAttribute("SizeY", Objetheight);
            objet.setAttribute("Brush", ObjetBrush);
            objet.setAttribute("Pen", ObjetPen);
            objet.setAttribute("Forme", ObjetForme);
            objet.setAttribute("Commentaire", ObjetComment);
            objet.setAttribute("Texte", ObjetTexte);
            objet.setAttribute("Chemin", ObjetChemin);
            objet.setAttribute("Mode", ObjetMode);
            objet.setAttribute("Etat", ObjetEtat);
            objet.setAttribute("TypeLine", ObjetTypeLine);
            objet.setAttribute("WidthLine", ObjetWidthLine);
            objet.setAttribute("ZValue", ObjetZValue);
            objet.setAttribute("Uuid", ObjetUuid);
            objet.setAttribute("FichierDetail", ObjetFichierDetail);

            objets.appendChild(objet);
        }
    }
    // Ecriture dans le fichier


    QString fileName = getFileNameXML();
    // backup du fichier avant modifications
    QString baseName = fileName.split(".", QString::SkipEmptyParts).at(0);
    baseName = baseName + ".bak";
    if (QFile::exists(baseName))
    {
        QFile::remove(baseName);
    }
    QFile::copy(fileName, baseName);

    //enregistrement dans un fichier
    if (fileName.isEmpty())
    {
        return;
    }
    QSettings settings;
    QFile     file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ERREUR D'ÉCRITURE");
        return;
    }
    else
    {
        settings.setValue("lastfile", fileName); // enregistre dans fichier .conf
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
        qDebug() << "enregistrement terminé ";
        ui->statusBar->showMessage("enregistrement terminé : " + fileName, 5000);
        ouvrir_FichierXML(fileName);
    }
}

void MainWindow::on_actionEnregistrer_sous_triggered()
{
    QString cellvalue;
    QString champ;

    // XML
    QDomDocument document;
    QDomNode     xmlNode = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

    document.insertBefore(xmlNode, document.firstChild());
    QDomElement root = document.createElement("root");
    document.appendChild(root);
    QDomElement base = document.createElement("base");
    root.appendChild(base);
    QDomElement background = document.createElement("background");
    root.appendChild(background);
    QDomElement objets = document.createElement("objets");
    root.appendChild(objets);
    QDomElement fichier_base = document.createElement("fichier_base");
    cellvalue = getfileNameSQL();
    champ     = "fichier";
    fichier_base.setAttribute(champ, cellvalue);
    base.appendChild(fichier_base);
    //background
    QDomElement fond = document.createElement("fond");

    fond.setAttribute("chemin", getFileNameBackGround());
    fond.setAttribute("Type", "7");
    fond.setAttribute("PositionX", "0");
    fond.setAttribute("PositionY", "0");
    background.appendChild(fond);

//Ecriture des objets
    QString ObjetId;
    QString ObjetNom;
    QString ObjetType;
    QString ObjetForme;
    QString ObjetPosX;
    QString ObjetPosY;
    QString ObjetWidth;
    QString Objetheight;
    QString ObjetBrush;
    QString ObjetPen;
    QString ObjetComment;
    QString ObjetTexte;
    QString ObjetChemin;
    QString ObjetEtat;
    QString ObjetMode;
    QString ObjetTypeLine;
    QString ObjetWidthLine;
    QString ObjetZValue;
    QString ObjetUuid;
    QString ObjetFichierDetail = "";

    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->pos().x() > 0)
        {
            QDomElement objet = document.createElement("Objet");

            if (itemList[i]->type() == 65536)   // MyItem
            {
                MyItem *item     = dynamic_cast <MyItem *> (itemList[i]);
                QPointF location = itemList[i]->pos();
                int     valX     = int(location.x());
                int     valY     = int(location.y());
                int     sizeW    = int(itemList[i]->boundingRect().width());
                int     sizeH    = int(itemList[i]->boundingRect().height());
                QColor  clr      = item->getColor();
                QColor  clrPen   = item->getPenColor();

                ObjetId            = QString::number(item->getId());
                ObjetNom           = item->getNom();
                ObjetType          = QString::number(65536);
                ObjetForme         = QString::number(item->getTypeShape());
                ObjetPosX          = QString::number(valX);
                ObjetPosY          = QString::number(valY);
                ObjetWidth         = QString::number(sizeW);
                Objetheight        = QString::number(sizeH);
                ObjetBrush         = QString::number(clr.rgb());
                ObjetPen           = QString::number(clrPen.rgb());
                ObjetComment       = item->getComment();
                ObjetTexte         = item->getTexte();
                ObjetChemin        = item->getPixFile();
                ObjetEtat          = QString::number(item->getEtat());
                ObjetMode          = QString::number(item->getMode());
                ObjetTypeLine      = QString::number(item->getTypeLine());
                ObjetWidthLine     = QString::number(item->getWidthLine());
                ObjetZValue        = QString::number(item->zValue());
                ObjetUuid          = item->getUuid().toString();
                ObjetFichierDetail = item->getDetailFile();
            }
            if (itemList[i]->type() == 65537)   // MyPolygone
            {
                MyPolygone *item     = dynamic_cast <MyPolygone *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjetId            = QString::number(item->getId());
                ObjetNom           = item->getNom();
                ObjetType          = QString::number(65537);
                ObjetForme         = QString::number(item->getTypeShape());
                ObjetPosX          = QString::number(valX);
                ObjetPosY          = QString::number(valY);
                ObjetWidth         = QString::number(sizeW);
                Objetheight        = QString::number(sizeH);
                ObjetBrush         = QString::number(clr.rgb());
                ObjetPen           = QString::number(clrPen.rgb());
                ObjetComment       = item->getComment();
                ObjetTexte         = item->getTexte();
                ObjetChemin        = item->getStrPoly();
                ObjetEtat          = QString::number(item->getEtat());
                ObjetMode          = QString::number(item->getMode());
                ObjetTypeLine      = QString::number(item->getTypeLine());
                ObjetWidthLine     = QString::number(item->getWidthLine());
                ObjetZValue        = QString::number(item->zValue());
                ObjetUuid          = item->getUuid().toString();
                ObjetFichierDetail = item->getDetailFile();
            }
            if (itemList[i]->type() == 65539)   // MyPolyline
            {
                MyPolyline *item     = dynamic_cast <MyPolyline *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjetId        = QString::number(item->getId());
                ObjetNom       = item->getNom();
                ObjetType      = QString::number(65539);
                ObjetForme     = QString::number(item->getTypeShape());
                ObjetPosX      = QString::number(valX);
                ObjetPosY      = QString::number(valY);
                ObjetWidth     = QString::number(sizeW);
                Objetheight    = QString::number(sizeH);
                ObjetBrush     = QString::number(clr.rgb());
                ObjetPen       = QString::number(clrPen.rgb());
                ObjetComment   = item->getComment();
                ObjetTexte     = item->getTexte();
                ObjetChemin    = item->getStrPoly();
                ObjetEtat      = QString::number(item->getEtat());
                ObjetMode      = QString::number(item->getMode());
                ObjetTypeLine  = QString::number(item->getTypeLine());
                ObjetWidthLine = QString::number(item->getWidthLine());
                ObjetZValue    = QString::number(item->zValue());
                ObjetUuid      = item->getUuid().toString();
            }
            objet.setAttribute("Id", ObjetId);
            objet.setAttribute("Nom", ObjetNom);
            objet.setAttribute("Type", ObjetType);
            objet.setAttribute("PositionX", ObjetPosX);
            objet.setAttribute("PositionY", ObjetPosY);
            objet.setAttribute("SizeX", ObjetWidth);
            objet.setAttribute("SizeY", Objetheight);
            objet.setAttribute("Brush", ObjetBrush);
            objet.setAttribute("Pen", ObjetPen);
            objet.setAttribute("Forme", ObjetForme);
            objet.setAttribute("Commentaire", ObjetComment);
            objet.setAttribute("Texte", ObjetTexte);
            objet.setAttribute("Chemin", ObjetChemin);
            objet.setAttribute("Mode", ObjetMode);
            objet.setAttribute("Etat", ObjetEtat);
            objet.setAttribute("TypeLine", ObjetTypeLine);
            objet.setAttribute("WidthLine", ObjetWidthLine);
            objet.setAttribute("ZValue", ObjetZValue);
            objet.setAttribute("Uuid", ObjetUuid);
            objet.setAttribute("FichierDetail", ObjetFichierDetail);

            objets.appendChild(objet);
        }
    }
    // Ecriture dans le fichier

    QFileInfo FichierXML(getFileNameXML());
    QDir      repertoireXML = FichierXML.absoluteDir();
    QString   repertoire    = repertoireXML.absolutePath();
    QString   fileName      =
        QFileDialog::getSaveFileName(this, tr("Sauvegarde du plan sous ..."),
                                     repertoire,
                                     tr("XML Files (*.xml)"));
    if (fileName.isEmpty())
    {
        return;
    }

    QSettings settings;
    QFile     file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ERREUR D'ÉCRITURE");
        return;
    }
    else
    {
        settings.setValue("lastfile", fileName); // enregistre dans fichier .conf
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
        qDebug() << "enregistrement terminé ";
        ui->statusBar->showMessage("enregistrement terminé : " + fileName, 5000);
    }
}

/*************************************************************************************************/
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

// **********************GRILLE **************************//

void MainWindow::dessine_grille(const int espace, const qreal&Opacite)
{
    if (getGrille() == 0)
    {
        int space = espace; //espacement de la grille
        //définition du nombre de carrés selon la taille de la scene
        int pasH = int((scene->sceneRect().width() / space)) + 1;
        int pasV = int((scene->sceneRect().height() / space)) + 1;
        for (int h = 1; h < pasH; h++)
        {
            for (int v = 1; v < pasV; v++)
            {
                mygrilleitem *case1 = new mygrilleitem(space, space);
                case1->setColor(QColor(Qt::white));
                case1->setPenColor(QColor(Qt::blue));
                case1->setPos((h * space) - (space / 2), (v * space) - (space / 2));
                case1->setOpacity(Opacite);
                scene->addItem(case1);
            }
        }
    }
}

void MainWindow::on_comboBox_AnneeEnCours_currentIndexChanged(const QString&arg1)
{   //le changement de l'année du combobox entraine l'affichage du planning calé au 1er janvier
    QString strDate       = "01/01/" + arg1;
    QDate   date          = QDate::fromString(strDate, "dd/MM/yyyy");
    int     jour          = date.dayOfWeek();
    QString strDateFev    = "01/02/" + arg1;
    QDate   dateFev       = QDate::fromString(strDateFev, "dd/MM/yyyy");
    int     NbJourFevrier = dateFev.daysInMonth();

    affiche_planning(jour, NbJourFevrier);
}

void MainWindow::on_comboBox_AnneeEnCours_2_currentIndexChanged(const QString&arg1)
{   //le changement de l'année du combobox cale le tableau des rotations sur l'année sélectionnée
    QString strDate = "01/01/" + arg1;
    QDate   date    = QDate::fromString(strDate, "dd/MM/yyyy");
    int     an      = date.year();

    affiche_rotation(an);
}

void MainWindow::on_pushButton_color_clicked()
{   //actualiser rotation
    QString strDate = "01/01/" + ui->comboBox_AnneeEnCours_2->currentText();
    QDate   date    = QDate::fromString(strDate, "dd/MM/yyyy");
    int     an      = date.year();

    affiche_rotation(an);
}

/********************************************************************************/
/************************  dessin du tableau rotation  **************************/
/********************************************************************************/
void MainWindow::affiche_rotation(int year)
{
    QTranslator translator;
    QString     fichier = ":/translations/open-jardin_" + util::getLocale();

    translator.load(fichier);
    qApp->installTranslator(&translator);

    scene_rotation->clear();
    scene_rotation3->clear();
    // DESSIN DE LA GRILLE DU TABLEAU ROTATION
    int spaceCase = 14; //espacement de la grille
    //définition du nombre de carrés selon la taille de la scene
    int pasHCase = int((scene_rotation->sceneRect().width() / spaceCase)) + 1;
    int pasVCase = int((scene_rotation->sceneRect().height() / spaceCase)) + 1;
    //remplissage des cases
    for (int h = 1; h < pasHCase; h++)
    {
        for (int v = 1; v < pasVCase; v++)
        {
            mygrilleitem *caseP = new mygrilleitem(spaceCase, spaceCase);
            caseP->setColor(QColor(Qt::white));
            caseP->setPenColor(QColor(Qt::blue));
            caseP->setPos((h * spaceCase) - (spaceCase / 2), (v * spaceCase) - (spaceCase / 2));
            scene_rotation->addItem(caseP);
        }
    }

    int decalage = 0; //98
    int mois     = 1;
    for (int i = 1; i < 73; i++)
    {
        if (mois == 1)
        {
            ajouter_vignette_haut_rotation(tr("Janvier"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
        }
        if (mois == 2)
        {
            ajouter_vignette_haut_rotation(tr("Février"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
        }
        if (mois == 3)
        {
            ajouter_vignette_haut_rotation(tr("Mars"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
        }
        if (mois == 4)
        {
            ajouter_vignette_haut_rotation(tr("Avril"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
        }
        if (mois == 5)
        {
            ajouter_vignette_haut_rotation(tr("Mai"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
        }
        if (mois == 6)
        {
            ajouter_vignette_haut_rotation(tr("Juin"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
        }
        if (mois == 7)
        {
            ajouter_vignette_haut_rotation(tr("Juillet"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
        }
        if (mois == 8)
        {
            ajouter_vignette_haut_rotation(tr("Aout"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
        }
        if (mois == 9)
        {
            ajouter_vignette_haut_rotation(tr("Septembre"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
        }
        if (mois == 10)
        {
            ajouter_vignette_haut_rotation(tr("Octobre"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
        }
        if (mois == 11)
        {
            ajouter_vignette_haut_rotation(tr("Novembre"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
        }
        if (mois == 12)
        {
            ajouter_vignette_haut_rotation(tr("Décembre"), 2, decalage, 5, Qt::lightGray);
            decalage = decalage + 5;
            mois     = 0;
        }
        mois++;
    }

    decalage = 0;
    for (int i = 1; i < 7; i++)
    {
        ajouter_vignette_haut_rotation(QString::number(year - 1 + i), 1, decalage, 60, Qt::yellow);
        decalage = decalage + 60;
    }

    // etiquettes horinzontales des parcelles
    int spaceE = 28; //espacement de la grille
    // ajouter_vignette_horizontale("PARCELLES",1*spaceE,97,spaceE/2,Qt::white);
    scene_rotation2->clear();
    QString nom_parcelle      = "";
    int     init_pos_vignette = 1;
    for (int h = 0; h < ui->tableWidget_parcelles->rowCount(); h++)
    {
        if (ui->tableWidget_parcelles->rowCount() > -1)
        {
            nom_parcelle = ui->tableWidget_parcelles->item(h, 1)->text();

            // ajouter_vignette_horiz_rotation(nom_parcelle, ((init_pos_vignette + h) * spaceE), 297, spaceE, Qt::lightGray);
            ajouter_vignette_horiz_rotation(nom_parcelle, ((init_pos_vignette + h) * spaceE), 297, spaceE,
                                            QColor(255, 255, 127, 100));
        }
    }

    //positionner les scrollbar sur le coin haut gauche
    ui->graphicsView_rotation->horizontalScrollBar()->setSliderPosition(1);
    ui->graphicsView_rotation2->horizontalScrollBar()->setSliderPosition(1);
    ui->graphicsView_rotation3->verticalScrollBar()->setSliderPosition(1);
    /*************MISE A JOUR AFFICHAGE DES CULTURES SUR LA ROTATION**************/
    mise_a_jour_table_cultures(1); // preparation affichage cultures
}

/****************************************************************/
/*********************etat d'affichage dans le planning**********/
/****************************************************************/
void MainWindow::on_comboBox_Etat_currentIndexChanged(int index)
{   // affichage ou non de la parcelle dans le planning
    Q_UNUSED(index)
    if (ui->comboBox_Etat->currentIndex() != 0)
    {
        int etat = 1;
        QList <QGraphicsItem *> itemList = scene->items();
        if (ui->comboBox_Etat->currentIndex() == 2)
        {
            etat = 0;
        }
        if (ui->comboBox_Etat->currentIndex() == 1)
        {
            etat = 1;
        }
        for (int i = 0; i < itemList.size(); i++)
        {
            if (itemList[i]->isSelected())
            {
                if (itemList[i]->type() == 65536) // MyItem
                {
                    MyItem *item = dynamic_cast <MyItem *> (itemList[i]);
                    item->setEtat(etat);
                    QString designation = item->getNom();
                    QString id          = QString::number(item->getId());
                    if (etat == 1)
                    {
                        ajouter_parcelle(id, designation);
                    }
                    else
                    {
                        supprimer_parcelle(id);
                    }
                }
                if (itemList[i]->type() == 65537)  // MyPolygone
                {
                    MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                    item->setEtat(etat);
                    QString designation = item->getNom();
                    QString id          = QString::number(item->getId());
                    if (etat == 1)
                    {
                        ajouter_parcelle(id, designation);
                    }
                    else
                    {
                        supprimer_parcelle(id);
                    }
                }
            }
        }
        ui->pushButton_recorPlan->show();

        if (etat == 1)
        {
            ui->toolButton_etatPlanning->setIcon(QIcon(":/images/planner_Valider.png"));
        }
        else
        {
            ui->toolButton_etatPlanning->setIcon(QIcon(":/images/planner_hide.png"));
        }
    }
    ui->comboBox_Etat->setCurrentIndex(0);
}

/*********************************************************************************/
/************************  dessin du tableau planning  ***************************/
/*********************************************************************************/
void MainWindow::affiche_planning(int day, int bis)
{
    QTranslator translator;
    QString     fichier = ":/translations/open-jardin_" + util::getLocale();

    translator.load(fichier);
    qApp->installTranslator(&translator);

    //affiche le planning en démarrant au premier jour de la semaine selon la valeur de "day"
    //en fonction de l'année
    scene_planning->clear();
    scene_planning3->clear();
    QStringList jours_semaine;
    jours_semaine << tr("L") << tr("M") << tr("M.") << tr("J") << tr("V") << tr("S") << tr("D") << tr("L") << tr("M") <<
        tr("M.") <<
        tr("J") << tr("V") << tr("S") << tr("D");

    // DESSIN DE LA GRILLE DU PLANNING
    int spaceCase = Consts::SPACE_CASE; //espacement de la grille
    //définition du nombre de carrés selon la taille de la scene
    double pasHCase = (scene_planning->sceneRect().width() / spaceCase) + 1;
    double pasVCase = (scene_planning->sceneRect().height() / spaceCase) + 1;
    //remplissage des cases
    for (int h = 1; h < pasHCase; h++)
    {
        for (int v = 1; v < pasVCase; v++)
        {
            mygrilleitem *caseP = new mygrilleitem(spaceCase, spaceCase);
            caseP->setColor(QColor(Qt::white));
            caseP->setPenColor(QColor(Qt::blue));
            caseP->setPos((h * spaceCase) - (spaceCase / 2), (v * spaceCase) - (spaceCase / 2));
            scene_planning->addItem(caseP);
        }
    }
    // etiquettes horinzontales des parcelles
    int spaceE = 28; //espacement de la grille
    // ajouter_vignette_horizontale("PARCELLES",1*spaceE,97,spaceE/2,Qt::white);
    scene_planning2->clear();
    QString nom_parcelle      = "";
    int     init_pos_vignette = 1;
    for (int h = 0; h < ui->tableWidget_parcelles->rowCount(); h++)
    {
        if (ui->tableWidget_parcelles->rowCount() > -1)
        {
            nom_parcelle = ui->tableWidget_parcelles->item(h, 1)->text();
            ajouter_vignette_horizontale(nom_parcelle, ((init_pos_vignette + h) * spaceE), 297, spaceE,
                                         QColor(255, 255, 127, 100));
        }
    }
    // remplissage des étiquettes semaines
    QString texte_date   = tr("semaine ");
    int     WidthSemaine = spaceCase * 7;
    // int     HeightSemaine = 35;
    int    HeightSemaine = 35;
    int    PosHSemaine   = 20;
    int    PosJour       = 37;
    double NbSemaines    = ((scene_planning3->sceneRect().width() - 100) / WidthSemaine) + 1;
    int    jour          = 1;                       //lundi
    int    decal         = 101 - ((day - 1) * 14);  //décalage des étiquettes semaines
    // ajout  semaine 52 année précédente
    ajouter_vignette_semaine("S " + QString::number(
                                 52), WidthSemaine + 4, PosHSemaine, WidthSemaine, HeightSemaine - 14, Qt::white);
    for (int i = 1; i < 8; i++)
    {
        ajouter_vignette_jour("", ((i - 1) * spaceCase) + 4, PosJour, spaceCase, spaceCase, Qt::lightGray);
    }
    for (int v = 2; v < NbSemaines + 2; v++)
    {
        QString titre = texte_date + QString::number(v - 1);
        ajouter_vignette_semaine(titre, (v * WidthSemaine - ((day - 1) * 14)) + 3, PosHSemaine, WidthSemaine, HeightSemaine - 14,
                                 Qt::white);
        for (int i = jour; i < 8; i++)
        {
            QString Jour = jours_semaine[i + -1];
            ajouter_vignette_jour(Jour, decal + ((i - 1) * spaceCase), PosJour, spaceCase, spaceCase, Qt::yellow);
        }
        decal = decal + (WidthSemaine);
    }
    // Etiquettes mois
    //définition du nombre de carrés selon la taille de la scene
    QStringList Listmois;
    Listmois << tr("Janvier") << tr("Février") << tr("Mars") << tr("Avril") << tr("Mai") << tr("Juin") << tr("Juillet") << tr(
        "Aout") << tr("Septembre") <<
        tr("Octobre") << tr("Novembre") << tr("Décembre") << tr("Janvier");
    QList <int> Jours_mois = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31 };
    if (bis == 29)
    {
        Jours_mois[1] = 29;
    }

    double pasHM       = (scene_planning3->sceneRect().width() / spaceCase) + 1;
    int    drap2       = 1;
    int    saut        = 1;
    int    mois        = 0;
    int    decalage    = 100; //98
    int    jour_depart = 1;
    //ajout décembre année précédente
    ajouter_vignette_mois(tr("Décembre"), 3, 8, Qt::lightGray);
    for (int i = 24; i < 31; i++)
    {
        ajouter_vignette_jour(QString::number(
                                  i + 1), ((i - 24) * spaceCase) + 4, HeightSemaine + spaceCase, spaceCase, spaceCase,
                              Qt::lightGray);
    }
    for (int v = 1; v < pasHM; v++)
    {
        if (v > 5)
        {
            if (drap2 == 0)
            {
                ajouter_vignette_mois(Listmois[mois], (decalage), Jours_mois[mois], Qt::yellow);
                for (int i = jour_depart - 1; i < Jours_mois[mois]; i++)
                {
                    ajouter_vignette_jour(QString::number(
                                              i + 1), decalage + (i * spaceCase) + 1, HeightSemaine + spaceCase, spaceCase, spaceCase,
                                          Qt::yellow);
                }
                decalage = decalage + (Jours_mois[mois] * spaceCase);

                if (mois == 12)
                {
                    mois = 0;
                }
                else
                {
                    mois++;
                }
                drap2 = 1;
                saut  = 1;
            }
            else
            {
                if (saut < 7)
                {
                    saut++;
                }
                else
                {
                    drap2 = 0;
                }
            }
        }
    }

    //positionner les scrollbar sur le coin haut gauche
    ui->graphicsView_planning->horizontalScrollBar()->setSliderPosition(1);
    ui->graphicsView_Planning2->horizontalScrollBar()->setSliderPosition(1);
    ui->graphicsView_planning3->verticalScrollBar()->setSliderPosition(1);
    /*************MISE A JOUR AFFICHAGE DES CULTURES SUR LE PLANNING**************/
    mise_a_jour_table_cultures(0); // preparation affichage cultures
    /****************afficher repére jour courant*******************************/
    QString strDate     = ui->comboBox_AnneeEnCours->currentText() + ".01.01";
    QDate   dateR       = QDate::fromString(strDate, "yyyy.MM.dd");
    QDate   currentDate = QDate::currentDate();


    ui->dateEdit_du_jour->setDate(currentDate);

    int Nbjour = int(dateR.daysTo(currentDate) * 14);
    ajouter_repere_jour("", Nbjour + 105, 1, 1, int( scene_planning->sceneRect().height()), Qt::red); // ajout de 105 positionner sur jour
    QPointF date = ui->graphicsView_planning->mapFromScene(QPointF(Nbjour * 14, 0));
    ui->graphicsView_planning->centerOn(date);
    //calage de la scrollbar horizontale du planning sur la date du jour
    qint64 Posjour = dateR.daysTo(currentDate);
    ui->graphicsView_planning->horizontalScrollBar()->setValue(int(5200 / 365 * (Posjour + 20)));
    ui->graphicsView_planning->verticalScrollBar()->setValue(0);
    ui->graphicsView_Planning2->verticalScrollBar()->setValue(0);
}

void MainWindow::on_pushButton_maj_planning_clicked()
{   // BOUTON DE MISE À JOUR DU PLANNING
    QString strDate       = "01/01/" + ui->comboBox_AnneeEnCours->currentText();
    QDate   date          = QDate::fromString(strDate, "dd/MM/yyyy");
    int     jour          = date.dayOfWeek();
    QString strDateFev    = "01/02/" + ui->comboBox_AnneeEnCours->currentText();
    QDate   dateFev       = QDate::fromString(strDateFev, "dd/MM/yyyy");
    int     NbJourFevrier = dateFev.daysInMonth();

    affiche_planning(jour, NbJourFevrier);
}

/*******************************************************************************/
/*********************SAUVEGARDE DU TABLEAU DANS UN FICHIER XML*****************/
/*******************************************************************************/
void MainWindow::on_pushButton_recorPlan_clicked()
{
    on_actionSauver_triggered();
    ui->pushButton_recorPlan->hide();
}

/**********************************************************/
// redimensionne planning & rotation ,selon le nombre de parcelles

void MainWindow::resize_planning(int nb_parcelles)
{
    int hauteur = ((nb_parcelles) * 28);

    if (hauteur < 1200)
    {
        hauteur = 1200;
    }

    scene_planning->setSceneRect(0, 0, 5205, hauteur);
    scene_planning2->setSceneRect(0, 0, 100, hauteur);
    scene_rotation->setSceneRect(0, 0, 5045, hauteur);
    scene_rotation2->setSceneRect(0, 0, 100, hauteur);
// qDebug() << "nb parcelles"<<nb_parcelles<<" hauteur :"<<hauteur;
// actualisation du planning et rotation

    QString strDate       = "01/01/" + ui->comboBox_AnneeEnCours->currentText();
    QDate   date          = QDate::fromString(strDate, "dd/MM/yyyy");
    int     jour          = date.dayOfWeek();
    QString strDateFev    = "01/02/" + ui->comboBox_AnneeEnCours->currentText();
    QDate   dateFev       = QDate::fromString(strDateFev, "dd/MM/yyyy");
    int     NbJourFevrier = dateFev.daysInMonth();

    affiche_planning(jour, NbJourFevrier);
    affiche_rotation(ui->comboBox_AnneeEnCours_2->currentText().toInt());
}

/************************************************************/
//VARIANTE D'OBJET CREE PixmapItem
void PixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    mousePressPt.setX(int(pos().x() - event->scenePos().x()));
    mousePressPt.setY(int(pos().y() - event->scenePos().y()));
    setSelected(true);
}

void PixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setX(mousePressPt.x() + event->scenePos().x());
    setY(mousePressPt.y() + event->scenePos().y());
    event->accept();
}

void PixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    setX(mousePressPt.x() + event->scenePos().x());
    setY(mousePressPt.y() + event->scenePos().y());
    event->accept();
}

void PixmapItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    //qDebug() << "hover event mouse";
    setToolTip(getComment());
    event->accept();
}

QString PixmapItem::getPixFile()
{
    return pixmapfileName;
}

void PixmapItem::setPixFile(QString file)
{
    pixmapfileName = file;
}

int PixmapItem::getTypeAction()
{
    return typeAction;
}

void PixmapItem::setTypeAction(int typeAct)
{
    typeAction = typeAct;
}

QString PixmapItem::getAction()
{
    return action;
}

void PixmapItem::setAction(QString act)
{
    action = act;
}

void PixmapItem::setTexte(const QString text)
{
    itemText = text;
}

QString PixmapItem::getTexte()
{
    return itemText;
}

void PixmapItem::setComment(const QString comment)
{
    itemComment = comment;
}

QString PixmapItem::getComment()
{
    return itemComment;
}

/**********************************************************/
//VARIANTE D'OBJET CREE FondItem

QString FondItem::getPixFile()
{
    return pixmapfileName;
}

void FondItem::setPixFile(QString file)
{
    pixmapfileName = file;
}

/****************************ACTIONS AJOUTS D'OBJET***************************************/

void MainWindow::on_actionAjouterRectangle_triggered()
{   //parcelle
    MyItem *item = new MyItem(50, 50);

    item->setPenColor(QColor(ui->label_couleurPolyline_Pen->text()));
    item->setColor(QColor(ui->label_couleurPolyline_P_background->text()));
    item->setWidthLine(ui->comboBox_epaisseurLignes_P->currentText().toInt());
    item->setTypeLine(ui->comboBox_typeLigne_P->currentIndex() + 1);
    item->setOpac(0.8);       //opacité de l'item
    item->setId(get_MaxId()); // valeur id en dernière position dans la table
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(sizeW / 2, sizeH / 2);
    item->setTexte("Parcelle");
    item->setNom("Parcelle " + QString::number(get_MaxId()));
    item->setEtat(1);
    item->setMode(0);
    item->setTypeShape(MyItem::Rectangle);
    scene->addItem(item);
    QPointF zero = ui->graphicsView->mapFromScene(QPointF(0, 0));
    ui->graphicsView->horizontalScrollBar()->setValue(int(zero.x()));
    ui->graphicsView->verticalScrollBar()->setValue(int(zero.y()));
    QList <QGraphicsItem *> itemList = scene->items();
    //  items_vers_listeparcelles();
    ajouter_parcelle(QString::number(item->getId()), item->getNom());
    ui->pushButton_recorPlan->show();
}

void MainWindow::on_actionAjouter_Rectangle_arrondi_triggered()
{
    MyItem *item = new MyItem(50, 50);

    item->setPenColor(QColor(ui->label_couleurPolyline_Pen->text()));
    item->setColor(QColor(ui->label_couleurPolyline_P_background->text()));
    item->setWidthLine(ui->comboBox_epaisseurLignes_P->currentText().toInt());
    item->setTypeLine(ui->comboBox_typeLigne_P->currentIndex() + 1);
    item->setId(get_MaxId());
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(sizeW / 2, sizeH / 2);
    item->setNom("Parcelle " + QString::number(get_MaxId()));
    item->setTexte(" ");
    item->setEtat(1);
    item->setMode(0);
    item->setTypeShape(MyItem::RoundedRectangle);
    scene->addItem(item);
    QPointF zero = ui->graphicsView->mapFromScene(QPointF(0, 0));
    ui->graphicsView->horizontalScrollBar()->setValue(int(zero.x()));
    ui->graphicsView->verticalScrollBar()->setValue(int(zero.y()));
    QList <QGraphicsItem *> itemList = scene->items();
    //  items_vers_listeparcelles();
    ajouter_parcelle(QString::number(item->getId()), item->getNom());
    ui->pushButton_recorPlan->show();
}

void MainWindow::on_actionAjouterCercle_triggered()
{
    MyItem *item = new MyItem(50, 50);

    item->setPenColor(QColor(ui->label_couleurPolyline_Pen->text()));
    item->setColor(QColor(ui->label_couleurPolyline_P_background->text()));
    item->setWidthLine(ui->comboBox_epaisseurLignes_P->currentText().toInt());
    item->setTypeLine(ui->comboBox_typeLigne_P->currentIndex() + 1);
    item->setOpac(0.8);                               //opacité de l'item
    item->setId(get_MaxId());
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(sizeW / 2, sizeH / 2);
    item->setNom("Plantation " + QString::number(get_MaxId()));
    item->setTexte("");
    item->setEtat(1);
    item->setMode(0);
    item->setTypeShape(MyItem::Circle);
    scene->addItem(item);
    QPointF zero = ui->graphicsView->mapFromScene(QPointF(0, 0));
    ui->graphicsView->horizontalScrollBar()->setValue(int(zero.x()));
    ui->graphicsView->verticalScrollBar()->setValue(int(zero.y()));
    QList <QGraphicsItem *> itemList = scene->items();
    // items_vers_listeparcelles();
    ajouter_parcelle(QString::number(item->getId()), item->getNom());
    ui->pushButton_recorPlan->show();
}

void MainWindow::on_actionAjouterImage_triggered()
{
    //utilisation de myItem avec painter drawImage
    int     ObjetPosX = 10;
    int     ObjetPosY = 10;
    QString fileName  =
        QFileDialog::getOpenFileName(this, tr("Ouverture du fichier image"),
                                     QDir::currentPath(),
                                     tr("Image Files (*.png *.jpg *.bmp)"));

    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        MyItem *item = new MyItem(32, 32);
        item->setId(get_MaxId());
        item->setPixFile(fileName);
        item->setPos(ObjetPosX, ObjetPosY);
        item->setTexte(" ");
        item->setEtat(1);
        item->setMode(0);
        item->setTypeShape(MyItem::Image);
        scene->addItem(item);
    }
    // items_vers_listeparcelles();

    ui->pushButton_recorPlan->show();
}

/**************************************item rotation*********************************/
void MainWindow::ajouter_vignette_horiz_rotation(QString titre, int ligne, int width, int height, QColor couleur)
{
    vignette *item2 = new vignette(width, height);

    item2->setColor(couleur);
    item2->setPenColor(QColor(Qt::blue));
    item2->setOpac(1.0); //opacité de l'item
    double sizeW2 = item2->boundingRect().width();
    double sizeH2 = item2->boundingRect().height();
    item2->setPos(sizeW2 / 2, ligne - (sizeH2 / 2));
    item2->setNom(titre);
    item2->setEtat(1);
    item2->setTypeShape(vignette::Rectangle);
    item2->setMode(1);               //mode utilisation
    scene_rotation2->addItem(item2); // bandeau parcelles
}

void MainWindow::ajouter_vignette_rotation(int culture, QString titre, int ligne, int colonne, int nbCases, QColor couleur,
                                           QString etat_affichage)
{   //titre,nb lignes,colonne de depart,nb cases,couleur
    int spaceCase   = 14;
    int hauteurCase = 28;
    int lg          = (ligne) * (spaceCase * 2);
    int col         = (colonne) * spaceCase;

    if (etat_affichage == "2" || etat_affichage == "3")
    {
        hauteurCase = 14;
    }
    MyItem *item = new MyItem(nbCases * 14, hauteurCase);

    item->setId(culture);
    item->setColor(couleur);
    item->setPenColor(QColor(Qt::black));
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    if (etat_affichage == "2")
    {
        item->setPos(col + (sizeW / 2) - 1, lg - (3 * (sizeH / 2)) - 1);
    }
    else
    {
        item->setPos(col + (sizeW / 2) - 1, lg - (sizeH / 2) - 1);
    }
    item->setNom(titre);
    item->setEtat(1);
    item->setMode(1);              //déplacements imossibles
    item->setTypeShape(MyItem::Rectangle);
    scene_rotation->addItem(item); //bandeau central
}

void MainWindow::ajouter_vignette_haut_rotation(QString titre, int ligne, int colonne, int nbCases, QColor couleur)
{                           // années et mois
    int     spaceCase = 14; //14
    int     lg        = (ligne) * (spaceCase * 2);
    int     col       = (colonne) * spaceCase;
    MyItem *item      = new MyItem(nbCases * 14, 28);

    item->setColor(couleur);
    item->setPenColor(QColor(Qt::black));
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(col + (sizeW / 2) + 2, (lg - (sizeH / 2)));
    item->setNom(titre);
    item->setEtat(1);
    item->setMode(2);               //déplacements possibles
    item->setTypeShape(MyItem::Rectangle);
    scene_rotation3->addItem(item); // bandeau haut horizontal
}

void MainWindow::ajouter_repere_haut_rotation(QString titre, int ligne, int colonne, int nbCases, QColor couleur)
{                       // années et mois
    int spaceCase = 28; //14
    int lg        = (ligne) * (spaceCase * 2);

    int col = (colonne) * spaceCase;

    MyItem *item = new MyItem(nbCases * 2, 56);

    item->setColor(couleur);
    item->setPenColor(QColor(Qt::black));
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(col + (sizeW / 2) + 2, (lg - (sizeH / 2)));
    item->setNom(titre);
    item->setEtat(1);
    item->setMode(2);              //déplacements possibles
    item->setTypeShape(MyItem::Rectangle);
    scene_rotation->addItem(item); // bandeau haut horizontal
}

void MainWindow::ajouter_repere_jour_rotation(QString titre, int colonne, int ligne, int width, int height, QColor couleur)
{   // repére du jour courant
    mygrilleitem *item = new mygrilleitem(width, height);

    item->setColor(couleur);
    item->setPenColor(QColor(Qt::red));
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(colonne + (sizeW / 2), ligne + sizeH / 2);
    item->setText(titre);
    scene_planning->addItem(item);
}

/***************************************item du planning******************************/
void MainWindow::ajouter_vignette_horizontale(QString titre, int ligne, int width, int height, QColor couleur)
{
    vignette *item2 = new vignette(width, height);

    item2->setColor(couleur);
    item2->setPenColor(QColor(Qt::blue));
    item2->setOpac(1.0); //opacité de l'item
    double sizeW2 = item2->boundingRect().width();
    double sizeH2 = item2->boundingRect().height();
    item2->setPos(sizeW2 / 2, ligne - (sizeH2 / 2));
    item2->setNom(titre);
    item2->setEtat(1);
    item2->setTypeShape(vignette::Rectangle);
    item2->setMode(1);//mode utilisation
    scene_planning2->addItem(item2);
}

void MainWindow::ajouter_vignette_verticale(QString titre, int colonne, int width, int height, QColor couleur)
{   // semaines
    planning_item *item = new planning_item(width, height);

    item->setColor(couleur);
    item->setPenColor(QColor(Qt::black));
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(colonne - (sizeW / 2) - 1, 20 + sizeH / 2);
    item->setNom(titre);
    item->setEtat(1);
    item->setTypeShape(MyItem::Rectangle);
    item->setMode(1);//mode utilisation
    scene_planning3->addItem(item);
}

void MainWindow::ajouter_vignette_planning(int culture, QString titre, int ligne, int colonne, int nbCases, QColor couleur,
                                           QString etat_affichage)
{   //titre,nb lignes,colonne de depart,nb cases,couleur
    int spaceCase   = 14;
    int hauteurCase = 28;
    int lg          = (ligne) * (spaceCase * 2);
    int col         = (colonne + 7) * spaceCase;

    if (etat_affichage == "2" || etat_affichage == "3")
    {
        hauteurCase = 14;
    }


    MyItem *item = new MyItem(nbCases * 14, hauteurCase);

    item->setId(culture);
    item->setColor(couleur);
    item->setPenColor(QColor(Qt::black));
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    if (etat_affichage == "2")
    {
        item->setPos(col + (sizeW / 2) - 1, lg - (3 * (sizeH / 2)) - 1);
    }
    else
    {
        item->setPos(col + (sizeW / 2) - 1, lg - (sizeH / 2) - 1);
    }

    item->setNom(titre);
    item->setEtat(1);
    item->setMode(2); //déplacements possibles
    item->setTypeShape(MyItem::Rectangle);
    scene_planning->addItem(item);
}

void MainWindow::ajouter_vignette_semaine(QString titre, int colonne, int ligne, int width, int height, QColor couleur)
{   // semaines
    planning_item *item = new planning_item(width, height);

    item->setColor(couleur);
    item->setPenColor(QColor(Qt::black));
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(colonne - (sizeW / 2 + 4), ligne + sizeH / 2);
    item->setNom(titre);
    item->setEtat(1);
    item->setTypeShape(MyItem::Rectangle);
    item->setMode(1);//mode utilisation
    scene_planning3->addItem(item);
}

void MainWindow::ajouter_vignette_mois(QString titre, int colonne, int nb_jours, QColor couleur)
{
    planning_item *item = new planning_item(nb_jours * 14, 20);

    item->setColor(couleur);
    item->setPenColor(QColor(Qt::black));
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(colonne + (sizeW / 2 - 3), sizeH / 2);
    item->setNom(titre);
    item->setEtat(1);
    item->setTypeShape(MyItem::Rectangle);
    item->setMode(1);//mode utilisation
    scene_planning3->addItem(item);
}

void MainWindow::ajouter_vignette_jour(QString titre, int colonne, int ligne, int width, int height, QColor couleur)
{   // semaines
    mygrilleitem *item = new mygrilleitem(width, height);

    item->setColor(couleur);
    item->setPenColor(QColor(Qt::black));
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(colonne + (sizeW / 2) - 3, ligne + sizeH / 2);
    item->setText(titre);
    scene_planning3->addItem(item);
}

void MainWindow::ajouter_repere_jour(QString titre, int colonne, int ligne, int width, int height, QColor couleur)
{   // repére du jour courant
    mygrilleitem *item = new mygrilleitem(width, height);

    item->setColor(couleur);
    item->setPenColor(QColor(Qt::red));
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(colonne + (sizeW / 2), ligne + sizeH / 2);
    item->setText(titre);
    scene_planning->addItem(item);
}

/************************************************************************************************************/


int MainWindow::get_MaxId()
{   // recherche de la valeur du dernier Id
    int Max = 0;

    QList <QGraphicsItem *> itemList = scene->items();

    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 65536)   // MyItem
        {
            MyItem *item = dynamic_cast <MyItem *> (itemList[i]);
            if (Max < item->getId())
            {
                Max = item->getId();
            }
        }
        if (itemList[i]->type() == 65537)   // MyPolygone
        {
            MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
            if (Max < item->getId())
            {
                Max = item->getId();
            }
        }
        if (itemList[i]->type() == 65539)   // MyPolyline
        {
            MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
            if (Max < item->getId())
            {
                Max = item->getId();
            }
        }
    }


    m_MaxId = Max + 1;
    return m_MaxId;
}

/*******************************FOND D'ECRAN*********************************************/

void MainWindow::on_actionFondEcran_triggered()
{   // choix fond d-ecran
    // ouverture du fichier pour lecture
    QString repertoire = QDir::homePath() + "/openjardin/";

    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Ouverture du fichier des objets"),
                                     repertoire,
                                     tr("Image Files (*.png *.jpg *.bmp)"));

    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        QList <QGraphicsItem *> itemList = scene->items();
        for (int i = 0; i < itemList.size(); i++)
        {
            if (itemList[i]->type() == 7)      // background
            {
                background *item = dynamic_cast <background *> (itemList[i]);
                scene->removeItem(item);
            }
        }
        QPixmap     pim(fileName);
        background *pixmap = new background();
        pixmap->setPixmap(pim);
        pixmap->setZValue(-3);
        scene->addItem(pixmap);
        //dimensionnement de la scene selon la taille de pixmap
        double sizeW = pixmap->boundingRect().width();
        double sizeH = pixmap->boundingRect().height();
        scene->setSceneRect(0, 0, sizeW, sizeH);
        // positionnement des scrollbar en haut et à gauche
        QPointF zero = ui->graphicsView->mapFromScene(QPointF(0, 0));
        ui->graphicsView->horizontalScrollBar()->setValue(int(zero.x()));
        ui->graphicsView->verticalScrollBar()->setValue(int(zero.y()));
        pixmap->setPixFile(fileName);
        setFileNameBackGround(fileName);
        qDebug() << " name background " << getFileNameBackGround();
        ui->pushButton_recorPlan->show();
    }
}

/*******************************CHOIX DE LA COULEUR DE L'OBJET********************************************/


void MainWindow::on_actionChoisir_la_couleur_triggered()
{
    QColorDialog *dialog = new QColorDialog(this);

    dialog->show();
    int resultCode = dialog->exec();
    if (resultCode == QDialog::Accepted)
    {
        QColor   color   = dialog->currentColor();
        QVariant variant = color;
        QString  rgb     = variant.toString();
        ui->label_couleurPolyline_P_background->setText(rgb);
        ui->label_couleurPolyline_P_background->setStyleSheet("QLabel { background-color : " + rgb + "}");
    }

    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                QColor   color   = dialog->currentColor();
                QVariant variant = color;
                QString  rgb     = variant.toString();
                MyItem * item    = dynamic_cast <MyItem *> (itemList[i]);
                item->setColor(color);
            }
            if (itemList[i]->type() == 65537)
            {
                QColor      color   = dialog->currentColor();
                QVariant    variant = color;
                QString     rgb     = variant.toString();
                MyPolygone *item    = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setColor(color);
            }
        }
    }
    //  items_vers_listeparcelles();

    delete dialog;
    ui->pushButton_recorPlan->show();
    scene->update();
}

void MainWindow::on_actionChoisir_le_type_de_crayon_triggered()
{   // choisir la couleur de pen
    QColorDialog *dialog = new QColorDialog(this);

    dialog->show();
    int resultCode = dialog->exec();
    if (resultCode == QDialog::Accepted)
    {
        QColor   color   = dialog->currentColor();
        QVariant variant = color;
        QString  rgb     = variant.toString();
        ui->label_couleurPolyline_Pen->setText(rgb);
        ui->label_couleurPolyline_Pen->setStyleSheet("QLabel { background-color : " + rgb + "}");
    }
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                QColor   color   = dialog->currentColor();
                QVariant variant = color;
                QString  rgb     = variant.toString();
                MyItem * item    = dynamic_cast <MyItem *> (itemList[i]);
                item->setPenColor(color);
            }
            if (itemList[i]->type() == 65537)
            {
                QColor      color   = dialog->currentColor();
                QVariant    variant = color;
                QString     rgb     = variant.toString();
                MyPolygone *item    = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setPenColor(color);
            }
            if (itemList[i]->type() == 65539)
            {
                QColor      color   = dialog->currentColor();
                QVariant    variant = color;
                QString     rgb     = variant.toString();
                MyPolyline *item    = dynamic_cast <MyPolyline *> (itemList[i]);
                item->setPenColor(color);
            }
        }
    }
    //items_vers_listeparcelles();

    delete dialog;
    ui->pushButton_recorPlan->show();
    scene->update();
}

/*******************************AUTRES ACTIONS*********************************************/

void MainWindow::on_actionSupprimer_triggered()
{   //suppression de l'objet
    // l'objet sera supprimé lors du prochain enregistrement du fichier xml si getPosX() < 0
    int ret = QMessageBox::warning(this, tr("Suppression"),
                                   tr("Cet objet peut être supprimer.\n"
                                      "Confirmer la suppression de l'objet ?"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel);

    switch (ret)
    {
    case QMessageBox::Ok:
    {
        QList <QGraphicsItem *> itemList = scene->items();
        for (int i = 0; i < itemList.size(); i++)
        {
            if (itemList[i]->isSelected())
            {
                if (itemList[i]->type() == 65536)  // MyItem
                {
                    MyItem *item = dynamic_cast <MyItem *> (itemList[i]);
                    supprimer_parcelle(QString::number(item->getId()));
                    scene->removeItem(item);
                }
                if (itemList[i]->type() == 65537)  // MyPolygone
                {
                    MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                    supprimer_parcelle(QString::number(item->getId()));
                    scene->removeItem(item);
                }
                if (itemList[i]->type() == 65539)  // MyPolyline
                {
                    MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                    scene->removeItem(item);
                }
            }
        }
        ui->statusBar->showMessage("L'objet a été supprimé ", 5000);
        ui->pushButton_recorPlan->show();
        break;
    }

    case QMessageBox::Cancel:
    {
        break;
    }

    default:
        break;
    }
}

void MainWindow::on_actionDeselectionner_tout_triggered()
{   //deselectionne tous les objets
    QList <QGraphicsItem *> itemList = scene->items();

    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            itemList[i]->setSelected(false);
            if (itemList[i]->type() == 65536) // MyItem
            {
                MyItem *item = dynamic_cast <MyItem *> (itemList[i]);
                item->setPenColor(QColor(Qt::red));
            }
        }
    }
}

QString MainWindow::Choisir_le_fichier()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Ouverture du fichier image"),
                                     QDir::currentPath(),
                                     tr("Image Files (*.png *.jpg *.bmp)"));

    return fileName;
}

/************************BRING TO BACK AND TO FRONT************************/


void MainWindow::on_actionBringToFront_triggered()
{
    if (scene->selectedItems().isEmpty())
    {
        return;
    }

    QGraphicsItem *         selectedItem = scene->selectedItems().first();
    QList <QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;

    foreach(QGraphicsItem * item, overlapItems)
    {
        zValue = item->zValue() + 0.1;
        qDebug() << zValue;
    }
    selectedItem->setZValue(zValue);
}

/**********************affichage fond d'écran***************************/

void MainWindow::on_actionCacher_le_fond_triggered()
{
    QList <QGraphicsItem *> itemList = scene->items();

    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 7) //IMAGES
        {
            FondItem *as_fonditem;
            if ((as_fonditem = dynamic_cast <FondItem *>(itemList[i])))
            {
                itemList[i]->hide();
            }
        }
    }
}

void MainWindow::on_actionAfficher_le_fond_triggered()
{
    QList <QGraphicsItem *> itemList = scene->items();

    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 7) //IMAGES
        {
            FondItem *as_fonditem;
            if ((as_fonditem = dynamic_cast <FondItem *>(itemList[i])))
            {
                itemList[i]->show();
            }
        }
    }
}

/************************************************************************************************/
//******************** GESTION DES MODES CREATION-MODIFICATION DU PLAN ET UTILISATION************/
/************************************************************************************************/

void MainWindow::on_actionModification_plan_triggered()
{
    m_mode = 0;
    MyGraphicsScene *scene2 = dynamic_cast <MyGraphicsScene *> (scene);
    scene2->setMode(MyGraphicsScene::Modification);

//  scene->setMode(MyGraphicsScene::Modification);
    ui->actionUtilisation->setChecked(false);
    ui->actionModification_plan->setChecked(true);
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 65536)  // MyItem
        {
            MyItem *item = dynamic_cast <MyItem *> (itemList[i]);
            item->setMode(0);
        }
        if (itemList[i]->type() == 65537)  // MyPolygone
        {
            MyPolygone *Pitem = dynamic_cast <MyPolygone *> (itemList[i]);
            Pitem->setMode(0);
        }
        if (itemList[i]->type() == 65539)  // MyPolyline
        {
            MyPolyline *Poline = dynamic_cast <MyPolyline *> (itemList[i]);
            Poline->setMode(0);
        }
    }
    ui->actionAjouterRectangle->setEnabled(true);
    ui->actionAjouterCercle->setEnabled(true);
    ui->actionAjouterImage->setEnabled(true);
    ui->actionAjouter_icone->setEnabled(true);
    ui->actionAjouter_Rectangle_arrondi->setEnabled(true);
    ui->actionChoisir_la_couleur->setEnabled(true);
    ui->actionChoisir_le_type_de_crayon->setEnabled(true);
    ui->actionBringToFront->setEnabled(true);
    ui->actionDeselectionner_tout->setEnabled(true);
    ui->actionFondEcran->setEnabled(true);
    ui->actionSelectionner_tout->setEnabled(true);
    ui->actionSupprimer->setEnabled(true);
    ui->actionModifier->setEnabled(true);
    ui->toolButton_modifierPolygon->setEnabled(true);
    ui->toolButton_newPolygon->setEnabled(true);
    ui->toolButton_validerPolygone->setDisabled(true);
    ui->toolButton_ValiderPolyline->setDisabled(true);
    ui->comboBox_epaisseurLignes_P->setEnabled(true);
    ui->comboBox_typeLigne_P->setEnabled(true);
    ui->toolButton_CouleurCrayon->setEnabled(true);
    ui->toolButton_CouleurFond->setEnabled(true);
    ui->frame_ToolsCreate->show();
    statusLabel->setText("MODE MODIFICATION");
}

void MainWindow::on_actionUtilisation_triggered()
{
    m_mode = 1;
    MyGraphicsScene *scene2 = dynamic_cast <MyGraphicsScene *> (scene);
    scene2->setMode(MyGraphicsScene::Utilisation);
    ui->actionUtilisation->setChecked(true);
    ui->actionModification_plan->setChecked(false);
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 65536) // MyItem
        {
            MyItem *item = dynamic_cast <MyItem *> (itemList[i]);
            item->setMode(1);
        }
        if (itemList[i]->type() == 65537) // MyPolygone
        {
            MyPolygone *Pitem = dynamic_cast <MyPolygone *> (itemList[i]);
            Pitem->setMode(1);
        }
        if (itemList[i]->type() == 65539) // MyPolyline
        {
            MyPolyline *Poline = dynamic_cast <MyPolyline *> (itemList[i]);
            Poline->setMode(1);
        }
    }
    ui->actionAjouterRectangle->setDisabled(true);
    ui->actionAjouterCercle->setDisabled(true);
    ui->actionAjouterImage->setDisabled(true);
    ui->actionAjouter_icone->setDisabled(true);
    ui->actionAjouter_Rectangle_arrondi->setDisabled(true);
    ui->actionChoisir_la_couleur->setDisabled(true);
    ui->actionChoisir_le_type_de_crayon->setDisabled(true);
    ui->actionBringToFront->setDisabled(true);
    ui->actionDeselectionner_tout->setDisabled(true);
    ui->actionFondEcran->setDisabled(true);
    ui->actionSelectionner_tout->setDisabled(true);
    ui->actionSupprimer->setDisabled(true);
    ui->actionModifier->setDisabled(true);
    ui->toolButton_modifierPolygon->setDisabled(true);
    ui->toolButton_newPolygon->setDisabled(true);
    ui->toolButton_validerPolygone->setDisabled(true);
    ui->toolButton_ValiderPolyline->setDisabled(true);
    ui->comboBox_epaisseurLignes_P->setDisabled(true);
    ui->comboBox_typeLigne_P->setDisabled(true);
    ui->toolButton_CouleurCrayon->setDisabled(true);
    ui->toolButton_CouleurFond->setDisabled(true);
    ui->frame_ToolsCreate->hide();
    statusLabel->setText("MODE UTILISATION");
}

/********************************************************************************/
/*************************action sur click item**********************************/
/********************************************************************************/

void MainWindow::Item_clicked()
{   //si l'item est sélectionné (selection changed)
    QList <QGraphicsItem *> itemList = scene->items();

    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536) // MyItem
            {
                MyItem *item = dynamic_cast <MyItem *> (itemList[i]);
                ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Nom_item->setText(item->getNom());
                ui->textEdit_plan_commentaires->setText(item->getTexte());
                if (item->getTypeShape() == (MyItem::Rectangle))
                {
                    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                }
                if (item->getTypeShape() == (MyItem::RoundedRectangle))
                {
                    ui->lineEdit_Id_Item->setText(QString::number(item->getId())); //équipement
                }
                if (item->getTypeShape() == (MyItem::Circle))
                {
                    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                }
                int etat = item->getEtat();
                if (etat == 1)
                {
                    ui->toolButton_etatPlanning->setIcon(QIcon(":/images/planner_Valider.png"));
                }
                else
                {
                    ui->toolButton_etatPlanning->setIcon(QIcon(":/images/planner_hide.png"));
                }
                //récupérer la couleur du fond
                QColor   color   = item->getColor();
                QVariant variant = color;
                QString  rgb     = variant.toString();
                ui->label_couleurPolyline_P_background->setText(rgb);
                ui->label_couleurPolyline_P_background->setStyleSheet("QLabel { background-color : " + rgb + "}");
                //récupérer la couleur du trait
                QColor   colorpen   = item->getPenColor();
                QVariant variantpen = colorpen;
                QString  rgbpen     = variantpen.toString();
                ui->label_couleurPolyline_Pen->setText(rgbpen);
                ui->label_couleurPolyline_Pen->setStyleSheet("QLabel { background-color : " + rgbpen + "}");
            }
            if (itemList[i]->type() == 65537) // MyPolygone
            {
                MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Nom_item->setText(item->getNom());
                ui->textEdit_plan_commentaires->setText(item->getTexte());
                int etat = item->getEtat();
                if (etat == 1)
                {
                    ui->toolButton_etatPlanning->setIcon(QIcon(":/images/planner_Valider.png"));
                }
                else
                {
                    ui->toolButton_etatPlanning->setIcon(QIcon(":/images/planner_hide.png"));
                }
                QColor   color   = item->getColor();
                QVariant variant = color;
                QString  rgb     = variant.toString();
                ui->label_couleurPolyline_P_background->setText(rgb);
                ui->label_couleurPolyline_P_background->setStyleSheet("QLabel { background-color : " + rgb + "}");
                QColor   colorpen   = item->getPenColor();
                QVariant variantpen = colorpen;
                QString  rgbpen     = variantpen.toString();
                ui->label_couleurPolyline_Pen->setText(rgbpen);
                ui->label_couleurPolyline_Pen->setStyleSheet("QLabel { background-color : " + rgbpen + "}");
            }

            if (itemList[i]->type() == 65539) // MyPolyline
            {
                MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                //    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Nom_item->setText(item->getNom());
                ui->textEdit_plan_commentaires->setText(item->getTexte());
                QColor   colorpen   = item->getPenColor();
                QVariant variantpen = colorpen;
                QString  rgbpen     = variantpen.toString();
                ui->label_couleurPolyline_Pen->setText(rgbpen);
                ui->label_couleurPolyline_Pen->setStyleSheet("QLabel { background-color : " + rgbpen + "}");
            }
        }
    }
}

void MainWindow::ItemPlanning_clicked()
{   //si l'item est sélectionné (scene_planning selection changed)
    QList <QGraphicsItem *> itemList = scene_planning->items();
    int     nbJours;
    int     dateDepart;
    QString strDateAn = ui->comboBox_AnneeEnCours->currentText() + ".01.01";
    QDate   dateAn    = QDate::fromString(strDateAn, "yyyy.MM.dd");

    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            MyItem *item = dynamic_cast <MyItem *> (itemList[i]);
            nbJours = (item->getWidth() / 14) + 1;
            ui->lineEdit_width->setText(QString::number(nbJours));
            dateDepart = (int(((item->pos().x()) / 14) - nbJours / 2)) - 6;
            QDate date_depart = dateAn.addDays(dateDepart);
            ui->lineEdit_dateDepart->setText(date_depart.toString("yyyy.MM.dd"));
            ui->lineEdit_id_culture->setText(QString::number(item->getId()));
            //  QDate date_fin = date_depart.addDays(nbJours);
            ui->lineEdit_texte_culture->setText(item->getNom());
            //  ui->lineEdit_date_recolte->setText(date_fin.toString("yyyy.MM.dd"));
        }
    }
}

/**********************************************************************/

void MainWindow::on_pushButton_Affiches_fiche_clicked()
{
    int Id = ui->lineEdit_Id_Item->text().toInt();

    if (Id >= 0)
    {
        QString   fileName       = getfileNameSQL();
        Cultures *fiche_Cultures = new Cultures(Id, 0);
        fiche_Cultures->show();
    }
}

void MainWindow::on_lineEdit_Nom_item_textChanged(const QString&arg1)
{
    Q_UNUSED(arg1)
    // on_pushButton_Enregistrer_modif_item_clicked();
    //ui->pushButton_Enregistrer_modif_item->setStyleSheet("background-color : #ffff00");
}

void MainWindow::on_pushButton_Enregistrer_modif_item_clicked()
{
    QList <QGraphicsItem *> itemList = scene->items();

    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536) // MyItem
            {
                MyItem *item = dynamic_cast <MyItem *> (itemList[i]);
                item->setNom(util::apos(ui->lineEdit_Nom_item->text()));
                item->setTexte(util::apos(ui->textEdit_plan_commentaires->document()->toPlainText()));
                ajouter_parcelle(QString::number(item->getId()), item->getNom());
            }
            if (itemList[i]->type() == 65537) // MyPolygone
            {
                MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setNom(util::apos(ui->lineEdit_Nom_item->text()));
                item->setTexte(util::apos(ui->textEdit_plan_commentaires->document()->toPlainText()));
                ajouter_parcelle(QString::number(item->getId()), item->getNom());
            }
            if (itemList[i]->type() == 65539) // MyPolyline
            {
                MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                item->setNom(util::apos(ui->lineEdit_Nom_item->text()));
                item->setTexte(util::apos(ui->textEdit_plan_commentaires->document()->toPlainText()));
            }
        }
    }
    // items_vers_listeparcelles();

    scene->update();
}

void MainWindow::on_pushButton_ficheCulture_clicked()
{
    // ouvre la fiche culture de la culture sélectionnée
    if (ui->lineEdit_id_culture->text().toInt() > 0)
    {
        for (int row = 0; row < ui->tableViewCultures->model()->rowCount(); row++)
        {
            QString id_culture  = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 0)).toString();
            QString id_parcelle = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 2)).toString();

            if (id_culture == ui->lineEdit_id_culture->text())
            {
                int Id   = id_parcelle.toInt();
                int cult = id_culture.toInt();
                if (Id >= 0)
                {
                    QString   fileName         = getfileNameSQL();
                    Cultures *fenetre_database = new Cultures(Id, cult);
                    fenetre_database->show();
                }
            }
        }
    }
}

/*************************************MISE A JOUR DES CULTURES DANS LE PLANNING ET ROTATION*****************************/

void MainWindow::mise_a_jour_table_cultures(int choix)
{   // mise à jour des cultures  pour affichage dans les onglets planning (0)et rotations(1)
    QSqlQueryModel *model5 = new QSqlQueryModel;

    model5->setQuery(
        "SELECT id, designation, parcelle, date_semis, duree,type_plante , commentaires , etat,date_recolte FROM cultures ");
    model5->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    model5->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));
    model5->setHeaderData(3, Qt::Vertical, QObject::tr("parcelle"));
    ui->tableViewCultures->setModel(model5);
    QString strDate  = ui->comboBox_AnneeEnCours->currentText() + ".01.01";
    QDate   date     = QDate::fromString(strDate, "yyyy.MM.dd");
    QString strDateR = ui->comboBox_AnneeEnCours_2->currentText() + ".01.01";
    QDate   dateR    = QDate::fromString(strDateR, "yyyy.MM.dd");



    QString num_parcelle = "";
    for (int h = 0; h < ui->tableWidget_parcelles->rowCount(); h++)
    {
        if (ui->tableWidget_parcelles->rowCount() > 0)
        {
            num_parcelle = ui->tableWidget_parcelles->item(h, 0)->text();
            for (int row = 0; row < ui->tableViewCultures->model()->rowCount(); row++)
            {
                QString str = ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 2)).toString();
                if (str == num_parcelle)
                {
                    ui->tableWidget_taches->setRowCount(ui->tableWidget_taches->rowCount() + 1);
                    int ligne = ui->tableWidget_taches->rowCount() - 1;
                    ui->tableWidget_taches->setItem(ligne, 0,
                                                    new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->
                                                                                                              tableViewCultures->
                                                                                                              model()->index(row,
                                                                                                                             0))
                                                                         .toString()));
                    ui->tableWidget_taches->setItem(ligne, 1,
                                                    new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->
                                                                                                              tableViewCultures->
                                                                                                              model()->index(row,
                                                                                                                             1))
                                                                         .toString()));
                    ui->tableWidget_taches->setItem(ligne, 2,
                                                    new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->
                                                                                                              tableViewCultures->
                                                                                                              model()->index(row,
                                                                                                                             2))
                                                                         .toString()));
                    ui->tableWidget_taches->setItem(ligne, 3,
                                                    new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->
                                                                                                              tableViewCultures->
                                                                                                              model()->index(row,
                                                                                                                             3))
                                                                         .toString()));
                    ui->tableWidget_taches->setItem(ligne, 4,
                                                    new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->
                                                                                                              tableViewCultures->
                                                                                                              model()->index(row,
                                                                                                                             4))
                                                                         .toString()));
                    ui->tableWidget_taches->setItem(ligne, 5,
                                                    new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->
                                                                                                              tableViewCultures->
                                                                                                              model()->index(row,
                                                                                                                             5))
                                                                         .toString()));
                    ui->tableWidget_taches->setItem(ligne, 6,
                                                    new QTableWidgetItem(ui->tableViewCultures->model()->data(ui->
                                                                                                              tableViewCultures->
                                                                                                              model()->index(row,
                                                                                                                             7))
                                                                         .toString()));
                    int culture =
                        ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 0)).toString().toInt();
                    QString designation =
                        ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 1)).toString();
                    QString texte_date =
                        ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 3)).toString();
                    int duree =
                        ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 4)).toString().toInt();
                    QString num_plante =
                        ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 5)).toString();
                    QString etat_affichage =
                        ui->tableViewCultures->model()->data(ui->tableViewCultures->model()->index(row, 7)).toString();
                    QSqlQuery query;
                    QString   nom_plante;
                    query.exec(QString("select designation from plantes where id =" + num_plante));
                    if (query.first())
                    {
                        nom_plante = query.value(0).toString();
                    }


                    QString num_espece;
                    query.exec(QString("select espece from plantes where id =" + num_plante));
                    if (query.first())
                    {
                        num_espece = query.value(0).toString();
                    }


                    QString num_famille;
                    query.exec(QString("select famille from especes where id =" + num_espece));
                    if (query.first())
                    {
                        num_famille = query.value(0).toString();
                    }


                    QString nom_famille;
                    query.exec(QString("select designation from familles where id =" + num_famille));
                    if (query.first())
                    {
                        nom_famille = query.value(0).toString();
                    }
                    else
                    {
                        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                            query.lastQuery();
                    }

                    QDate date_tache = QDate::fromString(texte_date, "yyyy.MM.dd");
                    int   jour       = int(date.daysTo(date_tache));
                    int   semaine    = int(((dateR.daysTo(date_tache)) / 6));
                    int   famille    = num_famille.toInt();

                    QColor couleur_famille;
                    couleur_famille = "#ded8d7";

                    if (famille == 3)                  //apiacées
                    {
                        couleur_famille = "#ff5500";
                    }
                    if (famille == 4)                  //astéracées
                    {
                        couleur_famille = "#aaaa7f";
                    }
                    if (famille == 7)                  //brassicacées
                    {
                        couleur_famille = "#aaffff";
                    }
                    if (famille == 8)                  //chenopodacées
                    {
                        couleur_famille = "#ff5500";
                    }
                    if (famille == 10)                  //cucurbitacées
                    {
                        couleur_famille = "#ff557f";
                    }
                    if (famille == 12)                  //fabacées
                    {
                        couleur_famille = "#00ff00";
                    }
                    if (famille == 15)                  //alliacées
                    {
                        couleur_famille = "#ffff7f";
                    }
                    if (famille == 17)                  //poacées
                    {
                        couleur_famille = "#aaaaff";
                    }
                    if (famille == 21)                  //solanacées
                    {
                        couleur_famille = "#ff00ff";
                    }
                    if (famille == 22)                  //valerianacées
                    {
                        couleur_famille = "#55ff7f";
                    }
                    if (choix == 0)
                    {
                        ajouter_vignette_planning(culture, designation, h + 1, jour, duree, couleur_famille, etat_affichage);
                    }
                    if (choix == 1)
                    {
                        ajouter_vignette_rotation(culture, nom_plante + "\n " + nom_famille, h + 1, semaine, (duree / 6) + 1,
                                                  couleur_famille, etat_affichage);                                                              //titre,nb ligne,colonne depart,nb cases,couleur
                    }
                }
            }
        }
    }
}

/*********************************IMPRESSIONS*****************************************/

void MainWindow::on_pushButton_print_clicked()
{
    on_actionImprimer_le_plan_triggered();
}

void MainWindow::on_actionImprimer_le_plan_triggered()
{
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);

    printer->setPaperSize(QPrinter::A4);
    printer->setOutputFormat(QPrinter::NativeFormat);
    // printer->setOutputFormat (QPrinter::PdfFormat);
    printer->setOrientation(QPrinter::Landscape);
    printer->setResolution(96);
    printer->setFullPage(true);
    QPrintDialog printDialog(printer, this);
    if (printDialog.exec() == 1)
    {
        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        scene->render(&painter);
    }
}

/***************************************************************************************/
//AFFICHAGE DE LA GRILLE


void MainWindow::on_actionAfficher_grille_triggered()
{   //AFFICHER LA GRILLE
    setGrille(0);
    ui->actionAfficher_grille->setChecked(true);
    ui->actionCacher_la_grille->setChecked(false);
}

void MainWindow::on_actionCacher_la_grille_triggered()
{   //CACHER LA GRILLE
    setGrille(1);
    ui->actionAfficher_grille->setChecked(false);
    ui->actionCacher_la_grille->setChecked(true);
}

/*****************************DESSIN DES POLYGONES****************************/

QPolygon MainWindow::convertStrToPoly(const QString MyString)
{
    //conversion d'une chaine de caractères avec séparateur des QPoints avec ; en QPolygon

    QStringList liste = MyString.split(';', QString::SkipEmptyParts);
    QPolygon    poly(liste.count());

    for (int i = 0; i < liste.count(); i++)
    {
        QString     valeur_point = liste[i];
        QStringList liste_point  = valeur_point.split(',', QString::SkipEmptyParts);
        QString     strX         = liste_point[0];
        QString     strY         = liste_point[1];
        int         x            = strX.toInt();
        int         y            = strY.toInt();
        poly.setPoint(i, x, y);
    }
    return poly;
}

void MainWindow::ShowContextMenu(const QPoint& pos) // this is a slot
{
    QList <Sommet *> sommets;

    foreach(QGraphicsItem * item, scene->items())
    {
        if (Sommet *sommet = qgraphicsitem_cast <Sommet *>(item))
        {
            if (sommet->isSelected())
            {
                QPoint globalPos = ui->graphicsView->mapToGlobal(pos);
                //  QPoint globalPos = qobject_cast< Sommet* >( sender() )->mapToGlobal( pos );

                QMenu myMenu;

                myMenu.addAction("Ajouter");
                myMenu.addAction("Supprimer");

                QAction *selectedItem = myMenu.exec(globalPos);
                if (selectedItem)
                {
                    if (selectedItem->text() == "Ajouter")
                    {
                        ajouterSommet();
                        break;
                    }
                    if (selectedItem->text() == "Supprimer")
                    {
                        supprimerSommet();
                        break;
                    }
                }
            }
        }
    }
}

void MainWindow::ajouterSommet()
{
    //ajout sommet
    QList <Sommet *> sommets;
    qreal            position = 0;

    foreach(QGraphicsItem * item, scene->items())
    {
        if (Sommet *sommet = qgraphicsitem_cast <Sommet *>(item))
        {
            position++;
            sommet->setZValue(position);
            if (sommet->isSelected())
            {
                Sommet *newSommet = new Sommet();
                int     xV        = int(sommet->x());
                int     yV        = int(sommet->y());

                newSommet->setPos(xV + 20, yV);
                newSommet->setZValue(position++);
                scene->addItem(newSommet);
            }
        }
    }


    validerPolygone();
    on_toolButton_modifierPolygon_toggled(true);
}

void MainWindow::supprimerSommet()
{
    QList <Sommet *> sommets;

    foreach(QGraphicsItem * item, scene->items())
    {
        if (Sommet *sommet = qgraphicsitem_cast <Sommet *>(item))
        {
            if (sommet->isSelected())
            {
                delete sommet;
            }
        }
    }
    validerPolygone();
    on_toolButton_modifierPolygon_toggled(true);
}

void MainWindow::validerPolygone()
{
    //validation du polygone
    QString strPolygon = "";

    QList <QGraphicsItem *> itemList = scene->items();
    QPolygon poly;
    int      drap1    = 0;
    int      initValX = 0;
    int      initValY = 0;

    for (int i = itemList.size() - 1; i >= 0; i--)
    {
        QPointF location = itemList[i]->pos();
        int     valX     = int(location.x());
        int     valY     = int(location.y());

        if (valX != 0 && valY != 0)
        {
            if (itemList[i]->type() == 65538)
            {
                if (drap1 == 0)
                {
                    initValX = int(itemList[i]->pos().x());
                    initValY = int(itemList[i]->pos().y());
                }
                drap1++;
                valX = int(location.x() - initValX);
                valY = int(location.y() - initValY);
                poly << QPoint(valX, valY);
                strPolygon = (strPolygon + QString::number(valX) + "," + QString::number(valY) + ";");
            }
        }
    }
    MyPolygone *polygon = new MyPolygone(convertStrToPoly(strPolygon));
    polygon->setPos(QPoint(initValX, initValY));
    polygon->setColor(QColor(Qt::yellow));
    polygon->setPenColor(QColor(ui->label_couleurPolyline_Pen->text()));
    polygon->setColor(QColor(ui->label_couleurPolyline_P_background->text()));
    polygon->setWidthLine(ui->comboBox_epaisseurLignes_P->currentText().toInt());
    polygon->setTypeLine(ui->comboBox_typeLigne_P->currentIndex() + 1);
    polygon->setOpac(0.8);                                    //opacité de l'item
    if (mode_modifier == true)
    {
        polygon->setId(ui->lineEdit_Id_Item->text().toInt());
        polygon->setTexte(ui->textEdit_plan_commentaires->document()->toPlainText());
        polygon->setNom("Parcelle " + ui->lineEdit_Id_Item->text());
    }
    else
    {
        polygon->setId(get_MaxId());
        polygon->setTexte("Parcelle ");
        polygon->setNom("Parcelle " + QString::number(get_MaxId()));
    }
    polygon->setEtat(1);
    polygon->setMode(0);
    polygon->setModif(0);
    polygon->setTypeShape(5);      //polygone
    polygon->setStrPoly(strPolygon);
    polygon->setPoly(convertStrToPoly(strPolygon));
    scene->addItem(polygon);
    polygon->setSelected(true);
    strPolygon = "";
    //suppression des items Sommets
    QList <Sommet *> sommets;
    foreach(QGraphicsItem * item, scene->items())
    {
        if (Sommet *sommet = qgraphicsitem_cast <Sommet *>(item))
        {
            sommets << sommet;
        }
    }
    foreach(Sommet * sommet, sommets)
    {
        delete sommet;
    }
    ui->toolButton_newPolygon->setChecked(false);
    ui->pushButton_recorPlan->show();
    ui->toolButton_modifierPolygon->setChecked(false);

    MyGraphicsScene *scene2 = dynamic_cast <MyGraphicsScene *> (scene);
    scene2->setMode(MyGraphicsScene::Utilisation);
    on_actionUtilisation_triggered();
    mode_modifier = false;
    ajouter_parcelle(QString::number(polygon->getId()), polygon->getNom());
}

void MainWindow::validerPolyline()
{
    //validation du polyline
    QString strPolygon = "";

    QList <QGraphicsItem *> itemList = scene->items();
    QPolygon poly;
    int      drap1    = 0;
    int      initValX = 0;
    int      initValY = 0;

    for (int i = itemList.size() - 1; i >= 0; i--)
    {
        QPointF location = itemList[i]->pos();
        int     valX     = int(location.x());
        int     valY     = int(location.y());

        if (valX != 0 && valY != 0)
        {
            if (itemList[i]->type() == 65538)
            {
                if (drap1 == 0)
                {
                    initValX = int(itemList[i]->pos().x());
                    initValY = int(itemList[i]->pos().y());
                }
                drap1++;
                valX = int(location.x() - initValX);
                valY = int( location.y() - initValY);
                poly << QPoint(valX, valY);
                strPolygon = (strPolygon + QString::number(valX) + "," + QString::number(valY) + ";");
            }
        }
    }
    MyPolyline *polyline = new MyPolyline(convertStrToPoly(strPolygon));
    polyline->setPos(QPoint(initValX, initValY));
    polyline->setPenColor(QColor(ui->label_couleurPolyline_Pen->text()));
    polyline->setColor(QColor(ui->label_couleurPolyline_P_background->text()));
    polyline->setWidthLine(ui->comboBox_epaisseurLignes_P->currentText().toInt());
    polyline->setTypeLine(ui->comboBox_typeLigne_P->currentIndex() + 1);
    polyline->setOpac(0.8);                               //opacité de l'item

    if (mode_modifier == true)
    {
        polyline->setId(ui->lineEdit_Id_Item->text().toInt());
        polyline->setTexte(ui->textEdit_plan_commentaires->document()->toPlainText());
        polyline->setNom("Réseau " + ui->lineEdit_Id_Item->text());
    }
    else
    {
        polyline->setId(get_MaxId());
        polyline->setTexte("Réseau");
        polyline->setNom("Réseau " + QString::number(get_MaxId()));
    }

    polyline->setEtat(1);
    polyline->setMode(0);
    polyline->setTypeShape(6);   //polyline
    polyline->setStrPoly(strPolygon);
    polyline->setPoly(convertStrToPoly(strPolygon));
    scene->addItem(polyline);
    strPolygon = "";

    //suppression des items Sommets
    QList <Sommet *> sommets;
    foreach(QGraphicsItem * item, scene->items())
    {
        if (Sommet *sommet = qgraphicsitem_cast <Sommet *>(item))
        {
            sommets << sommet;
        }
    }
    foreach(Sommet * sommet, sommets)
    delete sommet;

    ui->toolButton_newPolygon->setChecked(false);
    ui->pushButton_recorPlan->show();
    ui->toolButton_modifierPolygon->setChecked(false);
    mode_modifier = false;
}

void MainWindow::on_toolButton_validerPolygone_clicked()
{
    //validation et enregistrement du polygone
    validerPolygone();
    //  items_vers_listeparcelles();
    MyGraphicsScene *scene2 = dynamic_cast <MyGraphicsScene *> (scene);
    scene2->setMode(MyGraphicsScene::Utilisation);
    ui->toolButton_validerPolygone->setDisabled(true);
    ui->toolButton_ValiderPolyline->setDisabled(true);
    on_actionUtilisation_triggered();
}

void MainWindow::on_toolButton_ValiderPolyline_clicked()
{
    //validation du polyline
    validerPolyline();
    //  items_vers_listeparcelles();
    MyGraphicsScene *scene2 = dynamic_cast <MyGraphicsScene *> (scene);
    scene2->setMode(MyGraphicsScene::Utilisation);
    ui->toolButton_validerPolygone->setDisabled(true);
    ui->toolButton_ValiderPolyline->setDisabled(true);
    on_actionUtilisation_triggered();
}

void MainWindow::modifierPolygone()
{   // modification du polygone selectionné
    scene->update();
    mode_modifier = true;
    if (m_mode == 0)
    {
        QList <QGraphicsItem *> itemList = scene->items();
        for (int i = 0; i < itemList.size(); i++)
        {
            if (itemList[i]->isSelected())
            {
                if (itemList[i]->type() == 65537)    // MyPolygon
                {
                    MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                    item->setModif(1);    //drapeau de modification du polygone
                    QList <QPoint> ListSommets = item->getPoly().toList();
                    for (int i = 0; i < ListSommets.size(); i++)
                    {
                        Sommet *itemV     = new Sommet();
                        QPoint  posSommet = ListSommets[i];
                        int     valX      = int(posSommet.x() + item->pos().x());
                        int     valY      = int(posSommet.y() + item->pos().y());
                        itemV->setPos(valX, valY);
                        scene->addItem(itemV);
                    }
                    QList <Sommet *> sommets;
                    foreach(QGraphicsItem * item, scene->items())
                    {
                        if (Sommet *sommet = qgraphicsitem_cast <Sommet *>(item))
                        {
                            sommets << sommet;
                        }
                    }
                    for (int i = 0; i < sommets.size() - 1; i++)
                    {
                        if (sommets.size() > 1)
                        {
                            int     IdSource      = i;
                            int     IdDestination = i + 1;
                            Sommet *source        = dynamic_cast <Sommet *>(sommets[IdSource]);
                            Sommet *destination   = dynamic_cast <Sommet *>(sommets[IdDestination]);
                            Cote *  NouveauLien   = new Cote(source, destination);
                            scene->addItem(NouveauLien);
                        }
                    }
                    int     IdSource      = 0;
                    int     IdDestination = sommets.size() - 1;
                    Sommet *source        = dynamic_cast <Sommet *>(sommets[IdSource]);
                    Sommet *destination   = dynamic_cast <Sommet *>(sommets[IdDestination]);
                    Cote *  NouveauLien   = new Cote(source, destination);
                    scene->addItem(NouveauLien);
                    delete item; //suppression du polygone existant
                    break;
                }
                if (itemList[i]->type() == 65539)    // MyPolyline
                {
                    //passer en mode modification du polyline
                    MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                    item->setModif(1);    //drapeau de modification du polygone

                    QList <QPoint> ListSommets = item->getPoly().toList();
                    for (int i = 0; i < ListSommets.size(); i++)
                    {
                        Sommet *itemV     = new Sommet();
                        QPoint  posSommet = ListSommets[i];
                        int     valX      = int(posSommet.x() + item->pos().x());
                        int     valY      = int( posSommet.y() + item->pos().y());
                        itemV->setPos(valX, valY);
                        scene->addItem(itemV);
                    }
                    QList <Sommet *> sommets;
                    foreach(QGraphicsItem * item, scene->items())
                    {
                        if (Sommet *sommet = qgraphicsitem_cast <Sommet *>(item))
                        {
                            sommets << sommet;
                        }
                    }
                    for (int i = 0; i < sommets.size() - 1; i++)
                    {
                        if (sommets.size() > 1)
                        {
                            int     IdSource      = i;
                            int     IdDestination = i + 1;
                            Sommet *source        = dynamic_cast <Sommet *>(sommets[IdSource]);
                            Sommet *destination   = dynamic_cast <Sommet *>(sommets[IdDestination]);
                            Cote *  NouveauLien   = new Cote(source, destination);
                            scene->addItem(NouveauLien);
                        }
                    }
                    delete item; //suppression du polygone existant
                    break;
                }
            }
        }
        ui->toolButton_validerPolygone->setEnabled(true);
        ui->toolButton_ValiderPolyline->setEnabled(true);
    }
    else
    {
        // quitter le mode modification du polygone
        QList <QGraphicsItem *> itemList = scene->items();
        for (int i = 0; i < itemList.size(); i++)
        {
            if (itemList[i]->type() == 65537)    // MyPolygon
            {                                    //quitter le mode modification du polygone
                MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                if (item->getMode() == 1)
                {
                    item->setModif(0);          //drapeau de fin de modification du polygone
                }
            }
        }
        //suppression des items sommets
        for (int i = itemList.size() - 1; i >= 0; i--)
        {
            QPointF location = itemList[i]->pos();
            int     valX     = int(location.x());
            int     valY     = int(location.y());
            if (valX != 0 && valY != 0)
            {
                if (itemList[i]->type() == 65538)
                {
                    scene->removeItem(itemList[i]);
                }
            }
        }
    }
    MyGraphicsScene *scene2 = dynamic_cast <MyGraphicsScene *> (scene);
    scene2->setMode(MyGraphicsScene::Utilisation);
}

void MainWindow::on_toolButton_modifierPolygon_toggled(bool checked)
{
    //passer en mode modification du polygone selectionné
    if (checked == true)
    {
        modifierPolygone();
        ui->toolButton_validerPolygone->setEnabled(true);
        ui->toolButton_ValiderPolyline->setEnabled(true);
    }
}

void MainWindow::on_toolButton_newPolygon_toggled(bool checked)
{
    Q_UNUSED(checked)
    MyGraphicsScene * scene2 = dynamic_cast <MyGraphicsScene *> (scene);

    if (ui->toolButton_newPolygon->isChecked() && m_mode == 0)
    {
        ui->toolButton_validerPolygone->setEnabled(true);
        ui->toolButton_ValiderPolyline->setEnabled(true);
        scene2->setMode(MyGraphicsScene::InsertSommet);
    }
    else
    {
        ui->toolButton_validerPolygone->setEnabled(true);
        ui->toolButton_ValiderPolyline->setEnabled(true);
        scene2->setMode(MyGraphicsScene::Modification);
    }
}

void MainWindow::on_comboBox_epaisseurLignes_P_currentIndexChanged(const QString&arg1)
{
    QList <QGraphicsItem *> itemList = scene->items();

    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                MyItem *item = dynamic_cast <MyItem *> (itemList[i]);
                item->setWidthLine(arg1.toInt());
                //   ui->tableObjets->setItem(i, 5,new QTableWidgetItem(QString::number(arg1.toInt())));
            }
            if (itemList[i]->type() == 65537)
            {
                MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setWidthLine(arg1.toInt());
            }
            if (itemList[i]->type() == 65539)
            {
                MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                item->setWidthLine(arg1.toInt());
            }
        }
    }
    // items_vers_listeparcelles();

    ui->pushButton_recorPlan->show();
    scene->update();
}

void MainWindow::on_comboBox_typeLigne_P_currentIndexChanged(int index)
{
    QList <QGraphicsItem *> itemList = scene->items();

    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                MyItem *item = dynamic_cast <MyItem *> (itemList[i]);
                item->setTypeLine(index + 1);
                item->setOldTypeLine(index + 1);
//             ui->tableObjets->setItem(i, 6,new QTableWidgetItem(QString::number(index+1)));
            }
            if (itemList[i]->type() == 65537)
            {
                MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setTypeLine(index + 1);
                item->setOldTypeLine(index + 1);
            }
            if (itemList[i]->type() == 65539)
            {
                MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                item->setTypeLine(index + 1);
                item->setOldTypeLine(index + 1);
            }
        }
    }
    // items_vers_listeparcelles();

    ui->pushButton_recorPlan->show();
    scene->update();
}

void MainWindow::on_toolButton_InsertRectangle_clicked()
{
    on_actionAjouterRectangle_triggered();
}

void MainWindow::on_toolButton_InsertRoudedRectangle_clicked()
{
    on_actionAjouter_Rectangle_arrondi_triggered();
}

void MainWindow::on_toolButton_InsertCircle_clicked()
{
    on_actionAjouterCercle_triggered();
}

void MainWindow::on_toolButton_InsertImage_clicked()
{
    on_actionAjouterImage_triggered();
}

void MainWindow::on_toolButton_DeleteObjet_clicked()
{
    on_actionSupprimer_triggered();
}

void MainWindow::on_toolButton_CouleurCrayon_clicked()
{
    on_actionChoisir_le_type_de_crayon_triggered();
}

void MainWindow::on_toolButton_CouleurFond_clicked()
{
    on_actionChoisir_la_couleur_triggered();
}

/*********************ZOOM*************************/

void MainWindow::on_pushButton_zoomIn_clicked()
{
    zoomGraphicsView(2);
}

void MainWindow::on_pushButton_ZoomOut_clicked()
{
    zoomGraphicsView(3);
}

void MainWindow::zoomGraphicsView(int ratio)
{   // valeur ratio in-> 2  out-> 3)
    if (ratio == 3 && m_ZoomRatio > 0.5)
    {
        m_ZoomRatio = m_ZoomRatio * 0.8;
        ui->graphicsView->scale(0.8, 0.8);
    }
    if (ratio == 2 && m_ZoomRatio < 3)
    {
        m_ZoomRatio = m_ZoomRatio * 1.25;
        ui->graphicsView->scale(1.25, 1.25);
    }
    ui->label_zoom->setText(QString::number(m_ZoomRatio, 'f', 2));
}

/**************deplacement graphicsView*******************/

void MainWindow::on_pushButton_fleche_clicked()
{
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    ui->graphicsView->setCursor(Qt::ArrowCursor);
}

void MainWindow::on_pushButton_Deplace_clicked()
{
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
}

void MainWindow::on_actionAide_triggered()
{   //ouvrir la fenêtre aide
    dialog_Aide *fenetre_aide = new dialog_Aide(this);

    fenetre_aide->show();
}

void MainWindow::on_actionFiches_plantes_triggered()
{
    // affichage de la fiche des variétés de plantes
    Fiche_plantes *fenetre_plante = new Fiche_plantes(0);

    fenetre_plante->show();
}

void MainWindow::on_actionConfiguration_triggered()
{
    QString oldSqlFileName = getfileNameSQL();
    //ouvrir la fenêtre configuration
    Configuration *fenetre_configuration = new Configuration(getFileNameXML(), getfileNameSQL(), this);
    int            resultat = fenetre_configuration->exec();

    if (resultat == QDialog::Accepted)
    {
        setFileNameXML(fenetre_configuration->getXmlFilName());
        setfileNameSQL(fenetre_configuration->getSqlFileName());

        qDebug() << " base de données ouverte " << getfileNameSQL() << " XML " << getFileNameXML();
        if (getfileNameSQL() != oldSqlFileName)
        {   // si la base de données change tester la version et remplir la table des parcelles selon le fichier XML en cours
            createConnection(getfileNameSQL());
            testVersion();
            remplir_table_parcelles();
        }
    }
}

void MainWindow::on_action_Licenses_triggered()
{   // ouvrir la fenêtre des licenses
    //ouvrir la fenêtre configuration
    Licenses *fenetre_licenses = new Licenses(this);
    int       resultat         = fenetre_licenses->exec();

    if (resultat == QDialog::Accepted)
    {
        // qDebug() << "accepted ";
    }
}

void MainWindow::on_actionAfficher_Gantt_triggered()
{
    Planner *fenetre_Gantt = new Planner(this);
    int      resultat      = fenetre_Gantt->exec();

    if (resultat == QDialog::Accepted)
    {
        // qDebug() << "accepted ";
    }
}

void MainWindow::on_actionOuvrir_fiche_taches_triggered()
{
    Dialog_taches *FicheTaches = new Dialog_taches(0, this);
    int            resultat    = FicheTaches->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void MainWindow::on_actionOuvrir_fiche_ressources_triggered()
{
    // editer une fiche de ressource
    Dialog_ressources *Fiche = new Dialog_ressources(0, 0, this);
    int resultat             = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void MainWindow::on_actionOuvrir_fiche_Moyens_triggered()
{
    // editer une fiche des moyens
    Dialog_moyens *Fiche    = new Dialog_moyens(1, this);
    int            resultat = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void MainWindow::on_actionOuvrir_fiche_Coordonnees_triggered()
{
    // editer une fiche de coordonnées
    Dialog_coordonnees *Fiche = new Dialog_coordonnees(0, this);
    int resultat = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void MainWindow::on_actionOuvrir_fiche_Type_de_Moyens_triggered()
{
    // editer une fiche de type de ressource
    Dialog_type_de_moyen *Fiche = new Dialog_type_de_moyen(0, this);
    int resultat = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void MainWindow::on_pushButton_Detail_clicked()
{   // affichage du détail de la parcelle sélectionnée
    int Id = ui->lineEdit_Id_Item->text().toInt();

    if (Id >= 0)
    {
        QList <QGraphicsItem *> itemList = scene->items();
        for (int i = 0; i < itemList.size(); i++)
        {
            if (itemList[i]->isSelected())
            {
                if (itemList[i]->type() == 65536) // MyItem
                {
                    MyItem *         item           = dynamic_cast <MyItem *> (itemList[i]);
                    QString          fileName       = item->getDetailFile();
                    detail_parcelle *fenetre_detail = new detail_parcelle(Id, fileName);
                    int resultat = fenetre_detail->exec();
                    if (resultat == QDialog::Accepted)
                    {
                        qDebug() << "nom du fichier de détail" << fenetre_detail->getFileNameXML();
                        item->setDetailFile(fenetre_detail->getFileNameXML());
                    }
                }
                if (itemList[i]->type() == 65537) // MyItem
                {
                    MyPolygone *     item           = dynamic_cast <MyPolygone *> (itemList[i]);
                    QString          fileName       = item->getDetailFile();
                    detail_parcelle *fenetre_detail = new detail_parcelle(Id, fileName);
                    int resultat = fenetre_detail->exec();
                    if (resultat == QDialog::Accepted)
                    {
                        qDebug() << "nom du fichier de détail" << fenetre_detail->getFileNameXML();
                        item->setDetailFile(fenetre_detail->getFileNameXML());
                    }
                }
            }
        }
        ui->pushButton_recorPlan->show();
    }
}

void MainWindow::on_actionAide_PDF_triggered()
{
    QProcess *process = new QProcess(this);

    process->start("/etc/alternatives/x-www-browser /usr/share/openjardin/notice_openJardin_1-06.pdf");
}

void MainWindow::on_actionAfficherPlanning_triggered()
{   // Afficher le planning
    Planning *Fiche = new Planning(0);

    Fiche->show();
}

void MainWindow::on_toolButton_DeleteObjet_triggered(QAction *arg1)
{
}

void MainWindow::on_pushButton_clicked()
{   // ouvrir le planning de la parcelle
    int id_parcelle = ui->lineEdit_Id_Item->text().toInt();


    Planning *Fiche = new Planning(id_parcelle);

    Fiche->show();
}


