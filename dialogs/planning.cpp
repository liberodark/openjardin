#include "planning.h"
#include "ui_planning.h"
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
#include "utilitaires/util.h"
#include "dialogs/cultures.h"

#include "consts.h"

Planning::Planning(const int&IdParcelle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Planning)
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
    set_idparcelle(IdParcelle);
    ui->setupUi(this);
    scene_planning = new QGraphicsScene();
    scene_planning->setSceneRect(0, 0, 5205, 800);//5320
    scene_planning->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    scene_planning3 = new QGraphicsScene();
    scene_planning3->setSceneRect(0, 0, 5320, 100);
    scene_planning3->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    ui->graphicsView_planning->setScene(scene_planning);
    ui->graphicsView_planning3->setScene(scene_planning3);

    // synchronisation des deux graphicsviews du planning

    connect(ui->graphicsView_planning->horizontalScrollBar(), SIGNAL(valueChanged(
                                                                         int)),
            ui->graphicsView_planning3->horizontalScrollBar(), SLOT(setValue(int)));

    //   connect(scene_planning, SIGNAL(selectionChanged()), this, SLOT(ItemPlanning_clicked()));

    connect(ui->treeView_taches->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(update_planningslider(int)));
    connect(scene_planning, SIGNAL(selectionChanged()), this, SLOT(ItemPlanning_clicked()));


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
    init_base();
    QSqlQuery queryComboPhase;
    QString   Designation_parcelle;
    queryComboPhase.exec(QString("select designation from parcelles where id = " + QString::number(get_idparcelle())));
    if (queryComboPhase.first())
    {
        Designation_parcelle = queryComboPhase.value(0).toString();
    }
    if (Designation_parcelle != "")
    {
        ui->comboBox_phases->setCurrentText(Designation_parcelle);
    }
    // cacher les champs id
    ui->lineEdit_id_culture->hide();
}

Planning::~Planning()
{
    delete ui;
}

void Planning::update_planningslider(int position)
{
    ui->graphicsView_planning->verticalScrollBar()->setValue(position * 28);
}

void Planning::init_base()
{
    /********************initialisation des bases de données sqlite******************/

    QSqlQueryModel *modelComboParcelles = new QSqlQueryModel;

    modelComboParcelles->setQuery(
        "SELECT designation FROM parcelles ORDER BY id ASC");
    qDebug() << modelComboParcelles->lastError();
    ui->comboBox_phases->setModel(modelComboParcelles);
    populate_treeview();
}

/*********************************************************************************/
/************************  dessin du tableau planning  ***************************/
/*********************************************************************************/
void Planning::affiche_planning(int day, int bis)
{
    QTranslator translator;
    QString     fichier = ":/translations/open-jardin_" + util::getLocale();

    if (!translator.load(fichier)) {
        qWarning() << "Impossible de charger la traduction:" << fichier;
    }
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

void Planning::ajouter_vignette_mois(QString titre, int colonne, int nb_jours, QColor couleur)
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

void Planning::ajouter_vignette_jour(QString titre, int colonne, int ligne, int width, int height, QColor couleur)
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

void Planning::ajouter_vignette_planning(int id_tache, QString titre, int ligne, int colonne, int nbCases, int forme,
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
        item->setMode(1);  //1 pas de déplacements possibles
        item->setTypeShape(forme);
        scene_planning->addItem(item);
    }
}

void Planning::ajouter_intitule_planning(int id_tache, QString titre, int ligne, int colonne, int nbCases, QColor couleur)
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

void Planning::ajouter_vignette_semaine(QString titre, int colonne, int ligne, int width, int height, QColor couleur)
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

void Planning::ajouter_vignette_horizontale(QString titre, int ligne, int width, int height, QColor couleur)
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

void Planning::ajouter_repere_jour(QString titre, int colonne, int ligne, int width, int height, QColor couleur)
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

void Planning::on_comboBox_AnneeEnCours_currentTextChanged(const QString&arg1)
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

void Planning::mise_a_jour_phases()
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

void Planning::maj_planning()
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
void Planning::on_comboBox_phases_currentTextChanged(const QString&arg1)
{
    Q_UNUSED(arg1)
    populate_treeview();
}

