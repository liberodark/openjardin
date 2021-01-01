#include "dialog_taches.h"
#include "ui_dialog_taches.h"
#include "mainwindow.h"
#include "dialog_ressources.h"
#include "dialog_type_de_tache.h"
#include "dialog_ajout_ressource.h"
#include "utilitaires/util.h"

Dialog_taches::Dialog_taches(const int&IdTache, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_taches)
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
    init_base();
}

Dialog_taches::~Dialog_taches()
{
    delete ui;
}

void Dialog_taches::on_lineEdit_Designation_textChanged(const QString&arg1)
{
    Q_UNUSED(arg1)
    if (ui->lineEdit_Designation->text() == "")
    {
        ui->lineEdit_Designation->setStyleSheet(
            "QLineEdit { background-color: rgb(255,255,217);border-width: 2px;border-style: solid;border-color: red }");
    }
    else
    {
        ui->lineEdit_Designation->setStyleSheet(
            "QLineEdit { background-color: rgb(255,255,217); }");
    }
}

void Dialog_taches::getIdCulture(const QString&Idculture)
{   // nouvelle phase culture à partir de la fiche cultures
    ui->lineEdit_IdCulture->setText(Idculture);
    QSqlQueryModel *modelCultures = new QSqlQueryModel;
    modelCultures->setQuery(
        "SELECT designation FROM cultures ORDER BY id ASC");
    ui->comboBox_Culture->setModel(modelCultures);
    //calage combo_type_tache sur valeur id
    QSqlQuery query;
    QString   designationCulture;
    QString   idParcelle;
    query.exec(QString("select designation, parcelle from cultures where id =" + Idculture));
    if (query.first())
    {
        designationCulture = query.value(0).toString();
        idParcelle         = query.value(1).toString();
        ui->comboBox_Culture->setCurrentIndex(ui->comboBox_Culture->findText(designationCulture));
        ui->lineEdit_IdParcelle->setText(idParcelle);
    }
    else
    {
        qWarning("ne peut récupérer la valeur designation culture et parcelle");
    }

    //Préparer les champs pour la création d'une nouvelle phase
    ui->lineEdit_idtache->setText("");
    ui->lineEdit_Designation->clear();
    ui->textEdit_Commentaires->clear();
    ui->lineEdit_Duree->setText("1");
    QDate currentDate = QDate::currentDate();
    ui->dateEdit_Depart->setDate(currentDate);
    ui->dateEdit_Fin->setDate(currentDate);
    ui->spinBox_Avancement->setValue(0);
    ui->checkBox_phase->setChecked(true);
    ui->checkBox_racine->setChecked(true);
    ui->checkBox_ContrainteDate->setChecked(false);
    ui->comboBox_Precedent->setCurrentIndex(ui->comboBox_Precedent->count() - 1);
    ui->comboBox_Precedent->setEnabled(false);
    ui->comboBox_Type->setCurrentIndex(0); //position sur phase
}

void Dialog_taches::on_pushButton_nouveau_clicked()
{
    nouvelle_tache(ui->comboBox_Phase_Appartenance->currentText());
}

