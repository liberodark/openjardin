#ifndef LICENSES_H
#define LICENSES_H

#include <QDialog>

namespace Ui {
class Licenses;
}

class Licenses : public QDialog
{
    Q_OBJECT

public:
    explicit Licenses(QWidget *parent = nullptr);
    ~Licenses();

private:
    Ui::Licenses *ui;
};

#endif // LICENSES_H
