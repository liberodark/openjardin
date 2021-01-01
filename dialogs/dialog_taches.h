#ifndef DIALOG_TACHES_H
#define DIALOG_TACHES_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>
#include <QDebug>
#include <QWidget>

namespace Ui {
class Dialog_taches;
}

class Dialog_taches : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_taches(const int&IdTache, QWidget *parent = nullptr);
    ~Dialog_taches();

    void init_base();

    void setIdTache(const int&Idtache)
    {
        m_id_tache = Idtache;
    }

    int getIdtache()
    {
        return m_id_tache;
    }

    void calculer();

public slots:
    void getIdCulture(const QString&Idculture);
    void nouvelle_tache(const QString&PhaseParent);


private slots:
    void on_pushButton_nouveau_clicked();

    void on_pushButton_Supprimer_clicked();

    void on_pushButton_Valider_clicked();

    void on_pushButton_Annuler_clicked();

    void on_dateEdit_Depart_dateChanged(const QDate&date);

    void on_comboBox_Taches_currentIndexChanged(const QString&arg1);

    void on_spinBox_Avancement_valueChanged(int arg1);

    void on_pushButton_Edit_Type_clicked();

    void on_lineEdit_Duree_textChanged(const QString&arg1);

    void on_checkBox_ContrainteDate_stateChanged(int arg1);

    void on_comboBox_Type_currentIndexChanged(int index);

    void on_btnRessourceDelete_clicked();

    void on_btnRessourceEdit_clicked();

    void on_btnRessourcesAdd_clicked();

    void on_comboBox_Precedent_currentTextChanged(const QString&arg1);



    void on_checkBox_racine_stateChanged(int arg1);

    void on_checkBox_phase_stateChanged(int arg1);

    void on_tableView_Ressources_clicked(const QModelIndex&index);

    void on_comboBox_phasesCultures_currentIndexChanged(const QString&arg1);

    void on_lineEdit_Designation_textChanged(const QString&arg1);


private:
    Ui::Dialog_taches *ui;
    int m_id_tache;
};

#endif // DIALOG_TACHES_H