void Dialog_taches::nouvelle_tache(const QString&PhaseParent)
{
    //vider les champs pour la création d'une nouvelle tâche
    ui->comboBox_Taches->setCurrentIndex(-1);
    ui->lineEdit_idtache->setText("");
    ui->lineEdit_Designation->clear();
    ui->textEdit_Commentaires->clear();
    ui->lineEdit_Duree->setText("1");
    QDate currentDate = QDate::currentDate();
    ui->dateEdit_Depart->setDate(currentDate);
    ui->dateEdit_Fin->setDate(currentDate);
    ui->spinBox_Avancement->setValue(0);
    ui->checkBox_phase->setChecked(false);
    ui->checkBox_racine->setChecked(false);
    ui->checkBox_ContrainteDate->setChecked(false);
    ui->comboBox_Type->setCurrentIndex(1);
    ui->comboBox_Phase_Appartenance->setEnabled(true);
    ui->comboBox_Phase_Appartenance->setCurrentText(PhaseParent);

    QSqlQuery query;
    QString   id_precedent;
    QString   id_PhaseParent;
    QString   id_Culture;
    query.exec(QString("SELECT id,id_culture FROM tasks WHERE designation ='" + util::apos(PhaseParent) + "'"));

    if (query.first())
    {
        id_PhaseParent = query.value(0).toString();
        id_Culture     = query.value(1).toString();
    }
    else
    {
        qWarning("ne peut récupérer la valeur id phase parent");
        qDebug() << "erreur query 1:" << query.lastError().text() << "  " << query.lastError().databaseText() <<
            query.driver() << query.lastQuery();
    }

    //remplissage du combobox selon les tâches liées à la phase parent
    QSqlQueryModel *modelTachesPhaseParent = new QSqlQueryModel;
    modelTachesPhaseParent->setQuery(
        "SELECT designation FROM tasks WHERE phase_parent = " + id_PhaseParent + " ORDER BY precedent ASC");
    ui->comboBox_Precedent->setModel(modelTachesPhaseParent);
    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.lastQuery();
    }


    query.exec(QString("SELECT MAX(precedent) FROM tasks WHERE phase_parent =" + id_PhaseParent));

    if (query.first())
    {
        id_precedent = query.value(0).toString();
        if (id_precedent == "0")
        {
            id_precedent = id_PhaseParent;
        }

        ui->lineEditIdTachePrecedente->setText(id_precedent);
    }
    else
    {
        qWarning("ne peut récupérer la valeur id precedent");
        qDebug() << "erreur query 1:" << query.lastError().text() << "  " << query.lastError().databaseText() <<
            query.driver() << query.lastQuery();
    }
    query.exec(QString("select designation from tasks where id=" + id_precedent));
    if (query.first())
    {
        QString designation_precedent = query.value(0).toString();
        ui->comboBox_Precedent->setCurrentText(designation_precedent);
    }
    else
    {
        qWarning("ne peut récupérer la valeur precedent");
        qDebug() << "erreur query 2:" << query.lastError().text() << "  " << query.lastError().databaseText() <<
            query.driver() << query.lastQuery();
    }

    QString designationCulture;
    QString idParcelle;

    query.exec(QString("select designation, parcelle from cultures where id =" + id_Culture));
    if (query.first())
    {
        designationCulture = query.value(0).toString();
        idParcelle         = query.value(1).toString();
        ui->comboBox_Culture->setCurrentIndex(ui->comboBox_Culture->findText(designationCulture));
        ui->lineEdit_IdParcelle->setText(idParcelle);
        ui->lineEdit_IdCulture->setText(id_Culture);
    }
    else
    {
        qWarning("ne peut récupérer la valeur designation culture et parcelle");
        qDebug() << "erreur query culture :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
            query.driver() << query.lastQuery();
    }
}

void Dialog_taches::on_comboBox_phasesCultures_currentIndexChanged(const QString&arg1)
{
    QSqlQuery query;
    QString   idPhase;

    query.exec(QString("select id from tasks where designation='" + util::apos(arg1) + "'"));
    if (query.first())
    {
        idPhase = query.value(0).toString();
    }
    QSqlQueryModel *modelTaches = new QSqlQueryModel;
    modelTaches->setQuery(
        "SELECT designation FROM tasks WHERE phase_parent = '" + idPhase + "'ORDER BY id ASC");
    ui->comboBox_Taches->setModel(modelTaches);
}

