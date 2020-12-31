#ifndef DIALOG_AJOUT_MOYEN_H
#define DIALOG_AJOUT_MOYEN_H

#include <QDialog>

namespace Ui {
class Dialog_ajout_Moyen;
}

class Dialog_ajout_Moyen : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_ajout_Moyen(const int&IdRessource, QWidget *parent = nullptr);
    ~Dialog_ajout_Moyen();

    void setIdRessource(const int&Idressource)
    {
        m_id_ressource = Idressource;
    }

    int getIdRessource()
    {
        return m_id_ressource;
    }

private slots:
    void on_buttonBox_accepted();

    void on_tableView_moyens_clicked(const QModelIndex&index);

    void on_pushButton_addMoyen_clicked();

    void on_toolButton_editMoyen_clicked();

private:
    Ui::Dialog_ajout_Moyen *ui;
    int m_id_ressource;
};

#endif // DIALOG_AJOUT_MOYEN_H
