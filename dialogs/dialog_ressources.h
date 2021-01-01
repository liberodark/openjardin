#ifndef DIALOG_RESSOURCES_H
#define DIALOG_RESSOURCES_H

#include <QDialog>

namespace Ui {
class Dialog_ressources;
}

class Dialog_ressources : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_ressources(const int&IdRessource, const int&IdTache, QWidget *parent = nullptr);
    ~Dialog_ressources();

    void init_base();


    void setIdRessource(const int&Idressource)
    {
        m_id_ressource = Idressource;
    }

    int getIdRessource()
    {
        return m_id_ressource;
    }

    void setIdTache(const int&Idtache)
    {
        m_id_tache = Idtache;
    }

    int getIdtache()
    {
        return m_id_tache;
    }

    void majModelListeMoyens(int id_ressource);

    void valider();
    void calculer();

private slots:

    void on_pushButton_nouveau_clicked();

    void on_pushButton_Annuler_clicked();

    void on_comboBox_Ressource_currentTextChanged(const QString&arg1);

    void on_toolButton_addMoyen_clicked();

    void on_toolButton_supprimeMoyen_clicked();

    void on_btnMoyenEdit_clicked();

    void on_tableView_moyens_alloues_clicked(const QModelIndex&index);

    void on_toolButton_validerQuantiteMoyen_clicked();

    void on_pushButton_ajout_ressource_clicked();

    void on_lineEdit_idRessource_textChanged(const QString&arg1);

    void on_pushButton_Supprimer_clicked();

private:
    Ui::Dialog_ressources *ui;
    int m_id_ressource;
    int m_id_tache;
};

#endif // DIALOG_RESSOURCES_H