void Dialog_taches::init_base()
{   /***********************initialisation des bases de données sqlite******************/
    QSqlQuery query;

    QSqlQueryModel *modelPhases = new QSqlQueryModel;

    modelPhases->setQuery(
        "SELECT designation FROM tasks WHERE type = 1 ORDER BY id  ASC");
    ui->comboBox_Phase_Appartenance->setModel(modelPhases);
    ui->comboBox_phasesCultures->setModel(modelPhases);

    //calage du combobox phase de culture sur la phase parent de la tâche
    QString idTache = QString::number(getIdtache());
    QString phase_parent;
    QString designation_phase_parent;
    query.exec(QString("select phase_parent from tasks where id=" + idTache));
    if (query.first())
    {
        phase_parent = query.value(0).toString();
        query.exec(QString("select designation from tasks where id=" + phase_parent));
        if (query.first())
        {
            designation_phase_parent = query.value(0).toString();
            ui->comboBox_phasesCultures->setCurrentText(designation_phase_parent);
        }
    }
    else
    {
        qDebug() << "erreur query designation_phase_parent:" << query.lastError().text() << "  " <<
            query.lastError().databaseText() <<
            query.lastQuery().toUtf8();
    }

    QSqlQueryModel *modelTypedetaches = new QSqlQueryModel;
    modelTypedetaches->setQuery(
        "SELECT designation FROM type_de_tache ORDER BY id ASC");
    ui->comboBox_Type->setModel(modelTypedetaches);

    // remplir le listeview des ressources attachées à la tâche
    QSqlQueryModel *modelListeRessources = new QSqlQueryModel;

    modelListeRessources->setQuery(
        "SELECT id_ressource,designation FROM liste_ressources WHERE id_task=" + idTache + " ORDER BY id ASC ");
    ui->tableView_Ressources->setModel(modelListeRessources);


    QSqlQueryModel *modelCultures = new QSqlQueryModel;
    modelCultures->setQuery(
        "SELECT designation FROM cultures ORDER BY id ASC");
    ui->comboBox_Culture->setModel(modelCultures);


    QString designation;
    QString id_culture;
    query.exec(QString("select designation,id_culture from tasks where id =" + idTache));

    if (query.first())
    {
        designation = query.value(0).toString();
        id_culture  = query.value(1).toString();
        ui->lineEdit_Designation->setText(designation);
        ui->lineEdit_IdCulture->setText(id_culture);
    }
    ui->comboBox_Taches->setCurrentText(designation);
    QString designationCulture;
    QString idParcelle;
    query.exec(QString("select designation, parcelle from cultures where id =" + id_culture));
    if (query.first())
    {
        designationCulture = query.value(0).toString();
        idParcelle         = query.value(1).toString();
        ui->comboBox_Culture->setCurrentIndex(ui->comboBox_Culture->findText(designationCulture));
        ui->lineEdit_IdParcelle->setText(idParcelle);
    }
    else
    {
        qWarning("ne peut récupérer la valeur designation culture et parcelle");
    }
    ui->tableView_Ressources->setColumnWidth(0, 100);
    ui->tableView_Ressources->setColumnWidth(1, 350);
    calculer();
}

