#ifndef DIALOG_COORDONNEES_H
#define DIALOG_COORDONNEES_H

#include <QDialog>

namespace Ui {
class Dialog_coordonnees;
}

class Dialog_coordonnees : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_coordonnees(const int&IdCoord, QWidget *parent = nullptr);
    ~Dialog_coordonnees();

    void init_base();

    void setIdCoord(const int&Idcoord)
    {
        m_id_coord = Idcoord;
    }

    int getIdCoord()
    {
        return m_id_coord;
    }

private slots:

    void on_pushButton_Valider_clicked();

    void on_pushButton_nouveau_clicked();

    void on_pushButton_Supprimer_clicked();

    void on_pushButton_Annuler_clicked();

    void on_comboBox_societe_currentTextChanged(const QString&arg1);

private:
    Ui::Dialog_coordonnees *ui;
    int m_id_coord;
};

#endif // DIALOG_COORDONNEES_H
