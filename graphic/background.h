#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QGraphicsPixmapItem>

class background : public QGraphicsPixmapItem
{
public:

    background() : QGraphicsPixmapItem()
    {
    }

    ~background() override;
    void setPixFile(QString pixmapfileName);
    QString getPixFile();
    void setType(int Vtype);
    int getType();
    int getBgWidth();
    int getBgHeight();



signals:

protected:
    QPoint mousePressPt;

public slots:

private:
    QSize mBgSize;
    int mType;
    QString pixmapfileName;
};

#endif // BACKGROUND_H
