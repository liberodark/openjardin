#ifndef DIALOG_AJOUT_RESSOURCE_H
#define DIALOG_AJOUT_RESSOURCE_H

#include <QDialog>

namespace Ui {
class Dialog_ajout_ressource;
}

class Dialog_ajout_ressource : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_ajout_ressource(const int&IdTache, QWidget *parent = nullptr);
    ~Dialog_ajout_ressource();

    void setIdTache(const int&Idtache)
    {
        m_id_tache = Idtache;
    }

    int getIdTache()
    {
        return m_id_tache;
    }

    QString apos(QString texte);

private slots:
    void on_buttonBox_accepted();

    void on_tableView_ressources_clicked(const QModelIndex&index);

private:
    Ui::Dialog_ajout_ressource *ui;
    int m_id_tache;
};

#endif // DIALOG_AJOUT_RESSOURCE_H