void Dialog_taches::on_comboBox_Taches_currentIndexChanged(const QString&arg1)
{
    QSqlQuery query;
    QString   id_tache;
    QString   commentaires;
    QString   depart;
    QString   fin;
    QString   duree;
    QString   precedent;
    QString   avancement;
    QString   projet;
    QString   type_tache;
    QString   phase_parent;
    QString   designation_phase_parent;
    QString   id_culture;
    QString   designation_culture;
    QString   idParcelle;

    ui->lineEdit_Designation->setText(arg1);
    query.exec(QString("select id from tasks where designation ='" + util::apos(arg1) + "'"));

    if (query.first())
    {
        id_tache = query.value(0).toString();
        ui->lineEdit_idtache->setText(id_tache);
        //commentaires
        query.exec(QString("select commentaires from tasks where id=" + id_tache));
        if (query.first())
        {
            commentaires = query.value(0).toString();
            ui->textEdit_Commentaires->setPlainText(commentaires);
        }
        else
        {
            qWarning("ne peut récupérer la valeur commentaires");
        }

        //date départ
        query.exec(QString("select depart from tasks where id=" + id_tache));
        if (query.first())
        {
            depart = query.value(0).toString();
            QDate date_depart = QDate::fromString(depart, "dd-MM-yyyy");
            ui->dateEdit_Depart->setDate(date_depart);
        }
        else
        {
            qWarning("ne peut récupérer la valeur depart");
        }
        //date fin
        query.exec(QString("select fin from tasks where id=" + id_tache));
        if (query.first())
        {
            fin = query.value(0).toString();
            QDate date_fin = QDate::fromString(fin, "dd-MM-yyyy");
            ui->dateEdit_Fin->setDate(date_fin);
        }
        else
        {
            qWarning("ne peut récupérer la valeur fin");
        }
        //durée
        query.exec(QString("select duree from tasks where id=" + id_tache));
        if (query.first())
        {
            duree = query.value(0).toString();
            ui->lineEdit_Duree->setText(duree);
        }
        else
        {
            qWarning("ne peut récupérer la valeur duree");
        }


        //phase parent
        query.exec(QString("select phase_parent from tasks where id=" + id_tache));
        if (query.first())
        {
            phase_parent = query.value(0).toString();



            query.exec(QString("select designation from tasks where id=" + phase_parent));
            if (query.first())
            {
                designation_phase_parent = query.value(0).toString();
                ui->comboBox_Phase_Appartenance->setCurrentText(designation_phase_parent);
            }
        }
        else
        {
            qWarning("ne peut récupérer la valeur phase_parent");
        }


        //precedent

        QSqlQueryModel *modelTachesprecedentes = new QSqlQueryModel;
        modelTachesprecedentes->setQuery(
            "SELECT designation FROM tasks WHERE phase_parent = '" + phase_parent + "'ORDER BY id ASC");
        ui->comboBox_Precedent->setModel(modelTachesprecedentes);

        query.exec(QString("select precedent from tasks where id=" + id_tache));
        if (query.first())
        {
            precedent = query.value(0).toString();

            if (precedent == "0")
            {
                ui->checkBox_racine->setChecked(true);
                ui->comboBox_Precedent->setEnabled(false);
                ui->comboBox_Precedent->setCurrentIndex(-1);
                ui->lineEditIdTachePrecedente->setText(precedent);
                ui->comboBox_Phase_Appartenance->setEnabled(false);
            }
            else
            {
                ui->checkBox_racine->setChecked(false);
                ui->comboBox_Precedent->setEnabled(true);
                ui->lineEditIdTachePrecedente->setText(precedent);
                ui->comboBox_Phase_Appartenance->setEnabled(true);
                query.exec(QString("select designation from tasks where id=" + precedent));
                if (query.first())
                {
                    QString designation_precedent = query.value(0).toString();
                    ui->comboBox_Precedent->setCurrentText(designation_precedent);
                }
            }
        }
        else
        {
            qWarning("ne peut récupérer la valeur precedent");
            qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver() << query.lastQuery();
        }
        //culture et parcelle
        query.exec(QString("select id_culture from tasks where id=" + id_tache));
        if (query.first())
        {
            id_culture = query.value(0).toString();
            query.exec(QString("select designation, parcelle from cultures where id=" + id_culture));
            if (query.first())
            {
                designation_culture = query.value(0).toString();
                idParcelle          = query.value(1).toString();
                ui->comboBox_Culture->setCurrentText(designation_culture);
                ui->lineEdit_IdParcelle->setText(idParcelle);
                ui->lineEdit_IdCulture->setText(id_culture);
            }
        }
        else
        {
            qWarning(" ne peut récupérer la valeur designation culture et parcelle");
        }



        //type de tâche
        query.exec(QString("select type from tasks where id=" + id_tache));
        if (query.first())
        {
            type_tache = query.value(0).toString();
            ui->comboBox_Type->setCurrentIndex(type_tache.toInt() - 1);
            ui->lineEdit_Type->setText(type_tache);
            if (type_tache == "1")
            {
                ui->checkBox_phase->setChecked(true);
            }
            else
            {
                ui->checkBox_phase->setChecked(false);
            }
        }
        else
        {
            qWarning("ne peut récupérer la valeur type");
        }
        //avancement
        query.exec(QString("select avancement from tasks where id=" + id_tache));
        if (query.first())
        {
            avancement = query.value(0).toString();
            ui->spinBox_Avancement->setValue(avancement.toInt());
            ui->progressBar_Avancement->setValue(avancement.toInt());
        }
        else
        {
            qWarning("ne peut récupérer la valeur avancement");
        }
        //contrainte de date
        query.exec(QString("select contrainte_date from tasks where id=" + id_tache));
        if (query.first())
        {
            int contrainte_date = query.value(0).toInt();
            if (contrainte_date == 1)
            {
                ui->checkBox_ContrainteDate->setChecked(true);
            }
            else
            {
                ui->checkBox_ContrainteDate->setChecked(false);
            }
        }
        else
        {
            qWarning("ne peut récupérer la valeur contrainte date");
        }
        //date de fin de la tâche précédente
        query.exec(QString("select fin from tasks where id='" + precedent + "'"));
        if (query.first())
        {
            QString datefin           = query.value(0).toString();
            QDate   date_finprecedent = QDate::fromString(datefin, "dd-MM-yyyy");
            ui->dateEdit_fin_precedente->setDate(date_finprecedent);
            if (ui->checkBox_ContrainteDate->isChecked())
            {
                ui->dateEdit_Depart->setMinimumDate(date_finprecedent);
            }
        }
        // remplir le listeview des ressources attachées à la tâche
        QSqlQueryModel *modelListeRessources = new QSqlQueryModel;
        modelListeRessources->setQuery(
            "SELECT id_ressource,designation FROM liste_ressources WHERE id_task=" + id_tache + " ORDER BY id ASC ");
        ui->tableView_Ressources->setModel(modelListeRessources);


        query.exec(QString("select designation from ressources where tache=" + id_tache));
        if (query.first())
        {
            QString resultat = query.value(0).toString();
            ui->lineEdit_designation_Ressource->setText(resultat);
        }
        else
        {
            qWarning(" ne peut récupérer la valeur tache dans ressources");
        }
    }
    else
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() << query.driver();
    }
    calculer();
}

