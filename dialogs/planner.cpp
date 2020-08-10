#include "planner.h"
#include "ui_planner.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsGridLayout>

#include <QDebug>
#include <QtWidgets>
#include <QDialog>
#include <QDir>
#include <QTimerEvent>
#include <QTextStream>
#include <QPainter>
#include <QCoreApplication>
#include <QSettings>

#include "graphic/planning_item.h"
#include "graphic/myitem.h"
#include "graphic/mygrilleitem.h"
#include "graphic/tache_item.h"
#include "graphic/mypolyline.h"
#include "dialogs/dialog_taches.h"
#include "dialogs/dialog_type_de_tache.h"
#include "dialogs/dialog_type_de_moyen.h"
#include "dialogs/dialog_ressources.h"
#include "dialogs/dialog_coordonnees.h"
#include "dialogs/dialog_moyens.h"
#include "utilitaires/util.h"

#include "consts.h"



Planner::Planner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Planner)
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
    scene_planning = new QGraphicsScene(this);
    scene_planning->setSceneRect(0, 0, 5205, 800);//5320
    scene_planning->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    scene_planning3 = new QGraphicsScene(this);
    scene_planning3->setSceneRect(0, 0, 5320, 100);
    scene_planning3->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    ui->graphicsView_planning->setScene(scene_planning);
    ui->graphicsView_planning3->setScene(scene_planning3);

    // synchronisation des deux graphicsviews du planning

    connect(ui->graphicsView_planning->horizontalScrollBar(), SIGNAL(valueChanged(
                                                                         int)),
            ui->graphicsView_planning3->horizontalScrollBar(), SLOT(setValue(int)));

    connect(scene_planning, SIGNAL(selectionChanged()), this, SLOT(ItemPlanning_clicked()));

    connect(ui->treeView_taches->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(update_planningslider(int)));


    // affichage du planning
    QDate   currentDate   = QDate::currentDate();
    QString year          = QString::number(currentDate.year());
    QString strDateFev    = "01/02/" + year;
    QDate   dateFev       = QDate::fromString(strDateFev, "dd/MM/yyyy");
    QString strDate       = "01/01/" + year;
    QDate   date          = QDate::fromString(strDate, "dd/MM/yyyy");
    int     jour          = date.dayOfWeek();
    int     NbJourFevrier = dateFev.daysInMonth();
    ui->comboBox_AnneeEnCours->setCurrentText(year);
    affiche_planning(jour, NbJourFevrier);
    set_drapEndExpanded(0); //initialisation flag pour bloquer treeexpanded
    // cacher les champs id
    ui->lineEdit_id_tache->hide();
    init_base();
}

Planner::~Planner()
{
    delete ui;
}

void Planner::update_planningslider(int position)
{
    ui->graphicsView_planning->verticalScrollBar()->setValue(position * 28);
}

void Planner::init_base()
{
    /********************initialisation des bases de données sqlite******************/
    QSqlQueryModel *modelTaches = new QSqlQueryModel;

    modelTaches->setQuery(
        "SELECT id, designation, depart, fin, duree,commentaires,precedent, avancement,projet,type FROM tasks ORDER BY id ASC");
    modelTaches->setHeaderData(0, Qt::Vertical, QObject::tr("id"));
    modelTaches->setHeaderData(1, Qt::Vertical, QObject::tr("designation"));

    QSqlQueryModel *modelComboTaches = new QSqlQueryModel;
    modelComboTaches->setQuery(
        "SELECT designation FROM tasks WHERE type=1 ORDER BY id ASC");
    ui->comboBox_phases->setModel(modelComboTaches);
    populate_treeview();
}

