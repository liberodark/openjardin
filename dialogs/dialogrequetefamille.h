#ifndef DIALOGREQUETEFAMILLE_H
#define DIALOGREQUETEFAMILLE_H

#include <QDialog>

namespace Ui {
class DialogRequeteFamille;
}

class DialogRequeteFamille : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRequeteFamille(const int&IdFamille,QWidget *parent = nullptr);
    ~DialogRequeteFamille();

    void setIdEspece(const QString &IdEspece)
    {
        m_id_Espece = IdEspece;
    }

    QString getIdEspece()
    {
        return m_id_Espece;
    }

private slots:
    void on_comboBoxFamille_currentTextChanged(const QString &arg1);

    void on_comboBoxEspece_currentTextChanged(const QString &arg1);

    void on_buttonBox_accepted();

private:
    Ui::DialogRequeteFamille *ui;
    QString m_id_Espece;
};

#endif // DIALOGREQUETEFAMILLE_H