void Dialog_taches::on_pushButton_Supprimer_clicked()
{
    //remplacement de l'id de la tache precedente de la tâche à supprimer dans les tâches qui la suivent
    QString   tacheActive  = ui->lineEdit_idtache->text();
    QString   designation  = util::apos(ui->comboBox_Taches->currentText());
    QString   id_precedent = ui->lineEditIdTachePrecedente->text();
    QString   id_tache     = ui->lineEdit_idtache->text();
    QSqlQuery query;



    if (id_tache.toInt() > 0)
    {
        int ret = QMessageBox::warning(this, tr("Suppression"),
                                       tr("Cette fiche peut être supprimer.\n"
                                          "Confirmer la suppression de la fiche?"),
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Cancel);

        switch (ret)
        {
        case QMessageBox::Ok:
        {
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
            init_base();
            break;
        }

        case QMessageBox::Cancel:
        {
            qDebug() << " Cancel";
            break;
        }

        default:
            break;
        }
    }
    else
    {
        QMessageBox::information(this, tr("Pas de tâche sélectionnée"),
                                 tr("Veuillez choisir une tâche avant de le supprimer svp !"));
    }
}

void Dialog_taches::on_pushButton_Valider_clicked()
{
    QString id_tache = ui->lineEdit_idtache->text();


    QSqlQuery query;
    QString   designation  = util::apos(ui->lineEdit_Designation->text());
    QString   commentaires = util::apos(ui->textEdit_Commentaires->document()->toPlainText());
    QString   depart       = ui->dateEdit_Depart->date().toString("dd-MM-yyyy");
    QString   fin          = ui->dateEdit_Fin->date().toString("dd-MM-yyyy");
    QString   duree        = ui->lineEdit_Duree->text();
    QString   id_culture   = ui->lineEdit_IdCulture->text();
    QString   precedent;
    QString   contrainte_date;
    QString   phase_parent = util::apos(ui->comboBox_Phase_Appartenance->currentText());
    QString   id_phase_parent;
    QString   avancement = QString::number(ui->spinBox_Avancement->value());
    QString   type       = QString::number(ui->comboBox_Type->currentIndex() + 1);

    query.exec(QString("select id from tasks where designation = '" + phase_parent + "'"));
    if (query.first())
    {
        id_phase_parent = query.value(0).toString();
    }
    else
    {
        qWarning("ne peut récupérer la valeur phase_parent");
    }

    if (ui->checkBox_racine->isChecked())
    {   //  nouvel enregistrement d'une phase de culture
        precedent = "0";
        type      = "1";
        if (ui->lineEdit_idtache->text() == "")
        {
            // trouver dernier valeur de id de tasks pour passer la valeur +1 dans id_phase_parent
            QSqlQuery queryIdMax;
            QString   IdMax;
            query.exec(QString("SELECT id FROM tasks WHERE id =(SELECT MAX(id) FROM tasks)"));
            if (query.first())
            {
                IdMax = query.value(0).toString();
            }

            id_phase_parent = QString::number(IdMax.toInt() + 1);
        }
    }
    else
    {
        precedent = ui->lineEditIdTachePrecedente->text();
    }

    if (ui->checkBox_ContrainteDate->isChecked())
    {
        contrainte_date = "1";
    }
    else
    {
        contrainte_date = "0";
    }



    if (ui->lineEdit_idtache->text() == "")
    {   //mode nouvel enregistrement
        QSqlQuery query;

        QString str =
            "insert into tasks (designation,commentaires, depart, fin, duree,precedent, avancement,type,contrainte_date,phase_parent,id_culture)"
            "values('" + designation + "','" + commentaires + "','" + depart + "','" + fin +
            "'," + duree + "," + precedent + "," + avancement + "," + type + "," + contrainte_date + "," + id_phase_parent + "," +
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
    }
    else
    {
        //mode modification

        QString strQuery = "update tasks set designation = '" + designation + "',commentaires='" + commentaires + "',depart= '" +
                           depart +
                           "',fin= '" + fin + "',duree=" + duree + ",precedent= " + precedent + ",avancement=" + avancement +
                           ",type=" + type + ",contrainte_date=" + contrainte_date + ",phase_parent=" + id_phase_parent +
                           ",id_culture=" + id_culture +
                           " where id=" + id_tache;
        query.exec(strQuery);


        if (!query.isActive())
        {
            qDebug() << "erreur query modifier:" << query.lastError().text() << "  " << query.lastError().databaseText() <<
                query.driver();
            QMessageBox::information(this, tr("Erreur d'enregistrement"),
                                     tr("Veuillez vérifier que tous les champs soient bien remplis"));
        }
        else
        {
            qDebug() << "enregistrement terminé";
        }
    }
    QSqlQueryModel *modelTaches = new QSqlQueryModel;

    modelTaches->setQuery(
        "SELECT id, designation, depart, fin, duree,commentaires,precedent, avancement,type,contrainte_date FROM tasks ORDER BY id ASC");
    accept();
    close();
}