/*********************************************************************************/
/************************  dessin du tableau planning  ***************************/
/*********************************************************************************/
void Planner::affiche_planning(int day, int bis)
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
    //ajout semaine 52 année précédente
    ajouter_vignette_semaine(texte_date + QString::number(
                                 52), WidthSemaine + 4, PosHSemaine, WidthSemaine, HeightSemaine - 14, Qt::white);
    for (int i = 1; i < 8; i++)
    {
        ajouter_vignette_jour("", ((i - 1) * spaceCase) + 4, PosJour, spaceCase, spaceCase, Qt::lightGray);
    }
    for (int v = 2; v < NbSemaines + 2; v++)
    {
        ajouter_vignette_semaine(texte_date + QString::number(
                                     v - 1), (v * WidthSemaine - ((day - 1) * 14)) + 3, PosHSemaine, WidthSemaine, HeightSemaine - 14,
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
    ajouter_vignette_mois("Decembre", 3, 8, Qt::lightGray);
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
    ui->graphicsView_planning->horizontalScrollBar()->setValue(int(5200 / 365 * (Posjour - 20)));
}

void Planner::ajouter_vignette_mois(QString titre, int colonne, int nb_jours, QColor couleur)
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

void Planner::ajouter_vignette_jour(QString titre, int colonne, int ligne, int width, int height, QColor couleur)
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

void Planner::ajouter_vignette_planning(int id_tache, QString titre, int ligne, int colonne, int nbCases, int forme,
                                        QColor couleur)
{   //titre,nb lignes,colonne de depart,nb cases,couleur
    int    spaceCase = Consts::SPACE_CASE;
    double lg        = ceil((ligne) * (spaceCase * 2));
    double col       = ceil((colonne - 1) * spaceCase);

    if (forme == 6 || forme == 7) // phase et avancement
    {
        tache_item *item  = new tache_item(nbCases * spaceCase, 4);
        double      sizeW = item->boundingRect().width() - 5;
        double      sizeH = item->boundingRect().height();
        double      posX  = ceil(col + (sizeW / 2));
        double      posY  = ceil(lg - (sizeH / 2) - 1) - spaceCase;
        item->setWidthLine(10);
        item->setId(id_tache);
        item->setColor(couleur);
        item->setPenColor(QColor(Qt::blue));
        item->setPos(posX, posY);
        item->setNom(titre);
        item->setEtat(1);
        item->setMode(1); // 1 pas de déplacements possibles
        item->setTypeShape(forme);
        scene_planning->addItem(item);
    }
    else if (forme == 5 || forme == 8) //triangle et losange
    {
        tache_item *item  = new tache_item(spaceCase, 28);
        double      sizeW = item->boundingRect().width() - 5;
        double      sizeH = item->boundingRect().height();
        double      posX  = ceil(col + (sizeW / 2));
        double      posY  = ceil(lg - (sizeH / 2) - 1);
        item->setWidthLine(10);
        item->setId(id_tache);
        item->setColor(couleur);
        item->setPenColor(QColor(Qt::blue));
        item->setPos(posX, posY);
        item->setNom(titre);
        item->setEtat(1);
        item->setMode(3); // 1 pas de déplacements possibles
        item->setTypeShape(forme);
        scene_planning->addItem(item);
    }
    else
    {
        tache_item *item  = new tache_item(nbCases * spaceCase, 28);
        double      sizeW = item->boundingRect().width() - 5;
        double      sizeH = item->boundingRect().height();
        double      posX  = ceil(col + (sizeW / 2));
        double      posY  = ceil(lg - (sizeH / 2) - 1);
        item->setId(id_tache);
        item->setColor(couleur);
        item->setPenColor(QColor(Qt::black));
        item->setPos(posX, posY);
        item->setNom(titre);
        item->setEtat(1);
        item->setMode(2);  //2 déplacements possibles
        item->setTypeShape(forme);
        scene_planning->addItem(item);
    }
}

void Planner::ajouter_intitule_planning(int id_tache, QString titre, int ligne, int colonne, int nbCases, QColor couleur)
{
    Q_UNUSED(colonne)
    int spaceCase = Consts::SPACE_CASE;
    int     lg   = (ligne) * (spaceCase * 2);
    MyItem *item = new MyItem(nbCases * spaceCase, spaceCase * 2);

    item->setId(id_tache);
    item->setColor(couleur);
    item->setPenColor(QColor(Qt::black));
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(sizeW / 2, lg - (sizeH / 2) - 1);
    item->setNom(titre);
    item->setEtat(1);
    item->setMode(1); //pas de déplacement possible
    item->setTypeShape(MyItem::Rectangle);
}

void Planner::ajouter_vignette_semaine(QString titre, int colonne, int ligne, int width, int height, QColor couleur)
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

void Planner::ajouter_vignette_horizontale(QString titre, int ligne, int width, int height, QColor couleur)
{
    MyItem *item2 = new MyItem(width, height);

    item2->setColor(couleur);
    item2->setPenColor(QColor(Qt::black));
    item2->setOpac(1.0); //opacité de l'item
    double sizeW2 = item2->boundingRect().width();
    double sizeH2 = item2->boundingRect().height();
    item2->setPos(sizeW2 / 2, ligne - (sizeH2 / 2) - 1);
    item2->setNom(titre);
    item2->setEtat(1);
    item2->setTypeShape(MyItem::Rectangle);
    item2->setMode(1);//mode utilisation
}

void Planner::ajouter_repere_jour(QString titre, int colonne, int ligne, int width, int height, QColor couleur)
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

void Planner::on_comboBox_AnneeEnCours_currentIndexChanged(const QString&arg1)
{
    //le changement de l'année du combobox entraine l'affichage du planning calé au 1er janvier
    QString strDate       = "01/01/" + arg1;
    QDate   date          = QDate::fromString(strDate, "dd/MM/yyyy");
    int     jour          = date.dayOfWeek();
    QString strDateFev    = "01/02/" + arg1;
    QDate   dateFev       = QDate::fromString(strDateFev, "dd/MM/yyyy");
    int     NbJourFevrier = dateFev.daysInMonth();

    affiche_planning(jour, NbJourFevrier);
    populate_treeview();
}

void Planner::ItemPlanning_clicked()
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
            if (itemList[i]->type() == 65537) // type tache_item
            {
                tache_item *item = dynamic_cast <tache_item *> (itemList[i]);
                ui->lineEdit_id_tache->setText(QString::number(item->getId()));
                nbJours    = (item->getWidth() / Consts::SPACE_CASE);
                dateDepart = int((((item->pos().x()) / Consts::SPACE_CASE) - nbJours / 2) - 7);
                QDate date_depart = dateAn.addDays(dateDepart);
                QDate date_fin    = date_depart.addDays(nbJours - 1);
                ui->dateEdit_depart->setDate(date_depart);
                ui->dateEdit_fin->setDate(date_fin);
                ui->lineEdit_duree->setText(QString::number(nbJours));
            }
        }
    }
}

