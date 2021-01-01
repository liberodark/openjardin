#include "mygraphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPoint>
#include <QtWidgets>
#include <QMenu>
#include "myitem.h"
#include "mygrilleitem.h"
#include "mypolygone.h"
#include "mypolyline.h"
#include "myvertex.h"
#include "consts.h"
#include "cote.h"
#include "sommet.h"

//MyGraphicsScene::MyGraphicsScene(QWidget *parent)
MyGraphicsScene::MyGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setMode(MyGraphicsScene::InsertSommet);
    gridSize = Consts::SPACE_SNAP;
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    clearListSommets();

    if (mouseEvent->button() == Qt::LeftButton)
    {
        if (myMode == InsertSommet)
        {
            Sommet *newSommet = new Sommet();
            int     positionX = int (mouseEvent->buttonDownScenePos(Qt::LeftButton).x());
            int     positionY = int (mouseEvent->buttonDownScenePos(Qt::LeftButton).y());
            qreal   xV        = ceil(positionX / gridSize) * gridSize;
            qreal   yV        = ceil(positionY / gridSize) * gridSize;
            newSommet->setPos(xV, yV);
            newSommet->setColor(QColor(Qt::yellow));
            newSommet->setPenColor(QColor(Qt::black));
            newSommet->setOpac(0.8);      //opacité de l'item
            addItem(newSommet);

            listSommets << newSommet;
            if (listSommets.size() > 1)
            {
                int     IdSource      = listSommets.size() - 2;
                int     IdDestination = listSommets.size() - 1;
                Sommet *source        = dynamic_cast <Sommet *>(listSommets[IdSource]);
                Sommet *destination   = dynamic_cast <Sommet *>(listSommets[IdDestination]);
                Cote *  NouveauLien   = new Cote(source, destination);
                addItem(NouveauLien);
            }
        }
    }

    if (mouseEvent->button() == Qt::MiddleButton)
    {
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    /*  QList <Sommet *> sommets;
     * foreach(QGraphicsItem * item, items())
     * {
     *    if (Sommet *sommet = qgraphicsitem_cast <Sommet *>(item))
     *    {
     *        if (sommet->isSelected())
     *        {
     *            QPoint posSommet = sommet->pos().toPoint();
     *            int    positionX = posSommet.x();
     *            int    positionY = posSommet.y();
     *            qreal  xV        = ceil(positionX / gridSize) * gridSize;
     *            qreal  yV        = ceil(positionY / gridSize) * gridSize;
     *            sommet->setPos(xV, yV);
     *        }
     *    }
     * }*/
    update();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::MiddleButton)
    {
    }
    update();
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MyGraphicsScene::clearListSommets()
{
    QList <Sommet *> sommets;
    foreach(QGraphicsItem * item, items())
    {
        if (Sommet *sommet = qgraphicsitem_cast <Sommet *>(item))
        {
            sommets << sommet;
        }
    }
    if (sommets.count() < 1)
    {
        listSommets.clear();
    }
}
