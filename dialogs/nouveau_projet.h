#ifndef NOUVEAU_PROJET_H
#define NOUVEAU_PROJET_H

#include <QDialog>

namespace Ui {
class nouveau_Projet;
}

class nouveau_Projet : public QDialog
{
    Q_OBJECT

public:
    explicit nouveau_Projet(QWidget *parent = nullptr);
    ~nouveau_Projet() override;

    void setXmlFilName(const QString&xmlFileName)
    {
        m_xmlFilName = xmlFileName;
    }

    QString getXmlFilName()
    {
        return m_xmlFilName;
    }

private slots:
    void on_toolButton_quitter_clicked();

    void on_toolButton_repertoire_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_filename_textChanged(const QString&arg1);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::nouveau_Projet *ui;
    QString m_xmlFilName;
};

#endif // NOUVEAU_PROJET_H
