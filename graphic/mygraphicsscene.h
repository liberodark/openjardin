#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "cote.h"
#include "sommet.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;

QT_END_NAMESPACE

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT


public:
    enum Mode { Utilisation, Modification, InsertSommet };

//  explicit MyGraphicsScene(QWidget *parent = 0);
    explicit MyGraphicsScene(QObject *parent = 0);
    void setMode(Mode mode)
    {
        myMode = mode;
    }

    void clearListSommets();

public slots:



signals:


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;


private:
    Mode myMode; //0 modification - 1 utilisation - 2 création polygone
    QList <Sommet *> listSommets;
    int gridSize;
};

#endif // MYGRAPHICSSCENE_H