void Dialog_taches::on_pushButton_Annuler_clicked()
{
    accept();
    close();
}

void Dialog_taches::on_dateEdit_Depart_dateChanged(const QDate&date)
{
    int   duree       = ui->lineEdit_Duree->text().toInt();
    QDate date_depart = date;
    QDate date_fin    = date_depart.addDays(duree - 1);

    ui->dateEdit_Fin->setDate(date_fin);
}

void Dialog_taches::on_lineEdit_Duree_textChanged(const QString&arg1)
{   //modification de la durée calcul de la date de fin
    int   duree       = arg1.toInt();
    QDate date_depart = ui->dateEdit_Depart->date();
    QDate date_fin    = date_depart.addDays(duree - 1);

    ui->dateEdit_Fin->setDate(date_fin);
}

void Dialog_taches::on_spinBox_Avancement_valueChanged(int arg1)
{
    ui->progressBar_Avancement->setValue(arg1);
}

void Dialog_taches::on_btnRessourcesAdd_clicked()
{   //Ajouter une ressource dans la liste
    int id_tache = ui->lineEdit_idtache->text().toInt();
    Dialog_ressources *FicheRessources = new Dialog_ressources(0, id_tache, this);
    int resultat = FicheRessources->exec();

    if (resultat == QDialog::Accepted)
    {
        QSqlQueryModel *modelListeRessource = new QSqlQueryModel;
        QString         id_Tache            = QString::number(getIdtache());
        modelListeRessource->setQuery(
            "SELECT id_ressource,designation FROM liste_ressources WHERE id_task= " + id_Tache +
            " ORDER BY id ASC");
        ui->tableView_Ressources->setModel(modelListeRessource);
    }

    /*  Dialog_ajout_ressource *FicheAjoutRessource = new Dialog_ajout_ressource(id_tache, this);
     * int resultat = FicheAjoutRessource->exec();
     *
     * if (resultat == QDialog::Accepted)
     * {
     *  QSqlQueryModel *modelListeRessource = new QSqlQueryModel;
     *  QString         id_Tache            = QString::number(getIdtache());
     *  modelListeRessource->setQuery(
     *      "SELECT id_ressource,designation FROM liste_ressources WHERE id_task= " + id_Tache +
     *      " ORDER BY id ASC");
     *  ui->tableView_Ressources->setModel(modelListeRessource);
     * }*/

    on_comboBox_Taches_currentIndexChanged(ui->comboBox_Taches->currentText());
}

