#ifndef MYTREEWIDGETITEM_H
#define MYTREEWIDGETITEM_H

#include <QTreeWidgetItem>


class mytreewidgetitem : public QTreeWidgetItem
{
public:
    enum { Type = QTreeWidgetItem::UserType + 1 };
    mytreewidgetitem() : QTreeWidgetItem(Type)
    {
    }

    mytreewidgetitem(QTreeWidget *parent);

    void setAncre(int column, const QString ancre)
    {
        m_ancre  = ancre;
        m_column = column;
    }

    QString getAncre(int column)
    {
        Q_UNUSED(column);
        return m_ancre;
    }

    QString m_ancre;
    int     m_column;

signals:

public slots:
};

#endif // MYTREEWIDGETITEM_H
