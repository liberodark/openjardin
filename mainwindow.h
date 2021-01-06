#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "modeles/parcelle.h"
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QLabel>
#include <QDomDocument>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);


    ~MainWindow();
    void loadIniSettings();

    void setGrille(const int&grille)
    {
        m_grille = grille;
    }

    int getGrille()
    {
        return m_grille;
    }

    void setFileNameXML(const QString fileName)
    {
        m_fileName_XML = fileName;
    }

    QString getFileNameXML()
    {
        return m_fileName_XML;
    }

    void setfileNameSQL(const QString fileName)
    {
        m_fileName_SQL = fileName;
    }

    QString getfileNameSQL()
    {
        return m_fileName_SQL;
    }

    void setFileNameBackGround(const QString fileNameBG)
    {
        m_fileNameBackGround = fileNameBG;
    }

    QString getFileNameBackGround()
    {
        return m_fileNameBackGround;
    }

    void setLocale(const QString locale)
    {
        m_locale = locale;
    }

    QString getLocale()
    {
        return m_locale;
    }

    void supprimerSommet();
    void ajouterSommet();
    void modifierPolygone();
    void validerPolygone();
    void validerPolyline();
    void ouvrir_FichierXML(QString fileName);
    int get_MaxId(); //retourne l'Id le plus grand
    void mise_a_jour_table_cultures(int choix);
    void createConnection(QString fileName);

    void ajouter_vignette_verticale(const QString titre, const int colonne, const int width, const int height,
                                    const QColor couleur);
    void ajouter_vignette_horizontale(const QString titre, const int ligne, const int width, const int height,
                                      const QColor couleur);
    void ajouter_vignette_planning(const int culture, const QString titre, const int ligne, const int colonne, const int nbCases,
                                   const QColor couleur, const QString etat_affichage);
    void ajouter_vignette_semaine(const QString titre, const int colonne, const int ligne, const int width, const int height,
                                  const QColor couleur);
    void ajouter_vignette_mois(const QString titre, const int colonne, const int nb_jours, const QColor couleur);
    void ajouter_vignette_jour(const QString titre, const int colonne, const int ligne, const int width, const int height,
                               const QColor couleur);
    void ajouter_repere_jour(const QString titre, const int colonne, const int ligne, const int width, const int height,
                             const QColor couleur);
    void ajouter_repere_jour_rotation(const QString titre, const int colonne, const int ligne, const int width, const int height,
                                      const QColor couleur);
    void affiche_planning(const int day, const int bis);
    void dessine_grille(const int espace, const qreal&Opacite);
    void ajouter_vignette_haut_rotation(const QString titre, const int ligne, const int colonne, const int nbCases,
                                        const QColor couleur);
    void ajouter_vignette_rotation(const int culture, const QString titre, const int ligne, const int colonne, const int nbCases,
                                   const QColor couleur, const QString etat_affichage);
    void ajouter_vignette_horiz_rotation(const QString titre, const int ligne, const int width, const int height,
                                         const QColor couleur);
    void affiche_rotation(const int year);
    void ajouter_repere_haut_rotation(const QString titre, const int ligne, const int colonne, const int nbCases,
                                      const QColor couleur);

    QPolygon convertStrToPoly(const QString MyString);
    void resize_planning(int nb_parcelles);
    void zoomGraphicsView(int ratio);
    void testVersion();
    void mise_a_jour_DB(QString sqlfileName );
    void backup_base();
    void mise_a_jour_Titre();
    void tester_table_parcelles();
    void remplir_table_parcelles();
    void ajouter_parcelle(QString id, QString designation);
    void supprimer_parcelle(QString id);

public slots:


