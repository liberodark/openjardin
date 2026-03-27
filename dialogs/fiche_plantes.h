#ifndef FICHE_PLANTES_H
#define FICHE_PLANTES_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>
namespace Ui {
class Fiche_plantes;
}

class Fiche_plantes : public QDialog
{
    Q_OBJECT

public:
    explicit Fiche_plantes(const int&IdPlante, QWidget *parent = nullptr);
    ~Fiche_plantes() override;

    void setIdPlante(const int&idPlante)
    {
        m_idPlante = idPlante;
    }

    int getIdPlante()
    {
        return m_idPlante;
    }

    void init_base();
    void mise_a_jour_tables_semis(const QString&printemps, const QString&ete, const QString&automne);
    void mise_a_jourFiche(int row);
    void test_tables_semis();

private slots:


    void on_tableView_plantes_clicked(const QModelIndex&index);

    void on_comboBox_especes_de_plantes_currentTextChanged(const QString&arg1);

    void on_pushButton_Modifier_plantes_clicked();

    void on_pushButton_enregistrerVariete_clicked();

    void on_pushButton_supprimer_plantes_clicked();

    void on_pushButton_Nouveau_plantes_clicked();

    void on_tableWidget_culture_printemps_cellClicked(int row, int column);

    void on_tableWidget_culture_automne_cellClicked(int row, int column);

    void on_pushButton_Modifier_especes_clicked();

    void on_pushButton_Supprimer_especes_clicked();

    void on_pushButton_Nouveau_especes_clicked();

    void on_pushButton_enregistrer_especes_clicked();

    void on_pushButton_Modifier_familles_clicked();

    void on_pushButton_Supprimer_familles_clicked();

    void on_pushButton_Nouveau_familles_clicked();

    void on_pushButton_enregistrer_familles_clicked();

    void on_tableView_familles_clicked(const QModelIndex&index);

    void on_tableView_especes_clicked(const QModelIndex&index);

    void on_tableWidget_rotation_cellClicked(int row, int column);

    void on_lineEdit_valeur_rotation_familles_textChanged(const QString&arg1);

    void on_lineEdit_valeur_rotation_familles_2_textChanged(const QString&arg1);

    void on_pushButton_fermer_clicked();

    void on_toolButton_clicked();



    void on_toolButton_CouleurFond_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::Fiche_plantes *ui;
    int m_idPlante;
    QString m_printemps = "000000000000000000000000";
    QString m_ete       = "000000000000000000000000";
    QString m_automne   = "000000000000000000000000";

    QSqlTableModel model1; //especes
    QSqlTableModel model2; //familles
    QSqlTableModel model3; //plantes
    QSqlTableModel modelF; //combobox familles
    QSqlTableModel modelE; //combobox especes
};

#endif // FICHE_PLANTES_H