void Planner::on_pushButton_validerDates_clicked()
{   // bouton valider les dates suite à click sur item tâche dans le planning
    if (ui->lineEdit_id_tache->text() != "")
    {
        QSqlQuery query;
        QString   designation = util::apos(ui->lineEdit_designation->text());
        QString   id_tache    = ui->lineEdit_id_tache->text();
        QString   depart      = ui->dateEdit_depart->date().toString("dd-MM-yyyy");
        QString   fin         = ui->dateEdit_fin->date().toString("dd-MM-yyyy");
        QString   duree       = ui->lineEdit_duree->text();
        QString   contrainte_date;

        if (ui->checkBox_ContrainteDate->isChecked())
        {
            contrainte_date = "1";
        }
        else
        {
            contrainte_date = "0";
        }
        QString strQuery = "update tasks set designation='" + designation + "',depart= '" + depart +
                           "',fin= '" + fin + "',duree=" + duree + ",contrainte_date=" + contrainte_date +
                           " where id=" + id_tache;

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
        init_base();
        mise_a_jour_phases();
        init_base();
    }
    else
    {
        QMessageBox::information(this, tr("Erreur "),
                                 tr("Veuillez sélectionner une tâche svp"));
    }
}

void Planner::mise_a_jour_phases()
{
    QSqlQuery query;

    query.exec(QString("SELECT id FROM tasks WHERE type = 1 ORDER BY id  ASC"));
    QList <tache>             listeDesTaches;
    QList <QPair <int, int> > ListPhases_Taches;
    QStringList listPhases;
    listPhases.clear();
    while (query.next())
    {
        QString id = query.value(0).toString();
        listPhases << id;
    }
    query.exec(QString("SELECT id,depart, fin,duree,phase_parent,type FROM tasks ORDER BY id  ASC"));
    while (query.next())
    {
        int     id           = query.value(0).toInt();
        QString depart       = query.value(1).toString();
        QString fin          = query.value(2).toString();
        int     duree        = query.value(3).toInt();
        int     phase_parent = query.value(4).toInt();
        int     type         = query.value(5).toInt();
        //remplir la structure
        listeDesTaches.append(tache{ id, depart, fin, duree, phase_parent, type });
    }
    for (int i = 0; i < listeDesTaches.size(); ++i)
    {
        if (listeDesTaches.at(i).type == 1)
        {
            int     id     = listeDesTaches.at(i).id;
            QString depart = listeDesTaches.at(i).depart;
            // QString fin          = listeDesTaches.at(i).fin;
            QString fin          = listeDesTaches.at(i).depart;
            int     duree        = listeDesTaches.at(i).duree;
            int     phase_parent = listeDesTaches.at(i).phase_parent;
            int     type         = listeDesTaches.at(i).type;
            QDate   date_fin     = QDate::fromString(fin, "dd-MM-yyyy");
            QDate   date_depart  = QDate::fromString(depart, "dd-MM-yyyy");
            // Initialisation des dates depart et fin des phases sur phase initiale
            QDate dateMinDepart = QDate::fromString(listeDesTaches.at(i).depart, "dd-MM-yyyy");
            QDate dateMaxFin    = QDate::fromString(listeDesTaches.at(i).depart, "dd-MM-yyyy");
            int   drap          = 0;
            for (int j = 0; j < listeDesTaches.size(); ++j)
            {
                if (listeDesTaches.at(j).type != 1 && listeDesTaches.at(j).phase_parent == id)
                {
                    if (drap == 0)
                    {
                        QDate dateMinDepart = QDate::fromString(listeDesTaches.at(j).depart, "dd-MM-yyyy");
                        QDate dateMaxFin    = QDate::fromString(listeDesTaches.at(j).depart, "dd-MM-yyyy");
                        depart = dateMinDepart.toString("dd-MM-yyyy");
                        fin    = dateMaxFin.toString("dd-MM-yyyy");
                    }
                    QString departTache      = listeDesTaches.at(j).depart;
                    QDate   date_departTache = QDate::fromString(departTache, "dd-MM-yyyy");
                    QString finTache         = listeDesTaches.at(j).fin;
                    QDate   date_finTache    = QDate::fromString(finTache, "dd-MM-yyyy");

                    if (dateMinDepart.daysTo(date_departTache) <= 1)
                    {
                        // qDebug() << " dateMindepart " << dateMinDepart << "date depart tache " << date_departTache <<
                        //     "nb jours " << dateMinDepart.daysTo(date_departTache);
                        dateMinDepart = date_departTache;
                        date_depart   = dateMinDepart;
                        duree         = int(date_depart.daysTo(date_finTache));
                        depart        = dateMinDepart.toString("dd-MM-yyyy");
                        drap          = 1;
                    }
                    if (dateMaxFin.daysTo(date_finTache) >= 1)
                    {
                        //  qDebug() << " dateMaxfin " << dateMaxFin << "date fin tache " << date_finTache <<
                        //     "nb jours " << dateMinDepart.daysTo(date_departTache);
                        dateMaxFin = date_finTache;
                        date_fin   = dateMaxFin;
                        duree      = int(date_depart.daysTo(date_finTache));
                        fin        = dateMaxFin.toString("dd-MM-yyyy");
                        drap       = 1;
                    }
                }
                listeDesTaches.replace(i, tache{ id, depart, fin, duree, phase_parent, type });
            }

            /*         qDebug() << " structure modifiée " << " id " << listeDesTaches.at(i).id << " depart " <<
             *            listeDesTaches.at(i).depart <<
             *            " fin " <<
             *            listeDesTaches.at(i).fin << " duree " <<
             *            listeDesTaches.at(i).duree << " phase " <<
             *            listeDesTaches.at(i).phase_parent << " type " <<
             *            listeDesTaches.at(i).type;*/

            QSqlQuery query;
            QString   strQuery = "update tasks set depart= '" + depart + "',fin= '" + fin + "',duree=" +
                                 QString::number(duree) +
                                 " where id=" +
                                 QString::number(id);
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

void Planner::maj_planning()
{   // mise à jour de l'affichage du planning basé sur année en cours du combobox
    QString strDate       = "01/01/" + ui->comboBox_AnneeEnCours->currentText();
    QDate   date          = QDate::fromString(strDate, "dd/MM/yyyy");
    int     jour          = date.dayOfWeek();
    QString strDateFev    = "01/02/" + ui->comboBox_AnneeEnCours->currentText();
    QDate   dateFev       = QDate::fromString(strDateFev, "dd/MM/yyyy");
    int     NbJourFevrier = dateFev.daysInMonth();

    affiche_planning(jour, NbJourFevrier);
}

/*********************************************************************************/
/*********************************  TREEVIEW  ************************************/
/*********************************************************************************/
void Planner::on_comboBox_phases_currentTextChanged(const QString&arg1)
{
    Q_UNUSED(arg1)
    ui->lineEdit_id_tache->setText("");
    ui->dateEdit_depart->clear();
    ui->dateEdit_fin->clear();
    ui->lineEdit_duree->setText("");
    ui->lineEdit_designation->setText("");
    ui->checkBox_ContrainteDate->setChecked(false);
    populate_treeview();
}

void Planner::populate_treeview()
{   // remplissage de l'arborescence de l'arbre
    set_drapEndExpanded(0);
    //selection plante dans combobox
    QSqlQuery queryComboPhase;
    QString   phase_parent;
    QString   designation_combo_phases = util::apos(ui->comboBox_phases->currentText());
    queryComboPhase.exec(QString("select phase_parent from tasks where designation ='" + designation_combo_phases + "'"));
    if (queryComboPhase.first())
    {
        phase_parent = queryComboPhase.value(0).toString();
    }
    else
    {
        qDebug() << "erreur query 664:" << queryComboPhase.lastError().text() << "  " <<
            queryComboPhase.lastError().databaseText() << queryComboPhase.lastQuery().toUtf8();
    }
    standardModel = new QStandardItemModel(this);
    QSqlQuery query(
        "SELECT id, designation, depart, fin, duree,commentaires,precedent, avancement,type FROM tasks WHERE phase_parent=" + phase_parent +
        " ORDER BY precedent ASC");
    const QSqlRecord rec = query.record();
    while (query.next())
    {
        QString        designation = query.value(rec.indexOf("designation")).toString();
        int            precedent   = query.value(rec.indexOf("precedent")).toInt();
        int            id          = query.value(rec.indexOf("id")).toInt();
        QStandardItem *pr          = new QStandardItem(designation);
        pr->setData(id, RelationRoles::CodeRole);

        if (precedent == 0)
        {
            standardModel->invisibleRootItem()->appendRow(pr);
        }
        else
        {
            QModelIndexList inds = standardModel->match(standardModel->index(0, 0),
                                                        RelationRoles::CodeRole,
                                                        precedent,
                                                        1,
                                                        Qt::MatchExactly | Qt::MatchRecursive);
            if (inds.size() > 0)
            {
                QStandardItem *parent = standardModel->itemFromIndex(inds.first());
                parent->appendRow(pr);
            }
        }
    }

    //remplir la treeview
    ui->treeView_taches->setModel(standardModel);
    ui->treeView_taches->expandAll();
    ui->treeView_taches->setUniformRowHeights(true);
    int row;
    int col;
    int rowMax = standardModel->rowCount();
    int colMax = standardModel->columnCount();
    for (row = 0; row < rowMax; row++)
    {
        if (row == rowMax)
        {
            set_drapEndExpanded(1);
        }
        for (col = 0; col < colMax; col++)
        {
            QModelIndex index = standardModel->index(row, col);
            standardModel->setData(index, QVariant(QSize(100, 28)), Qt::SizeHintRole);
        }
    }

    /************ afficher dans le planning selon les positions dans le tree view ****************/
    QModelIndex modelIndexp = ui->treeView_taches->indexAt(ui->treeView_taches->rect().topLeft());
    int         rowTreep    = 0;
    while (modelIndexp.isValid())
    {
        modelIndexp = ui->treeView_taches->indexBelow(modelIndexp);
        rowTreep++;
    }
    // qDebug() << "rowtree =" << rowTreep;
    resize_planning(rowTreep);
    /***********************************/
    QModelIndex modelIndex = ui->treeView_taches->indexAt(ui->treeView_taches->rect().topLeft());
    QString     strId      = ui->treeView_taches->model()->data(modelIndex, RelationRoles::CodeRole).toString();
    int         rowTree    = 0;
    //afficher l'item de root dans le planning
    afficher_item_planning(rowTree, strId);
    //afficher dans le planning les suivants selon l'ordre de l'arborescence
    while (modelIndex.isValid())
    {
        modelIndex = ui->treeView_taches->indexBelow(modelIndex);

        QString strposition = ui->treeView_taches->model()->data(modelIndex, RelationRoles::CodeRole).toString();
        rowTree++;
        afficher_item_planning(rowTree, strposition);
    }
    remplir_liste_lien_planning();
    afficher_les_liens_planning();
    set_drapEndExpanded(1);
}

void Planner::resize_planning(int nb_taches)
{
    int hauteur = (nb_taches) * 28;

    scene_planning->setSceneRect(0, 0, 5205, hauteur);
    maj_planning();
}

void Planner::afficher_item_planning(const int position, const QString id_item)
{
    int       decalagejour = 8;
    QString   strDate      = "01-01-" + ui->comboBox_AnneeEnCours->currentText();
    QDate     date         = QDate::fromString(strDate, "dd-MM-yyyy");
    QSqlQuery query;

    query.exec(QString("SELECT id, designation, depart, fin, avancement,type FROM tasks  where id=" +
                       id_item));
    if (query.first())
    {
        QString strDesignation  = query.value(1).toString();
        QString strDepart       = query.value(2).toString();
        QString strFin          = query.value(3).toString();
        QString strAvancement   = query.value(4).toString();
        QString strType         = query.value(5).toString();
        QDate   date_depart     = QDate::fromString(strDepart, "dd-MM-yyyy");
        QDate   date_fin        = QDate::fromString(strFin, "dd-MM-yyyy");
        double  jour            = date.daysTo(date_depart);
        double  duree           = ((date.daysTo(date_fin)) - jour);
        double  avancement_jour = (duree + 1) * strAvancement.toInt() / 100;
        QString couleur;
        QString forme;
        query.exec(QString("select couleur from type_de_tache where id=" + strType));
        if (query.first())
        {
            couleur = query.value(0).toString();
        }
        query.exec(QString("select forme from type_de_tache where id=" + strType));
        if (query.first())
        {
            forme = query.value(0).toString();
        }
        int id_tache = id_item.toInt();

        // etiquettes horinzontales des taches

        ajouter_vignette_planning(id_tache, " ", position + 1, decalagejour + jour, duree + 1, forme.toInt(), couleur);  // ajout  7 colonne

        if (avancement_jour > 0)
        {
            ajouter_vignette_planning(id_tache, " ", position + 1, decalagejour + jour, avancement_jour, 7, Qt::blue);
        }
        ui->graphicsView_planning->verticalScrollBar()->setValue(1);
    }
}

void Planner::on_treeView_taches_clicked(const QModelIndex&index)
{
    QSqlQuery query;
    QString   strposition    = ui->treeView_taches->model()->data(index, RelationRoles::CodeRole).toString();
    QString   strDesignation = index.data(Qt::DisplayRole).toString();

    ui->treeView_taches->model()->data(index);
    ui->lineEdit_id_tache->setText(strposition);
    query.exec(QString("select depart,fin, duree,contrainte_date from tasks where id=" + strposition));
    if (query.first())
    {
        QString strDepart       = query.value(0).toString();
        QString strFin          = query.value(1).toString();
        QString strDuree        = query.value(2).toString();
        int     contrainte_date = query.value(3).toInt();
        QDate   date_depart     = QDate::fromString(strDepart, "dd-MM-yyyy");
        QDate   date_fin        = QDate::fromString(strFin, "dd-MM-yyyy");
        int     duree           = int((date_depart.daysTo(date_fin)));
        ui->dateEdit_depart->setDate(date_depart);
        ui->dateEdit_fin->setDate(date_fin);
        ui->lineEdit_duree->setText(QString::number(duree + 1));
        ui->lineEdit_designation->setText(strDesignation);
        if (contrainte_date == 0)
        {
            ui->checkBox_ContrainteDate->setChecked(false);
        }
        else
        {
            ui->checkBox_ContrainteDate->setChecked(true);
        }
    }
    else
    {
        qWarning("ne peut récupérer la valeur  ");
    }
}

void Planner::on_lineEdit_id_tache_textChanged(const QString&arg1)
{
    QSqlQuery query;

    query.exec(QString("select designation,depart,fin, duree from tasks where id=" + arg1));
    if (query.first())
    {
        QString strDesignation = query.value(0).toString();
        QString strDepart      = query.value(1).toString();
        QString strFin         = query.value(2).toString();
        QString strDuree       = query.value(3).toString();
        QDate   date_depart    = QDate::fromString(strDepart, "dd-MM-yyyy");
        QDate   date_fin       = QDate::fromString(strFin, "dd-MM-yyyy");
        int     duree          = int((date_depart.daysTo(date_fin)));
        ui->dateEdit_depart->setDate(date_depart);
        ui->dateEdit_fin->setDate(date_fin);
        ui->lineEdit_duree->setText(QString::number(duree + 1));
        ui->lineEdit_designation->setText(strDesignation);
    }
    else
    {
        qWarning("ne peut récupérer la valeur ");
    }
}

void Planner::on_treeView_taches_collapsed(const QModelIndex&index)
{
    Q_UNUSED(index)
    effacer_item_planning();
    QModelIndex modelIndexp = ui->treeView_taches->indexAt(ui->treeView_taches->rect().topLeft());
    int         rowTreep    = 0;
    while (modelIndexp.isValid())
    {
        modelIndexp = ui->treeView_taches->indexBelow(modelIndexp);
        rowTreep++;
    }

    resize_planning(rowTreep);
    /***********************************/
    QModelIndex modelIndex  = ui->treeView_taches->indexAt(ui->treeView_taches->rect().topLeft());
    QString     strposition = ui->treeView_taches->model()->data(modelIndex, RelationRoles::CodeRole).toString();
    int         rowTree     = 0;
    afficher_item_planning(rowTree, strposition);
    while (modelIndex.isValid())
    {
        modelIndex = ui->treeView_taches->indexBelow(modelIndex);

        QString strposition = ui->treeView_taches->model()->data(modelIndex, RelationRoles::CodeRole).toString();
        rowTree++;
        afficher_item_planning(rowTree, strposition);
    }
    remplir_liste_lien_planning();
    afficher_les_liens_planning();
}

void Planner::on_treeView_taches_expanded(const QModelIndex&index)
{
    Q_UNUSED(index)
    if (get_drapEndExpanded() == 1)
    {
        effacer_item_planning();
        QModelIndex modelIndexp = ui->treeView_taches->indexAt(ui->treeView_taches->rect().topLeft());
        int         rowTreep    = 0;
        while (modelIndexp.isValid())
        {
            modelIndexp = ui->treeView_taches->indexBelow(modelIndexp);
            rowTreep++;
        }

        resize_planning(rowTreep);
        /***********************************/
        QModelIndex modelIndex  = ui->treeView_taches->indexAt(ui->treeView_taches->rect().topLeft());
        QString     strposition = ui->treeView_taches->model()->data(modelIndex, RelationRoles::CodeRole).toString();
        int         rowTree     = 0;
        afficher_item_planning(rowTree, strposition);
        while (modelIndex.isValid())
        {
            modelIndex = ui->treeView_taches->indexBelow(modelIndex);

            QString strposition = ui->treeView_taches->model()->data(modelIndex, RelationRoles::CodeRole).toString();
            rowTree++;
            afficher_item_planning(rowTree, strposition);
        }
        remplir_liste_lien_planning();
        afficher_les_liens_planning();
    }
}

void Planner::effacer_item_planning()
{   //effacement des items planning
    QList <QGraphicsItem *> itemList = scene_planning->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 65537)
        {
            //qDebug() << "type =" << itemList[i]->type() << " shape " << itemList[i]->shape();
            tache_item *item = dynamic_cast <tache_item *> (itemList[i]);
            delete item;
        }
    }
    ListPositions.clear();
}

void Planner::creer_lien_planning(int row_origin, int col_origin, int row_dest, int col_dest)
{   //dessiner un lien entre tâches dans le planning
    int      spaceCase = Consts::SPACE_CASE;
    QPolygon poly;
    int      lgOrigin  = int( ceil((row_origin) * (spaceCase * 2)));
    int      colOrigin = int (ceil((col_origin) * spaceCase));
    int      lgDest    = int(ceil((row_dest) * (spaceCase * 2)));
    int      colDest   = int(ceil((col_dest) * spaceCase));

    poly << QPoint(colOrigin, lgOrigin - spaceCase);
    poly << QPoint(colDest + (spaceCase / 2) - 2, lgOrigin - spaceCase);
    poly << QPoint(colDest + (spaceCase / 2) - 2, lgDest - (spaceCase * 2));
    //  poly << QPoint(colOrigin, lgOrigin - spaceCase);
    //  poly << QPoint(colOrigin, lgDest - spaceCase);
    //  poly << QPoint(colDest, lgDest - spaceCase);
    MyPolyline *polyline = new MyPolyline(poly);
    scene_planning->addItem(polyline);
}

void Planner::remplir_liste_lien_planning()
{   // remplir la liste des paires index - ligne du treeview
    ListPositions.clear();
    QSqlQuery   query;
    QModelIndex modelIndex = ui->treeView_taches->indexAt(ui->treeView_taches->rect().topLeft());
    int         rowTree    = 1;
    while (modelIndex.isValid())
    {
        modelIndex = ui->treeView_taches->indexBelow(modelIndex);
        QString strId    = ui->treeView_taches->model()->data(modelIndex, RelationRoles::CodeRole).toString();
        int     id_tache = strId.toInt();
        setListePosition(rowTree, id_tache);
        rowTree++;
    }
}

void Planner::afficher_les_liens_planning()
{   // afficher les liens entre tâches dans le planning
    int       decalagejour = 8;
    QString   strDate      = "01-01-" + ui->comboBox_AnneeEnCours->currentText();
    QDate     date         = QDate::fromString(strDate, "dd-MM-yyyy");
    QSqlQuery query;

    for (int i = 1; i < ListPositions.count(); i++)
    {
        QString StrId = QString::number(getListe_Id(i));
        // qDebug() << " row " << i << "StrId " << StrId;
        query.exec(QString("SELECT depart, precedent, contrainte_date FROM tasks  where id=" + StrId));
        if (query.first())
        {
            QString strDepart     = query.value(0).toString();
            QString strprecedent  = query.value(1).toString();
            QString strContrainte = query.value(2).toString();
            QDate   date_depart   = QDate::fromString(strDepart, "dd-MM-yyyy");
            double  jour          = date.daysTo(date_depart);
            // récupération de la date de fin de la tâche précedente si checkbox de contrainte activé
            if (strContrainte == "1")
            {
                QString strfinPrecedent;
                query.exec(QString("select fin from tasks where id =" + strprecedent));
                if (query.first())
                {
                    strfinPrecedent = query.value(0).toString();
                }
                else
                {
                    qDebug() << i << " erreur query fin :" << query.lastError().text() << "  " <<
                        query.lastError().databaseText() <<
                        query.driver();
                }
                QDate  date_fin_precedent = QDate::fromString(strfinPrecedent, "dd-MM-yyyy");
                double jour_precedent     = date.daysTo(date_fin_precedent);
                int    prec_row           = getListe_Position(strprecedent.toInt());
                // ajouter le lien graphique avec tâche précédente
                creer_lien_planning(prec_row + 1, decalagejour + jour_precedent, i + 1, decalagejour + jour - 1);
            }
        }
    }
}

void Planner::on_pushButton_AjouterTache_clicked()
{
    // créer une nouvelle tâche

    if (ui->lineEdit_id_tache->text().toInt() > 0)
    {
        QString designation  = "nouvelle tache";
        QString commentaires = " ";
        QString depart       = ui->dateEdit_depart->date().toString("dd-MM-yyyy");
        QString fin          = ui->dateEdit_depart->date().toString("dd-MM-yyyy");
        QString duree        = "1";
        QString id_culture;
        QString precedent       = ui->lineEdit_id_tache->text();
        QString contrainte_date = "0";
        QString phase_parent;
        QString avancement = "0";
        QString type       = "2";

        QSqlQuery queryComboPhase;

        QString designation_combo_phases = util::apos(ui->comboBox_phases->currentText());
        queryComboPhase.exec(QString("select phase_parent,id_culture from tasks where designation ='" + designation_combo_phases +
                                     "'"));
        if (queryComboPhase.first())
        {
            phase_parent = queryComboPhase.value(0).toString();
            id_culture   = queryComboPhase.value(1).toString();
            qDebug() << "phase parent " << phase_parent << " id_culture " << id_culture << "designation " <<
                designation_combo_phases;
        }
        else
        {
            qDebug() << "erreur query recherche phase_parent :" << queryComboPhase.lastError().text() << "  " <<
                queryComboPhase.lastError().databaseText() << queryComboPhase.lastQuery().toUtf8();
        }

        QSqlQuery query;

        QString str =
            "insert into tasks (designation,commentaires, depart, fin, duree,precedent, avancement,type,contrainte_date,phase_parent,id_culture)"
            "values('" + designation + "','" + commentaires + "','" + depart + "','" + fin +
            "'," + duree + "," + precedent + "," + avancement + "," + type + "," + contrainte_date + "," + phase_parent + "," +
            id_culture + ")";
        query.exec(str);
        if (!query.isActive())
        {
            qDebug() << "erreur query valider:" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.lastQuery().toUtf8();
            QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                     tr("Veuillez vérifier que tous les champs soient bien remplis"));
        }
        else
        {
            qDebug() << "enregistrement terminé avec succès";
        }
        maj_planning();
        populate_treeview();
    }
    else
    {
        QMessageBox::information(this, tr("Erreur "),
                                 tr("Veuillez sélectionner la tache précédente svp"));
    }
}