private slots:

    void on_actionSauver_triggered();

    void on_actionOuvrir_triggered();

    void on_actionAjouterRectangle_triggered();

    void on_actionAjouterCercle_triggered();

    void on_actionFondEcran_triggered();

    void on_actionAjouterImage_triggered();

    void on_actionChoisir_la_couleur_triggered();

    void on_actionSupprimer_triggered();

    void on_actionDeselectionner_tout_triggered();

    void ShowContextMenu(const QPoint& pos);

    void on_actionAjouter_Rectangle_arrondi_triggered();

    void on_actionBringToFront_triggered();

    void on_actionA_propos_de_triggered();

    void on_actionQuitter_triggered();

    void on_actionChoisir_le_type_de_crayon_triggered();

    void on_actionModification_plan_triggered();

    void on_actionUtilisation_triggered();

    void Item_clicked();

    void ItemPlanning_clicked();

    void on_actionCacher_le_fond_triggered();

    void on_actionAfficher_le_fond_triggered();

    void on_comboBox_AnneeEnCours_currentIndexChanged(const QString&arg1);

    void on_pushButton_Affiches_fiche_clicked();

    void on_pushButton_Enregistrer_modif_item_clicked();

    void on_pushButton_print_clicked();

    void on_actionImprimer_le_plan_triggered();

    void on_comboBox_AnneeEnCours_2_currentIndexChanged(const QString&arg1);

    void on_pushButton_color_clicked();

    void on_actionAfficher_grille_triggered();

    void on_actionCacher_la_grille_triggered();

    void on_lineEdit_Nom_item_textChanged(const QString&arg1);

    void on_pushButton_recorPlan_clicked();

    void on_toolButton_modifierPolygon_toggled(bool checked);

    void on_toolButton_newPolygon_toggled(bool checked);

    void on_comboBox_typeLigne_P_currentIndexChanged(int index);

    void on_comboBox_epaisseurLignes_P_currentIndexChanged(const QString&arg1);

    void on_toolButton_InsertRectangle_clicked();

    void on_toolButton_InsertRoudedRectangle_clicked();

    void on_toolButton_InsertCircle_clicked();

    void on_toolButton_InsertImage_clicked();

    void on_toolButton_DeleteObjet_clicked();

    void on_toolButton_CouleurCrayon_clicked();

    void on_toolButton_CouleurFond_clicked();

    void on_pushButton_zoomIn_clicked();

    void on_pushButton_ZoomOut_clicked();

    void on_pushButton_fleche_clicked();

    void on_pushButton_Deplace_clicked();

    void on_comboBox_Etat_currentIndexChanged(int index);

    void on_actionAide_triggered();

    void on_pushButton_maj_planning_clicked();

    void on_toolButton_validerPolygone_clicked();

    void on_toolButton_ValiderPolyline_clicked();

    void on_pushButton_ficheCulture_clicked();

    void on_actionFiches_plantes_triggered();

    void on_actionConfiguration_triggered();

    void on_action_Licenses_triggered();

    void on_actionAfficher_Gantt_triggered();

    void on_actionOuvrir_fiche_taches_triggered();

    void on_actionOuvrir_fiche_ressources_triggered();

    void on_actionOuvrir_fiche_Moyens_triggered();

    void on_actionOuvrir_fiche_Coordonnees_triggered();

    void on_actionOuvrir_fiche_Type_de_Moyens_triggered();

    void on_pushButton_Detail_clicked();

    void on_actionEnregistrer_sous_triggered();



    void on_actionNouveau_projet_triggered();

    void on_actionAide_PDF_triggered();

    void on_actionAfficherPlanning_triggered();

    void on_toolButton_DeleteObjet_triggered(QAction *arg1);

    void on_pushButton_clicked();

    void on_actionA_propos_de_Qt_triggered();

    void on_actionLangue_Anglais_triggered();

    void on_actionTraduire_en_francais_triggered();


protected:
    QPoint mousePressPt;
    void mousePressEvent(QMouseEvent *event);
    void Choisir_la_couleur(int nCell);
    QString Choisir_le_fichier();
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *scene_planning;
    QGraphicsScene *scene_planning2;
    QGraphicsScene *scene_planning3;
    QGraphicsScene *scene_rotation;
    QGraphicsScene *scene_rotation2;
    QGraphicsScene *scene_rotation3;
    QString fondfileName;
    bool mode_modifier;
    int m_id;
    int m_MaxId;
    int m_grille;             //0 affiche - 1 cache
    int m_mode;
    qreal m_ZoomRatio;
    QString m_fileName_XML;
    QString m_fileName_SQL;
    QString m_fileNameBackGround;
    QString m_locale = "fr_FR";

    QSqlTableModel model1; //especes
    QSqlTableModel model2; //familles
    QSqlTableModel model3; //plantes
    QSqlTableModel model4; //taches
    QSqlTableModel modelF; //combobox familles
    QSqlTableModel modelE; //combobox especes
    QSqlTableModel model5; //toutes les taches
    QLabel *statusLabel;
};

/*********************CLASS PIXMAPITEM************************/
// QGraphicsPixmapItem subclass
class PixmapItem : public QGraphicsPixmapItem
{
public:

    PixmapItem() : QGraphicsPixmapItem()
    {
        setCursor(Qt::OpenHandCursor);
        setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
        setAcceptHoverEvents(true);
    }

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void setPixFile(QString pixmapfileName);
    QString getPixFile();
    void setTypeAction(int typeAction);
    int getTypeAction();
    void setAction(QString action);
    QString getAction();
    void setTexte(const QString text);
    QString getTexte();
    void setComment(const QString comment);
    QString getComment();

    void setEtat(const int&etat)
    {
        m_etat = etat;
    }

    int getEtat()
    {
        return m_etat;
    }

    void setMode(const int&mode)
    {
        m_mode = mode;
    }

    int getMode()
    {
        return m_mode;
    }

    void setId(const int&id)
    {
        m_pixmapId = id;
    }

    int getId()
    {
        return m_pixmapId;
    }

    void setNom(const QString nom)
    {
        itemNom = nom;
    }

    QString getNom()
    {
        return itemNom;
    }

protected:
    QPoint mousePressPt;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QString pixmapfileName;
    int typeAction;
    QString action;
    QString itemText;
    QString itemComment;
    int m_etat;
    int m_mode;
    int m_pixmapId;
    QString itemNom;
};
/********************CLASS FONDITEM************************/


// QGraphicsPixmapItem subclass
class FondItem : public QGraphicsPixmapItem
{
public:

    FondItem() : QGraphicsPixmapItem()
    {
        //setCursor(Qt::OpenHandCursor);
        //setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
    }

    void setPixFile(QString pixmapfileName);
    QString getPixFile();


protected:
    QPoint mousePressPt;


private:
    QString pixmapfileName;
};


/******************************************************/

#endif // MAINWINDOW_H
