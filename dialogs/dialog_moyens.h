#ifndef DIALOG_MOYENS_H
#define DIALOG_MOYENS_H

#include <QDialog>

namespace Ui {
class Dialog_moyens;
}

class Dialog_moyens : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_moyens(const int&IdMoyen, QWidget *parent = nullptr);
    ~Dialog_moyens();

    void init_base();
    void vider_fiche();

    void setIdMoyen(const int&Idmoyen)
    {
        m_id_moyen = Idmoyen;
    }

    int getIdMoyen()
    {
        return m_id_moyen;
    }

private slots:
    void on_pushButton_nouveau_clicked();

    void on_pushButton_Supprimer_clicked();

    void on_pushButton_Valider_clicked();

    void on_pushButton_Annuler_clicked();

    void on_comboBox_Moyens_currentIndexChanged(const QString&arg1);



    void on_pushButton_Edit_coord_moyen_clicked();

    void on_pushButton_Edit_Type_clicked();

    void on_pushButton_Add_coord_moyen_clicked();

    void on_comboBox_coordonnees_currentIndexChanged(const QString&arg1);

    void on_comboBox_coordonnees_currentTextChanged(const QString&arg1);

    void on_comboBox_Type_moyen_currentTextChanged(const QString&arg1);

    void on_comboBox_unite_currentTextChanged(const QString&arg1);

    void on_toolButton_clicked();

private:
    Ui::Dialog_moyens *ui;
    int m_id_moyen;
};

#endif // DIALOG_MOYENS_H