void Planner::on_pushButton_editTache_clicked()
{   // editer une fiche de tache
    int            idtache     = ui->lineEdit_id_tache->text().toInt();
    Dialog_taches *FicheTaches = new Dialog_taches(idtache, this);
    int            resultat    = FicheTaches->exec();

    if (resultat == QDialog::Accepted)
    {
        mise_a_jour_phases();
        maj_planning();
        populate_treeview();
    }
}

void Planner::on_toolButton_ficheTaches_clicked()
{
    Dialog_taches *FicheTaches = new Dialog_taches(0, this);
    int            resultat    = FicheTaches->exec();

    if (resultat == QDialog::Accepted)
    {
        maj_planning();
        populate_treeview();
    }
}

void Planner::on_toolButton_ficheRessources_clicked()
{
    // editer une fiche de ressource
    Dialog_ressources *Fiche = new Dialog_ressources(0, 0, this);
    int resultat             = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void Planner::on_toolButton_type_de_Moyens_clicked()
{
    // editer une fiche de type de ressource
    Dialog_type_de_moyen *Fiche = new Dialog_type_de_moyen(0, this);
    int resultat = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void Planner::on_toolButton_Coordonnees_clicked()
{
    // editer une fiche de coordonnées
    Dialog_coordonnees *Fiche = new Dialog_coordonnees(0, this);
    int resultat = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void Planner::on_toolButton_fiche_typeTaches_clicked()
{
    // editer une fiche de type de tâche
    Dialog_type_de_tache *Fiche = new Dialog_type_de_tache(1, this);
    int resultat = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void Planner::on_toolButton_ficheMoyens_clicked()
{
    // editer une fiche des moyens
    Dialog_moyens *Fiche    = new Dialog_moyens(1, this);
    int            resultat = Fiche->exec();

    if (resultat == QDialog::Accepted)
    {
    }
}

void Planner::on_toolButton_supprimerTache_clicked()
{   // supprimer une tâche
    if (ui->lineEdit_id_tache->text() != "")
    {
        QSqlQuery query;
        //remplacement de l'id de la tache precedente de la tâche à supprimer dans les tâches qui la suivent
        QString tacheActive = ui->lineEdit_id_tache->text();
        QString designation = util::apos(ui->lineEdit_designation->text());
        QString id_precedent;
        query.exec(QString("select precedent from tasks where id=" + tacheActive));
        if (query.first())
        {
            id_precedent = query.value(0).toString();
        }
        QString strQuery = "update tasks set precedent= " + id_precedent + " where precedent=" + tacheActive;
        query.exec(strQuery);

        // suppression de la tâche
        query.exec(QString("delete from tasks where designation='" + designation + "'"));
        if (!query.isActive())
        {
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.lastQuery();
        }
        else
        {
            qDebug() << "suppression terminée";
        }
        maj_planning();
        populate_treeview();
    }
    else
    {
        QMessageBox::information(this, tr("Erreur "),
                                 tr("Veuillez sélectionner une tâche svp"));
    }
}

void Planner::on_toolButton_close_clicked()
{
    close();
}
