#ifndef PLANNING_H
#define PLANNING_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>
#include <QStandardItemModel>



namespace Ui {
class Planning;
}

class Planning : public QDialog
{
    Q_OBJECT

public:
    explicit Planning(const int&IdParcelle, QWidget *parent = nullptr);
    ~Planning();

    enum RelationRoles
    {
        CodeRole = Qt::UserRole + 1000,
    };
    struct tache
    {
        int     id;
        QString depart;
        QString fin;
        int     duree;
        int     phase_parent;
        int     type;
    };


    void affiche_planning(const int day, const int bis);
    void ajouter_vignette_planning(const int id_tache, const QString titre, const int ligne, const int colonne, const int nbCases,
                                   int forme, const QColor couleur);
    void ajouter_intitule_planning(const int id_tache, const QString titre, const int ligne, const int colonne, const int nbCases,
                                   const QColor couleur);
    void ajouter_vignette_mois(const QString titre, const int colonne, const int nb_jours, const QColor couleur);
    void ajouter_vignette_jour(const QString titre, const int colonne, const int ligne, const int width, const int height,
                               const QColor couleur);
    void ajouter_vignette_semaine(const QString titre, const int colonne, const int ligne, const int width, const int height,
                                  const QColor couleur);
    void ajouter_vignette_horizontale(const QString titre, const int ligne, const int width, const int height,
                                      const QColor couleur);
    void ajouter_repere_jour(const QString titre, const int colonne, const int ligne, const int width, const int height,
                             const QColor couleur);

    void createConnection(QString fileName);

    void init_base();

    void afficher_item_planning(const int position, const QString id_item);
    void maj_planning();

    void loadIniSettings();
    void effacer_item_planning();
    void resize_planning(int nb_taches);
    void creer_lien_planning(int row_origin, int col_origin, int row_dest, int col_dest);
    void remplir_liste_lien_planning();
    void afficher_les_liens_planning();
    void mise_a_jour_phases();

    QString recherche_couleur_famille(const QString id_plante);

    // position dans le treeview selon l'index
    void setListePosition(int row, int index)
    {
        ListPositions.append(qMakePair(int(row), int(index)));
    }

    int getListe_Position(int index)
    {
        int row = 0;

        for (int i = 0; i < ListPositions.count(); i++)
        {
            QPair <int, int> pair = ListPositions.at(i);
            if (pair.second == index)
            {
                row = pair.first;
            }
        }
        return row;
    }

    int getListe_Id(int row)
    {
        int index = 0;

        for (int i = 0; i < ListPositions.count(); i++)
        {
            QPair <int, int> pair = ListPositions.at(i);
            if (pair.first == row)
            {
                index = pair.second;
            }
        }
        return index;
    }

    void set_drapEndExpanded(int etat)
    {
        m_drapEndExpanded = etat;
    }

    int get_drapEndExpanded()
    {
        return m_drapEndExpanded;
    }

    int get_idparcelle()
    {
        return m_id_parcelle;
    }

    void set_idparcelle(int idparcelle)
    {
        m_id_parcelle = idparcelle;
    }

signals:
    void sendText(const QString&);

private slots:

    void on_comboBox_AnneeEnCours_currentIndexChanged(const QString&arg1);

    void populate_treeview();

    void on_treeView_taches_collapsed(const QModelIndex&index);

    void on_treeView_taches_expanded(const QModelIndex&index);

    void update_planningslider(int position);

    void on_comboBox_phases_currentTextChanged(const QString&arg1);

    void on_pushButton_ficheCulture_clicked();

    void ItemPlanning_clicked();

private:
    Ui::Planning *ui;
    QGraphicsScene *scene_planning;
    QGraphicsScene *scene_planning3;
    QStandardItemModel *standardModel;
    int m_id_parcelle;
    QList <QPair <int, int> > ListPositions;
    int m_drapEndExpanded;
};

#endif // PLANNING_H