void Dialog_taches::on_btnRessourceDelete_clicked()
{
    int       tacheActive = ui->lineEdit_idtache->text().toInt();
    QSqlQuery query;
    QString   id_ressource = ui->lineEdit_Id_Ressource->text();
    QString   str          = "delete from liste_ressources where id=" + id_ressource;

    query.exec(str);
    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
            query.driver() << query.lastQuery();
    }
    else
    {
        qDebug() << "Moyen supprimé avec succès";
    }
    init_base();
    ui->comboBox_Taches->setCurrentIndex(tacheActive - 1);
}

void Dialog_taches::on_btnRessourceEdit_clicked()
{
    int tacheActive  = ui->lineEdit_idtache->text().toInt();
    int id_ressource = ui->lineEdit_Id_Ressource->text().toInt();

    if (id_ressource > 0)
    {
        Dialog_ressources *FicheRessources = new Dialog_ressources(id_ressource, tacheActive, this);
        int resultat = FicheRessources->exec();

        if (resultat == QDialog::Accepted)
        {
            init_base();
            ui->comboBox_Taches->setCurrentIndex(tacheActive - 1);
        }
    }
    else
    {
        QMessageBox::information(this, tr("Pas de ressource sélectionnée"),
                                 tr("Veuillez choisir une ressource avant de l'éditer svp !"));
    }
}

void Dialog_taches::on_tableView_Ressources_clicked(const QModelIndex&index)
{
    int     row   = index.row();
    QString strId =
        ui->tableView_Ressources->model()->data(ui->tableView_Ressources->model()->index(row, 0)).toString();
    QString strDesignation =
        ui->tableView_Ressources->model()->data(ui->tableView_Ressources->model()->index(row, 1)).toString();

    ui->lineEdit_designation_Ressource->setText(strDesignation);
    ui->lineEdit_Id_Ressource->setText(strId);
}