void Planning::populate_treeview()
{   // remplissage de l'arborescence de l'arbre
    set_drapEndExpanded(0);

    QString strDate = "01-01-" + ui->comboBox_AnneeEnCours->currentText();
    QDate   date    = QDate::fromString(strDate, "dd-MM-yyyy");


    //selection plante dans combobox
    QSqlQuery queryComboPhase;
    QString   parcelle;
    QString   designation_combo_phases = util::apos(ui->comboBox_phases->currentText());
    queryComboPhase.exec(QString("select id from parcelles where designation ='" + designation_combo_phases + "'"));
    if (queryComboPhase.first())
    {
        parcelle = queryComboPhase.value(0).toString();
    }
    else
    {
        qDebug() << "erreur query :" << queryComboPhase.lastError().text() << "  " <<
            queryComboPhase.lastError().databaseText() << queryComboPhase.lastQuery().toUtf8();
    }
    standardModel = new QStandardItemModel(this);
    QStandardItem *prtitre = new QStandardItem(designation_combo_phases);
    prtitre->setData(parcelle, RelationRoles::CodeRole);
    standardModel->invisibleRootItem()->appendRow(prtitre);
    QSqlQuery query(
        "SELECT id, designation, date_semis, duree,commentaires FROM cultures WHERE parcelle =" + parcelle +
        " ORDER BY parcelle ASC");
    const QSqlRecord rec = query.record();
    while (query.next())
    {
        QString designation = query.value(rec.indexOf("designation")).toString();
        int     precedent   = parcelle.toInt();
        int     id          = query.value(rec.indexOf("id")).toInt();

        QString strDepart   = query.value(rec.indexOf("date_semis")).toString();
        qint64  Duree       = query.value(rec.indexOf("duree")).toInt();
        QDate   date_depart = QDate::fromString(strDepart, "yyyy.MM.dd");
        QDate   date_fin    = date_depart.addDays(Duree);
        if (date_fin.year() == date.year() || date_depart.year() == date.year())
        {
            qDebug() << " designation parcelle id " << designation << " " << precedent << " " << id;
            QStandardItem *pr = new QStandardItem(designation);
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
    }
    if (!query.isActive())
    {
        qDebug() << "erreur query recherche cultures :" << query.lastError().text() << "  " <<
            query.lastError().databaseText() << query.lastQuery();
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

        QString idCulture = ui->treeView_taches->model()->data(modelIndex, RelationRoles::CodeRole).toString();
        rowTree++;
        qDebug() << "rowtree =" << rowTree << "idCulture " << idCulture;
        afficher_item_planning(rowTree, idCulture);
    }
    set_drapEndExpanded(1);
}

void Planning::resize_planning(int nb_taches)
{
    int hauteur = (nb_taches) * 28;

    scene_planning->setSceneRect(0, 0, 5205, hauteur);
    maj_planning();
}

void Planning::afficher_item_planning(const int position, const QString id_item)
{   // position = ligne    id_item = idculture
    if (position > 0)
    {
        int       decalagejour = 8;
        QString   strDate      = "01-01-" + ui->comboBox_AnneeEnCours->currentText();
        QDate     date         = QDate::fromString(strDate, "dd-MM-yyyy");
        QSqlQuery query;

        query.exec(QString("SELECT id, designation, date_semis, duree, type_plante FROM cultures  where id=" +
                           id_item));
        if (query.first())
        {
            QString strDesignation = query.value(1).toString();
            QString strDepart      = query.value(2).toString();
            QString strDuree       = query.value(3).toString();
            QString strPlante      = query.value(4).toString();
            QDate   date_depart    = QDate::fromString(strDepart, "yyyy.MM.dd");
            QString strType        = "2";
            double  jour           = date.daysTo(date_depart);
            double  duree          = strDuree.toDouble();
            qDebug() << " designation " << strDesignation << " " << id_item;
            QString couleur = recherche_couleur_famille(strPlante);

            /*  QString forme;
             * query.exec(QString("select couleur from type_de_tache where id=" + strType));
             * if (query.first())
             * {
             *    couleur = query.value(0).toString();
             * }
             * query.exec(QString("select forme from type_de_tache where id=" + strType));
             * if (query.first())
             * {
             *    forme = query.value(0).toString();
             * }*/
            int id_tache = id_item.toInt();
            qDebug() << " id_item " << id_item << " strDepart =" << strDepart << "date depart " << date_depart;
            // etiquettes horinzontales des taches
            qDebug() << " id_item " << id_item << " position =" << position << "jour " << jour << " duree " << duree;
            ajouter_vignette_planning(id_tache, strDesignation, position + 1, decalagejour + jour, duree + 1, 1, couleur); // ajout  7 colonnes
            //  ajouter_vignette_planning(id_tache, " ", position + 1, decalagejour + jour, duree + 1, forme.toInt(), couleur); // ajout  7 colonnes
        }
        ui->graphicsView_planning->verticalScrollBar()->setValue(1);
    }
}

QString Planning::recherche_couleur_famille(const QString id_plante)
{   // recherche la couleur de la famille de la plante
    QSqlQuery query;
    QString   num_espece;

    query.exec(QString("select espece from plantes where id =" + id_plante));
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
    QString couleur;
    query.exec(QString("select couleur from familles where id =" + num_famille));
    if (query.first())
    {
        couleur = query.value(0).toString();
    }
    return couleur;
}

void Planning::on_treeView_taches_collapsed(const QModelIndex&index)
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

void Planning::on_treeView_taches_expanded(const QModelIndex&index)
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

void Planning::effacer_item_planning()
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

void Planning::creer_lien_planning(int row_origin, int col_origin, int row_dest, int col_dest)
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

void Planning::remplir_liste_lien_planning()
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

void Planning::afficher_les_liens_planning()
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

void Planning::on_pushButton_ficheCulture_clicked()
{
    QSqlQuery queryComboPhase;
    QString   parcelle;
    QString   designation_combo_phases = util::apos(ui->comboBox_phases->currentText());

    queryComboPhase.exec(QString("select id from parcelles where designation ='" + designation_combo_phases + "'"));
    if (queryComboPhase.first())
    {
        parcelle = queryComboPhase.value(0).toString();
    }
    int Id = ui->lineEdit_id_culture->text().toInt();

    if (Id >= 0)
    {
        Cultures *fiche_Cultures = new Cultures(parcelle.toInt(), Id);
        fiche_Cultures->show();
    }
}

void Planning::ItemPlanning_clicked()
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
            tache_item *item = dynamic_cast <tache_item *> (itemList[i]);
            nbJours = (item->getWidth() / 14) + 1;
            ui->lineEdit_width->setText(QString::number(nbJours));
            dateDepart = (int(((item->pos().x()) / 14) - nbJours / 2)) - 6;
            QDate date_depart = dateAn.addDays(dateDepart);
            ui->lineEdit_dateDepart->setText(date_depart.toString("yyyy.MM.dd"));
            ui->lineEdit_id_culture->setText(QString::number(item->getId()));
            ui->lineEdit_texte_culture->setText(item->getNom());
        }
    }
}
