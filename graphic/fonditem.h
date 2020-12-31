#ifndef FONDITEM_H
#define FONDITEM_H
#include <QGraphicsItem>

class FondItem : public QGraphicsPixmapItem
{
public:

    ~FondItem() override;
    FondItem()
    {
        //setCursor(Qt::OpenHandCursor);
        //setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
    }

    enum { Type = UserType + 4 };//65540

    int type() const override
    {
        return Type;
    }

    QString getPixFile()
    {
        return pixmapfileName;
    }

    void setPixFile(QString file)
    {
        pixmapfileName = file;
    }

protected:
    QPoint mousePressPt;


private:
    QString pixmapfileName;
};

#endif // FONDITEM_H
