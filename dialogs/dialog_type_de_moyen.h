#ifndef DIALOG_TYPE_DE_MOYEN_H
#define DIALOG_TYPE_DE_MOYEN_H

#include <QDialog>

namespace Ui {
class Dialog_type_de_moyen;
}

class Dialog_type_de_moyen : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_type_de_moyen(const int&IdTypeMoyen, QWidget *parent = nullptr);
    ~Dialog_type_de_moyen();

    void init_base();

    void setIdtypeMoyen(const int&IdtypeMoyen)
    {
        m_id_typeMoyen = IdtypeMoyen;
    }

    int getIdtypeMoyen()
    {
        return m_id_typeMoyen;
    }

private slots:
    void on_pushButton_nouveau_clicked();

    void on_pushButton_Supprimer_clicked();

    void on_pushButton_Valider_clicked();

    void on_pushButton_Annuler_clicked();

private:
    Ui::Dialog_type_de_moyen *ui;
    int m_id_typeMoyen;
};

#endif // DIALOG_TYPE_DE_MOYEN_H
