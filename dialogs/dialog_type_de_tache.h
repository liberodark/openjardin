#ifndef DIALOG_TYPE_DE_TACHE_H
#define DIALOG_TYPE_DE_TACHE_H

#include <QDialog>

namespace Ui {
class Dialog_type_de_tache;
}

class Dialog_type_de_tache : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_type_de_tache(const int&IdTypeTache, QWidget *parent = nullptr);
    ~Dialog_type_de_tache();
    void init_base();

    void setIdTypeDeTache(const int&IdTypedeTache)
    {
        m_id_TypedeTache = IdTypedeTache;
    }

    int getIdTypeDeTache()
    {
        return m_id_TypedeTache;
    }

private slots:


    void on_pushButton_nouveau_clicked();

    void on_pushButton_Supprimer_clicked();

    void on_pushButton_Valider_clicked();

    void on_pushButton_Annuler_clicked();

    void on_comboBox_Type_de_tache_currentIndexChanged(const QString&arg1);

    void on_toolButton_CouleurFond_clicked();

private:
    Ui::Dialog_type_de_tache *ui;
    int m_id_TypedeTache;
};

#endif // DIALOG_TYPE_DE_TACHE_H
