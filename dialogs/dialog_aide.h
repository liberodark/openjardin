#ifndef DIALOG_AIDE_H
#define DIALOG_AIDE_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "graphic/mytreewidgetitem.h"
namespace Ui {
class dialog_Aide;
}

class dialog_Aide : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_Aide(QWidget *parent = nullptr);
    ~dialog_Aide();

private slots:
    void populate(QStringList linkNames);

    void parse_html();

    void on_treeWidget_aide_itemClicked(QTreeWidgetItem *item, int column);


private:
    Ui::dialog_Aide *ui;
};

#endif // DIALOG_AIDE_H