void Dialog_taches::on_pushButton_Edit_Type_clicked()
{
    int typedeTache = ui->lineEdit_Type->text().toInt();
    Dialog_type_de_tache *FicheTypeTache = new Dialog_type_de_tache(typedeTache, this);
    int resultat = FicheTypeTache->exec();

    if (resultat == QDialog::Accepted)
    {
        init_base();
    }
}

void Dialog_taches::on_checkBox_ContrainteDate_stateChanged(int arg1)
{
    Q_UNUSED(arg1)
    if (ui->checkBox_ContrainteDate->isChecked())
    {
        QDate date_precedent = ui->dateEdit_fin_precedente->date();
        ui->dateEdit_Depart->setMinimumDate(date_precedent);
    }
    else
    {
        ui->dateEdit_Depart->clearMinimumDate();
    }
}

void Dialog_taches::on_comboBox_Precedent_currentTextChanged(const QString&arg1)
{
    QString   precedent;
    QSqlQuery query;

    query.exec(QString("select id from tasks where designation ='" + util::apos(arg1) + "'"));

    if (query.first())
    {
        precedent = query.value(0).toString();
        ui->lineEditIdTachePrecedente->setText(precedent);
    }
    else
    {
        qWarning("ne peut récupérer la valeur date fin précedent");
    }
    query.exec(QString("select fin from tasks where id='" + precedent + "'"));
    if (query.first())
    {
        precedent = query.value(0).toString();
        QDate date_precedent = QDate::fromString(precedent, "dd-MM-yyyy");
        ui->dateEdit_fin_precedente->setDate(date_precedent);
        if (ui->checkBox_ContrainteDate->isChecked())
        {
            ui->dateEdit_Depart->setMinimumDate(date_precedent);
        }
    }
    else
    {
        qWarning("ne peut récupérer la valeur date fin précedent");
    }
}

void Dialog_taches::on_comboBox_Type_currentIndexChanged(int index)
{
    ui->lineEdit_Type->setText(QString::number(index + 1));
}

void Dialog_taches::on_checkBox_racine_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->comboBox_Precedent->setEnabled(true);
    }
    else
    {
        ui->comboBox_Precedent->setEnabled(false);
    }
}

void Dialog_taches::on_checkBox_phase_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->comboBox_Type->setEnabled(true);
    }
    else
    {
        ui->comboBox_Type->setCurrentIndex(0);
        ui->comboBox_Type->setEnabled(false);
    }
}

void Dialog_taches::calculer()
{   // calculer le total actualisé
    QString   idtache = ui->lineEdit_idtache->text();
    QSqlQuery query("SELECT id_ressource FROM liste_ressources WHERE id_task =" + idtache);

    double  coutPrevu   = 0;
    double  coutRealise = 0;
    QString idRessource;

    while (query.next())
    {
        idRessource = query.value(0).toString();

        QSqlQuery query2("select cout_prevu,cout_actualise from ressources where id =" + idRessource);
        if (query2.first())
        {
            coutPrevu   = coutPrevu + (util::afficheZeros(query2.value(0).toString())).toDouble();
            coutRealise = coutRealise + (util::afficheZeros(query2.value(1).toString())).toDouble();
        }
        if (!query2.isActive())
        {
            qDebug() << "erreur query :" << query2.lastError().text() << "  " << query2.lastError().databaseText() <<
                query2.driver() << query2.lastQuery();
        }
        else
        {
            qDebug() << " query ok :" << query2.lastQuery() << coutPrevu;
        }
    }
    if (!query.isActive())
    {
        qDebug() << "erreur query :" << query.lastError().text() << "  " << query.lastError().databaseText() <<
            query.driver() << query.lastQuery();
    }
    else
    {
        qDebug() << " query ok :" << query.lastQuery();
    }


    ui->lineEdit_coutPrevu->setText(util::afficheZeros(QString::number(coutPrevu, 'f', 1)));
    ui->lineEdit_coutTotal->setText(util::afficheZeros(QString::number(coutRealise, 'f', 1)));
}
