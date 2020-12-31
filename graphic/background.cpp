#include "background.h"


background::~background()
{
}

int background::getBgWidth()
{
    return mBgSize.width();
}

int background::getBgHeight()
{
    return mBgSize.height();
}

void background::setType(int Vtype)
{
    mType = Vtype;
}

int background::getType()
{
    return mType;
}

QString background::getPixFile()
{
    return pixmapfileName;
}

void background::setPixFile(QString file)
{
    pixmapfileName = file;
    QPixmap pixmap(file);
    setPixmap(pixmap);
    mBgSize = QSize(pixmap.width(), pixmap.height());
}
