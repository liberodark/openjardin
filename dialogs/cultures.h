#ifndef CULTURES_H
#define CULTURES_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>
#include <QDebug>
#include <QWidget>
QT_BEGIN_NAMESPACE
class QComboBox;
class QDataWidgetMapper;
class QItemSelectionModel;
class QLabel;
class QLineEdit;
class QPushButton;
class QSqlRelationalTableModel;
class QStandardItemModel;
class QStringListModel;
class QTextEdit;
QT_END_NAMESPACE

namespace Ui {
class Cultures;
}

class Cultures : public QDialog
{
    Q_OBJECT

public:
    explicit Cultures(const int&IdItem, const int&IdCulture, QWidget *parent = 0);
    ~Cultures();

    void setIdItem(const int&idItem)
    {
        m_idItem = idItem;
    }

    int getIdItem()
    {
        return m_idItem;
    }

    void setIdCulture(const int&idCulture)
    {
        m_idCulture = idCulture;
    }

    int getIdCulture()
    {
        return m_idCulture;
    }

    void init_models(int idculture);
    void creer_phase();

private slots:
    void on_pushButton_validerData_clicked();

    void on_tableViewCultures_clicked(const QModelIndex&index);

    void on_lineEditTypeLune_textChanged(const QString&arg1);

    void on_pushButton_modifier_clicked();

    void on_pushButton_nouvelleOperation_clicked();

    void on_pushButton_print_fiche_clicked();

    void on_comboBox_plante_currentIndexChanged(const QString&arg1);

    void on_pushButton_creer_tache_clicked();

    void on_tableView_taches_clicked(const QModelIndex&index);

    void on_pushButton_modifier_tache_clicked();

    void on_lineEdit_id_cultures_textChanged(const QString&arg1);

    void on_pushButton_supprimer_culture_clicked();

    void on_pushButton_supprimer_tache_clicked();

    void on_lineEdit_duree_textChanged(const QString&arg1);



    void on_toolButton_FichePlantes_clicked();

    void on_toolButton_NouvellePlante_clicked();

    void on_pushButton_Gantt_clicked();

    void on_toolButton_clicked();

signals:
    void sendText(const QString&);


private:

    int m_idItem;
    int m_idCulture;
    Ui::Cultures *ui;
    QSqlTableModel model;
    QSqlTableModel modelAll;
    QSqlTableModel *modelM;
    QSqlTableModel *modelLune;
    QSqlQueryModel *modelL;
    QSqlQueryModel *modelObs;



    //  QSqlRelationalTableModel *modelM;
    QItemSelectionModel *selectionModel;
    QDataWidgetMapper *mapper;
};



#endif // CULTURES_H
