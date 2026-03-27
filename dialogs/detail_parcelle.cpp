#include "detail_parcelle.h"
#include "ui_detail_parcelle.h"
#include "graphic/parcelleitem.h"
#include "graphic/mygraphicsscene.h"
#include "graphic/background.h"
#include "dialogs/cultures.h"
#include "graphic/mypolygone.h"
#include "graphic/mypolyline.h"
#include "utilitaires/util.h"

#include <QDomDocument>
#include <QDir>
#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QScrollBar>
#include <QColorDialog>
#include <QUuid>

detail_parcelle::detail_parcelle(const int&IdParcelle, const QString&fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::detail_parcelle)
{
    // translator
    QTranslator translator;
    QString     fichier = ":/translations/open-jardin_" + util::getLocale();
    QSettings settings;
    QFile        iniFile(settings.fileName());
    if (iniFile.exists())
    {
        QSettings settings(iniFile.fileName(), QSettings::IniFormat);
        QString langue = settings.value("langue").toString();
        if(langue == "english")
            {
            // forcer la langue anglaise
                 fichier = ":/translations/open-jardin_en.ts";

             }


     }
    if (!translator.load(fichier)) {
        qWarning() << "Impossible de charger la traduction:" << fichier;
    }
    qApp->installTranslator(&translator);

    ui->setupUi(this);
    setFileNameXML(fileName); //nom du fichier du plan de détail
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1900, 1200);
    scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    ui->graphicsView->setScene(scene);
    // si la selection de l'item change lancer la commande
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(Item_clicked()));
    m_ZoomRatio = 1;  //variable du facteur de zoom (actual size =1 )
    ui->label_parcelle->setText(QString::number(IdParcelle));
    setIdParcelle(IdParcelle);
    if (fileName != "")
    {
        ouvrir_FichierXML(fileName);
    }
}

detail_parcelle::~detail_parcelle()
{
    delete ui;
}

void detail_parcelle::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    accept();
    close();
}

void detail_parcelle::ouvrir_FichierXML(QString fileName)
{
    QDomDocument document;
    QString      champ;
    QFile        nomFichier(fileName);

    parcelleList.clear();
    qDebug() << " fichier xml " << fileName;
    if (nomFichier.exists())
    {
        QFile file(nomFichier.fileName());
        setFileNameXML(fileName);  //nom du fichier du plan de détail
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("ERREUR DE LECTURE");
            return;
        }
        else
        {
            if (!document.setContent(&file))
            {
                //Erreur de lecture du fichier";
            }
            else
            {
                // Element root
                setFileNameXML(fileName);
                QDomElement  root  = document.firstChildElement();
                QDomNodeList base  = root.elementsByTagName("fichier_base");
                QDomNodeList fond  = root.elementsByTagName("fond");
                QDomNodeList objet = root.elementsByTagName("Objet");

                // écriture dans le tableau de l'objet fond d'écran
                QDomNode fondnode = fond.at(0);
                if (fondnode.isElement())
                {
                    // qDebug() << "boucle fond element";
                    QDomElement fond = fondnode.toElement();
                    setFileNameBackGround(fond.attribute("chemin"));
                    QString     fileName = getFileNameBackGround();
                    QPixmap     pim(fileName);
                    background *pixmap = new background();

                    scene->clear();
                    pixmap->setPixmap(pim);
                    scene->addItem(pixmap);
                    pixmap->setPixFile(fileName);
                    double sizeW = pixmap->boundingRect().width();
                    double sizeH = pixmap->boundingRect().height();
                    scene->setSceneRect(0, 0, sizeW, sizeH);
                }
                // boucle d'écriture de chaque objet dans le tableau
                for (int i = 0; i < objet.count(); i++)
                {
                    QDomNode objetnode = objet.at(i);
                    if (objetnode.isElement())
                    {
                        QDomElement objet = objetnode.toElement();
                        if (objet.attribute("Type").toInt() == 65536) //parcelleItem
                        {
                            QColor        couleurfond = QRgb(objet.attribute("Brush").toDouble());
                            QColor        couleurpen  = QRgb(objet.attribute("Pen").toDouble());
                            parcelleItem *item        = new parcelleItem(objet.attribute("SizeX").toInt(), objet.attribute(
                                                                             "SizeY").toInt());
                            QPen pen(Qt::blue, 1);
                            item->setColor(QColor(couleurfond));
                            item->setPenColor(QColor(Qt::black));
                            item->setOpac(0.8);
                            item->setPos(objet.attribute("PositionX").toInt(), objet.attribute("PositionY").toInt());
                            item->setTexte(objet.attribute("Texte"));
                            item->setTypeShape(objet.attribute("Forme").toInt());// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
                            item->setComment(objet.attribute("Commentaire"));
                            item->setPenColor(QColor(couleurpen));
                            item->setPixFile(objet.attribute("Chemin"));
                            item->setId(objet.attribute("Id").toInt());
                            item->setNom(objet.attribute("Nom"));
                            item->setEtat(objet.attribute("Etat").toInt());
                            item->setMode(objet.attribute("Mode").toInt());
                            item->setTypeLine(objet.attribute("TypeLine").toInt());
                            item->setWidthLine(objet.attribute("WidthLine").toInt());
                            item->setZValue(objet.attribute("ZValue").toDouble());
                            if (objet.attribute("Uuid") == "")
                            {
                                item->setUuid(QUuid::createUuid());
                            }
                            else
                            {
                                item->setUuid(QUuid::fromString(objet.attribute("Uuid")));
                            }
                            scene->addItem(item);
                        }
                        if (objet.attribute("Type").toInt() == 65537) // MyPolygone
                        {
                            MyPolygone *item = new MyPolygone(convertStrToPoly(objet.attribute("Chemin")));
                            QPen        pen(Qt::blue, 1);
                            QColor      couleurfond = QRgb(objet.attribute("Brush").toDouble());
                            QColor      couleurpen  = QRgb(objet.attribute("Pen").toDouble());
                            item->setColor(QColor(couleurfond));
                            item->setPenColor(QColor(Qt::black));
                            item->setOpac(0.8);
                            item->setPos(objet.attribute("PositionX").toInt(), objet.attribute("PositionY").toInt());
                            item->setTexte(objet.attribute("Texte"));
                            item->setTypeShape(objet.attribute("Forme").toInt());// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
                            item->setComment(objet.attribute("Commentaire"));
                            item->setPenColor(QColor(couleurpen));
                            item->setStrPoly(objet.attribute("Chemin"));
                            item->setId(objet.attribute("Id").toInt());
                            item->setNom(objet.attribute("Nom"));
                            item->setEtat(objet.attribute("Etat").toInt());
                            item->setMode(objet.attribute("Mode").toInt());
                            item->setTypeLine(objet.attribute("TypeLine").toInt());
                            item->setWidthLine(objet.attribute("WidthLine").toInt());
                            item->setZValue(objet.attribute("ZValue").toDouble());
                            item->setUuid(QUuid::fromString(objet.attribute("Uuid")));
                            if (objet.attribute("Uuid") == "")
                            {
                                item->setUuid(QUuid::createUuid());
                            }
                            else
                            {
                                item->setUuid(QUuid::fromString(objet.attribute("Uuid")));
                            }
                            scene->addItem(item);
                        }
                        if (objet.attribute("Type").toInt() == 65539) // MyPolyline
                        {
                            MyPolyline *item = new MyPolyline(convertStrToPoly(objet.attribute("Chemin")));
                            QPen        pen(Qt::blue, 1);
                            QColor      couleurfond = QRgb(objet.attribute("Brush").toDouble());
                            QColor      couleurpen  = QRgb(objet.attribute("Pen").toDouble());
                            item->setColor(QColor(couleurfond));
                            item->setPenColor(QColor(Qt::black));
                            item->setOpac(0.8);
                            item->setPos(objet.attribute("PositionX").toInt(), objet.attribute("PositionY").toInt());
                            item->setTexte(objet.attribute("Texte"));
                            item->setTypeShape(objet.attribute("Forme").toInt());// type de shape 1 rectangle - 2 rectangle  rounded - 3 circle
                            item->setComment(objet.attribute("Commentaire"));
                            item->setPenColor(QColor(couleurpen));
                            item->setStrPoly(objet.attribute("Chemin"));
                            item->setId(objet.attribute("Id").toInt());
                            item->setNom(objet.attribute("Nom"));
                            item->setEtat(objet.attribute("Etat").toInt());
                            item->setMode(objet.attribute("Mode").toInt());
                            item->setTypeLine(objet.attribute("TypeLine").toInt());
                            item->setWidthLine(objet.attribute("WidthLine").toInt());
                            item->setZValue(objet.attribute("ZValue").toDouble());
                            if (objet.attribute("Uuid") == "")
                            {
                                item->setUuid(QUuid::createUuid());
                            }
                            else
                            {
                                item->setUuid(QUuid::fromString(objet.attribute("Uuid")));
                            }
                            scene->addItem(item);
                        }
                    }
                }
            }
        }
        file.close();
    }
    on_toolButtonUtilisation_clicked();
}

void detail_parcelle::on_pushButton_ouvrir_clicked()
{
    // ouverture du fichier pour lecture
    QString repertoire = QDir::homePath() + "/openjardin/";

    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Ouverture du fichier des objets"),
                                     repertoire,
                                     tr("XML Files (*.xml);; All files (*.*)"));

    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        ouvrir_FichierXML(fileName);
    }
}

QPolygon detail_parcelle::convertStrToPoly(const QString MyString)
{
    //conversion d'une chaine de caractères avec séparateur des QPoints avec ; en QPolygon

    QStringList liste = MyString.split(';', Qt::SkipEmptyParts);
    QPolygon    poly(liste.count());

    for (int i = 0; i < liste.count(); i++)
    {
        QString     valeur_point = liste[i];
        QStringList liste_point  = valeur_point.split(',', Qt::SkipEmptyParts);
        QString     strX         = liste_point[0];
        QString     strY         = liste_point[1];
        int         x            = strX.toInt();
        int         y            = strY.toInt();
        poly.setPoint(i, x, y);
    }
    return poly;
}

void detail_parcelle::Item_clicked()
{   //si l'item est sélectionné (selection changed)
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536) // parcelleItem
            {
                parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
                ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Nom_item->setText(item->getNom());
                ui->textEdit_plan_commentaires->setText(item->getTexte());

                if (item->getTypeShape() == (parcelleItem::Rectangle))
                {
                    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                }
                if (item->getTypeShape() == (parcelleItem::RoundedRectangle))
                {
                    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                }
                if (item->getTypeShape() == (parcelleItem::Circle))
                {
                    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                }
            }
            if (itemList[i]->type() == 65537) // MyPolygone
            {
                MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Nom_item->setText(item->getNom());
                ui->textEdit_plan_commentaires->setText(item->getTexte());
            }

            if (itemList[i]->type() == 65539) // MyPolyline
            {
                MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                //    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
                ui->lineEdit_Nom_item->setText(item->getNom());
                ui->textEdit_plan_commentaires->setText(item->getTexte());
            }
        }
    }
}

void detail_parcelle::on_toolButton_save_as_clicked()
{
    //sauvegarder le plan
    QString cellvalue;
    QString champ;

    // XML
    QDomDocument document;
    QDomNode     xmlNode = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

    document.insertBefore(xmlNode, document.firstChild());
    QDomElement root = document.createElement("root");
    document.appendChild(root);
    QDomElement base = document.createElement("base");
    root.appendChild(base);
    QDomElement background = document.createElement("background");
    root.appendChild(background);
    QDomElement objets = document.createElement("objets");
    root.appendChild(objets);
    QDomElement fichier_base = document.createElement("fichier_base");
    cellvalue = getfileNameSQL();
    champ     = "fichier";
    fichier_base.setAttribute(champ, cellvalue);
    base.appendChild(fichier_base);
    //background
    QDomElement fond = document.createElement("fond");

    fond.setAttribute("chemin", getFileNameBackGround());
    fond.setAttribute("Type", "7");
    fond.setAttribute("PositionX", "0");
    fond.setAttribute("PositionY", "0");
    background.appendChild(fond);

    //Ecriture des objets
    QString ObjetId;
    QString ObjetNom;
    QString ObjetType;
    QString ObjetForme;
    QString ObjetPosX;
    QString ObjetPosY;
    QString ObjetWidth;
    QString Objetheight;
    QString ObjetBrush;
    QString ObjetPen;
    QString ObjetComment;
    QString ObjetTexte;
    QString ObjetChemin;
    QString ObjetEtat;
    QString ObjetMode;
    QString ObjetTypeLine;
    QString ObjetWidthLine;
    QString ObjetZValue;
    QString ObjetUuid;

    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = itemList.size() - 1; i >= 0; i--)
    //for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->pos().x() > 0)
        {
            QDomElement objet = document.createElement("Objet");

            if (itemList[i]->type() == 65536)       // MyItem
            {
                parcelleItem *item     = dynamic_cast <parcelleItem *> (itemList[i]);
                QPointF       location = itemList[i]->pos();
                int           valX     = int(location.x());
                int           valY     = int(location.y());
                int           sizeW    = int(itemList[i]->boundingRect().width());
                int           sizeH    = int(itemList[i]->boundingRect().height());
                QColor        clr      = item->getColor();
                QColor        clrPen   = item->getPenColor();

                ObjetId        = QString::number(item->getId());
                ObjetNom       = item->getNom();
                ObjetType      = QString::number(65536);
                ObjetForme     = QString::number(item->getTypeShape());
                ObjetPosX      = QString::number(valX);
                ObjetPosY      = QString::number(valY);
                ObjetWidth     = QString::number(sizeW);
                Objetheight    = QString::number(sizeH);
                ObjetBrush     = QString::number(clr.rgb());
                ObjetPen       = QString::number(clrPen.rgb());
                ObjetComment   = item->getComment();
                ObjetTexte     = item->getTexte();
                ObjetChemin    = item->getPixFile();
                ObjetEtat      = QString::number(item->getEtat());
                ObjetMode      = QString::number(item->getMode());
                ObjetTypeLine  = QString::number(item->getTypeLine());
                ObjetWidthLine = QString::number(item->getWidthLine());
                ObjetZValue    = QString::number(item->zValue());
                ObjetUuid      = item->getUuid().toString();
            }
            if (itemList[i]->type() == 65537)       // MyPolygone
            {
                MyPolygone *item     = dynamic_cast <MyPolygone *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjetId        = QString::number(item->getId());
                ObjetNom       = item->getNom();
                ObjetType      = QString::number(65537);
                ObjetForme     = QString::number(item->getTypeShape());
                ObjetPosX      = QString::number(valX);
                ObjetPosY      = QString::number(valY);
                ObjetWidth     = QString::number(sizeW);
                Objetheight    = QString::number(sizeH);
                ObjetBrush     = QString::number(clr.rgb());
                ObjetPen       = QString::number(clrPen.rgb());
                ObjetComment   = item->getComment();
                ObjetTexte     = item->getTexte();
                ObjetChemin    = item->getStrPoly();
                ObjetEtat      = QString::number(item->getEtat());
                ObjetMode      = QString::number(item->getMode());
                ObjetTypeLine  = QString::number(item->getTypeLine());
                ObjetWidthLine = QString::number(item->getWidthLine());
                ObjetZValue    = QString::number(item->zValue());
                ObjetUuid      = item->getUuid().toString();
            }
            if (itemList[i]->type() == 65539)       // MyPolyline
            {
                MyPolyline *item     = dynamic_cast <MyPolyline *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjetId        = QString::number(item->getId());
                ObjetNom       = item->getNom();
                ObjetType      = QString::number(65539);
                ObjetForme     = QString::number(item->getTypeShape());
                ObjetPosX      = QString::number(valX);
                ObjetPosY      = QString::number(valY);
                ObjetWidth     = QString::number(sizeW);
                Objetheight    = QString::number(sizeH);
                ObjetBrush     = QString::number(clr.rgb());
                ObjetPen       = QString::number(clrPen.rgb());
                ObjetComment   = item->getComment();
                ObjetTexte     = item->getTexte();
                ObjetChemin    = item->getStrPoly();
                ObjetEtat      = QString::number(item->getEtat());
                ObjetMode      = QString::number(item->getMode());
                ObjetTypeLine  = QString::number(item->getTypeLine());
                ObjetWidthLine = QString::number(item->getWidthLine());
                ObjetZValue    = QString::number(item->zValue());
                ObjetUuid      = item->getUuid().toString();
            }



            objet.setAttribute("Id", ObjetId);
            objet.setAttribute("Nom", ObjetNom);
            objet.setAttribute("Type", ObjetType);
            objet.setAttribute("PositionX", ObjetPosX);
            objet.setAttribute("PositionY", ObjetPosY);
            objet.setAttribute("SizeX", ObjetWidth);
            objet.setAttribute("SizeY", Objetheight);
            objet.setAttribute("Brush", ObjetBrush);
            objet.setAttribute("Pen", ObjetPen);
            objet.setAttribute("Forme", ObjetForme);
            objet.setAttribute("Commentaire", ObjetComment);
            objet.setAttribute("Texte", ObjetTexte);
            objet.setAttribute("Chemin", ObjetChemin);
            objet.setAttribute("Mode", ObjetMode);
            objet.setAttribute("Etat", ObjetEtat);
            objet.setAttribute("TypeLine", ObjetTypeLine);
            objet.setAttribute("WidthLine", ObjetWidthLine);
            objet.setAttribute("ZValue", ObjetZValue);
            objet.setAttribute("Uuid", ObjetUuid);
            objets.appendChild(objet);
        }
    }
    // Ecriture dans le fichier

    QFileInfo FichierXML(getFileNameXML());
    QDir      repertoireXML = FichierXML.absoluteDir();
    QString   repertoire    = QDir::homePath() + "/openjardin/";
    QString   fileName      =
        QFileDialog::getSaveFileName(this, tr("Sauvegarde du plan de détail"),
                                     repertoire,
                                     tr("XML Files (*.xml)"));
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ERREUR D'ÉCRITURE");
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
        setFileNameXML(fileName);
    }
    close();
}

void detail_parcelle::on_toolButton_Sauver_clicked()
{   //sauvegarder le plan
    QString cellvalue;
    QString champ;

    // XML
    QDomDocument document;
    QDomNode     xmlNode = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

    document.insertBefore(xmlNode, document.firstChild());
    QDomElement root = document.createElement("root");
    document.appendChild(root);
    QDomElement base = document.createElement("base");
    root.appendChild(base);
    QDomElement background = document.createElement("background");
    root.appendChild(background);
    QDomElement objets = document.createElement("objets");
    root.appendChild(objets);
    QDomElement fichier_base = document.createElement("fichier_base");
    cellvalue = getfileNameSQL();
    champ     = "fichier";
    fichier_base.setAttribute(champ, cellvalue);
    base.appendChild(fichier_base);
    //background
    QDomElement fond = document.createElement("fond");

    fond.setAttribute("chemin", getFileNameBackGround());
    fond.setAttribute("Type", "7");
    fond.setAttribute("PositionX", "0");
    fond.setAttribute("PositionY", "0");
    background.appendChild(fond);

//Ecriture des objets
    QString ObjetId;
    QString ObjetNom;
    QString ObjetType;
    QString ObjetForme;
    QString ObjetPosX;
    QString ObjetPosY;
    QString ObjetWidth;
    QString Objetheight;
    QString ObjetBrush;
    QString ObjetPen;
    QString ObjetComment;
    QString ObjetTexte;
    QString ObjetChemin;
    QString ObjetEtat;
    QString ObjetMode;
    QString ObjetTypeLine;
    QString ObjetWidthLine;
    QString ObjetZValue;
    QString ObjetUuid;

    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = itemList.size() - 1; i >= 0; i--)
    //for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->pos().x() > 0)
        {
            QDomElement objet = document.createElement("Objet");

            if (itemList[i]->type() == 65536)   // MyItem
            {
                parcelleItem *item     = dynamic_cast <parcelleItem *> (itemList[i]);
                QPointF       location = itemList[i]->pos();
                int           valX     = int(location.x());
                int           valY     = int(location.y());
                int           sizeW    = int(itemList[i]->boundingRect().width());
                int           sizeH    = int(itemList[i]->boundingRect().height());
                QColor        clr      = item->getColor();
                QColor        clrPen   = item->getPenColor();

                ObjetId        = QString::number(item->getId());
                ObjetNom       = item->getNom();
                ObjetType      = QString::number(65536);
                ObjetForme     = QString::number(item->getTypeShape());
                ObjetPosX      = QString::number(valX);
                ObjetPosY      = QString::number(valY);
                ObjetWidth     = QString::number(sizeW);
                Objetheight    = QString::number(sizeH);
                ObjetBrush     = QString::number(clr.rgb());
                ObjetPen       = QString::number(clrPen.rgb());
                ObjetComment   = item->getComment();
                ObjetTexte     = item->getTexte();
                ObjetChemin    = item->getPixFile();
                ObjetEtat      = QString::number(item->getEtat());
                ObjetMode      = QString::number(item->getMode());
                ObjetTypeLine  = QString::number(item->getTypeLine());
                ObjetWidthLine = QString::number(item->getWidthLine());
                ObjetZValue    = QString::number(item->zValue());
                ObjetUuid      = item->getUuid().toString();
            }
            if (itemList[i]->type() == 65537)   // MyPolygone
            {
                MyPolygone *item     = dynamic_cast <MyPolygone *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjetId        = QString::number(item->getId());
                ObjetNom       = item->getNom();
                ObjetType      = QString::number(65537);
                ObjetForme     = QString::number(item->getTypeShape());
                ObjetPosX      = QString::number(valX);
                ObjetPosY      = QString::number(valY);
                ObjetWidth     = QString::number(sizeW);
                Objetheight    = QString::number(sizeH);
                ObjetBrush     = QString::number(clr.rgb());
                ObjetPen       = QString::number(clrPen.rgb());
                ObjetComment   = item->getComment();
                ObjetTexte     = item->getTexte();
                ObjetChemin    = item->getStrPoly();
                ObjetEtat      = QString::number(item->getEtat());
                ObjetMode      = QString::number(item->getMode());
                ObjetTypeLine  = QString::number(item->getTypeLine());
                ObjetWidthLine = QString::number(item->getWidthLine());
                ObjetZValue    = QString::number(item->zValue());
                ObjetUuid      = item->getUuid().toString();
            }
            if (itemList[i]->type() == 65539)   // MyPolyline
            {
                MyPolyline *item     = dynamic_cast <MyPolyline *> (itemList[i]);
                QPointF     location = itemList[i]->pos();
                int         valX     = int(location.x());
                int         valY     = int(location.y());
                int         sizeW    = int(itemList[i]->boundingRect().width());
                int         sizeH    = int(itemList[i]->boundingRect().height());
                QColor      clr      = item->getColor();
                QColor      clrPen   = item->getPenColor();

                ObjetId        = QString::number(item->getId());
                ObjetNom       = item->getNom();
                ObjetType      = QString::number(65539);
                ObjetForme     = QString::number(item->getTypeShape());
                ObjetPosX      = QString::number(valX);
                ObjetPosY      = QString::number(valY);
                ObjetWidth     = QString::number(sizeW);
                Objetheight    = QString::number(sizeH);
                ObjetBrush     = QString::number(clr.rgb());
                ObjetPen       = QString::number(clrPen.rgb());
                ObjetComment   = item->getComment();
                ObjetTexte     = item->getTexte();
                ObjetChemin    = item->getStrPoly();
                ObjetEtat      = QString::number(item->getEtat());
                ObjetMode      = QString::number(item->getMode());
                ObjetTypeLine  = QString::number(item->getTypeLine());
                ObjetWidthLine = QString::number(item->getWidthLine());
                ObjetZValue    = QString::number(item->zValue());
                ObjetUuid      = item->getUuid().toString();
            }



            objet.setAttribute("Id", ObjetId);
            objet.setAttribute("Nom", ObjetNom);
            objet.setAttribute("Type", ObjetType);
            objet.setAttribute("PositionX", ObjetPosX);
            objet.setAttribute("PositionY", ObjetPosY);
            objet.setAttribute("SizeX", ObjetWidth);
            objet.setAttribute("SizeY", Objetheight);
            objet.setAttribute("Brush", ObjetBrush);
            objet.setAttribute("Pen", ObjetPen);
            objet.setAttribute("Forme", ObjetForme);
            objet.setAttribute("Commentaire", ObjetComment);
            objet.setAttribute("Texte", ObjetTexte);
            objet.setAttribute("Chemin", ObjetChemin);
            objet.setAttribute("Mode", ObjetMode);
            objet.setAttribute("Etat", ObjetEtat);
            objet.setAttribute("TypeLine", ObjetTypeLine);
            objet.setAttribute("WidthLine", ObjetWidthLine);
            objet.setAttribute("ZValue", ObjetZValue);
            objet.setAttribute("Uuid", ObjetUuid);
            objets.appendChild(objet);
        }
    }
    // Ecriture dans le fichier


    QString fileName = getFileNameXML();

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ERREUR D'ÉCRITURE");
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
        setFileNameXML(fileName);
    }
    close();
}

void detail_parcelle::on_toolButton_AjoutRectangle_clicked()
{   //créer un rectangle
    parcelleItem *item = new parcelleItem(50, 50);

    item->setPenColor(QColor(ui->label_couleurPolyline_Pen->text()));
    item->setColor(QColor(ui->label_couleurPolyline_P_background->text()));
    item->setWidthLine(ui->comboBox_epaisseurLignes_P->currentText().toInt());
    item->setTypeLine(ui->comboBox_typeLigne_P->currentIndex() + 1);
    item->setOpac(0.8);       //opacité de l'item
    item->setId(get_MaxId()); // valeur id en dernière position dans la table
    item->setUuid(QUuid::createUuid());
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(sizeW / 2, sizeH / 2);
    item->setTexte("Parcelle");
    item->setNom("Parcelle " + QString::number(get_MaxId()));
    item->setEtat(1);
    item->setMode(0);
    item->setTypeShape(parcelleItem::Rectangle);
    scene->addItem(item);
    QPointF zero = ui->graphicsView->mapFromScene(QPointF(0, 0));
    ui->graphicsView->horizontalScrollBar()->setValue(int(zero.x()));
    ui->graphicsView->verticalScrollBar()->setValue(int(zero.y()));
    QList <QGraphicsItem *> itemList = scene->items();
    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
}

void detail_parcelle::on_toolButton_AjoutCercle_clicked()
{   // créer un cercle
    parcelleItem *item = new parcelleItem(50, 50);

    item->setPenColor(QColor(ui->label_couleurPolyline_Pen->text()));
    item->setColor(QColor(ui->label_couleurPolyline_P_background->text()));
    item->setWidthLine(ui->comboBox_epaisseurLignes_P->currentText().toInt());
    item->setTypeLine(ui->comboBox_typeLigne_P->currentIndex() + 1);
    item->setOpac(0.8);                               //opacité de l'item
    item->setId(get_MaxId());
    item->setUuid(QUuid::createUuid());
    double sizeW = item->boundingRect().width();
    double sizeH = item->boundingRect().height();
    item->setPos(sizeW / 2, sizeH / 2);
    item->setNom("Plantation " + QString::number(get_MaxId()));
    item->setTexte("");
    item->setEtat(1);
    item->setMode(0);
    item->setTypeShape(parcelleItem::Circle);
    scene->addItem(item);
    QPointF zero = ui->graphicsView->mapFromScene(QPointF(0, 0));
    ui->graphicsView->horizontalScrollBar()->setValue(int(zero.x()));
    ui->graphicsView->verticalScrollBar()->setValue(int(zero.y()));
    QList <QGraphicsItem *> itemList = scene->items();
    ui->lineEdit_Id_Item->setText(QString::number(item->getId()));
}

int detail_parcelle::get_MaxId()
{   // recherche de la valeur du dernier Id
    int Max = 0;

    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 65536)   // parcelleItem
        {
            parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
            if (Max < item->getId())
            {
                Max = item->getId();
            }
        }
        if (itemList[i]->type() == 65537)   // MyPolygone
        {
            MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
            if (Max < item->getId())
            {
                Max = item->getId();
            }
        }
        if (itemList[i]->type() == 65539)   // MyPolyline
        {
            MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
            if (Max < item->getId())
            {
                Max = item->getId();
            }
        }
    }
    m_MaxId = Max + 1;
    return m_MaxId;
}

void detail_parcelle::on_comboBox_epaisseurLignes_P_currentTextChanged(const QString&arg1)
{
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
                item->setWidthLine(arg1.toInt());
                //   ui->tableObjets->setItem(i, 5,new QTableWidgetItem(QString::number(arg1.toInt())));
            }
            if (itemList[i]->type() == 65537)
            {
                MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setWidthLine(arg1.toInt());
            }
            if (itemList[i]->type() == 65539)
            {
                MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                item->setWidthLine(arg1.toInt());
            }
        }
    }

    scene->update();
}

void detail_parcelle::on_comboBox_typeLigne_P_currentIndexChanged(int index)
{
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
                item->setTypeLine(index + 1);
                item->setOldTypeLine(index + 1);
//             ui->tableObjets->setItem(i, 6,new QTableWidgetItem(QString::number(index+1)));
            }
            if (itemList[i]->type() == 65537)
            {
                MyPolygone *item = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setTypeLine(index + 1);
                item->setOldTypeLine(index + 1);
            }
            if (itemList[i]->type() == 65539)
            {
                MyPolyline *item = dynamic_cast <MyPolyline *> (itemList[i]);
                item->setTypeLine(index + 1);
                item->setOldTypeLine(index + 1);
            }
        }
    }
    scene->update();
}

void detail_parcelle::on_toolButton_CouleurCrayon_clicked()
{
    // choisir la couleur du crayon
    QColorDialog *dialog = new QColorDialog(this);

    dialog->show();
    int resultCode = dialog->exec();
    if (resultCode == QDialog::Accepted)
    {
        QColor   color   = dialog->currentColor();
        QVariant variant = color;
        QString  rgb     = variant.toString();
        ui->label_couleurPolyline_Pen->setText(rgb);
        ui->label_couleurPolyline_Pen->setStyleSheet("QLabel { background-color : " + rgb + "}");
    }
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                QColor        color   = dialog->currentColor();
                QVariant      variant = color;
                QString       rgb     = variant.toString();
                parcelleItem *item    = dynamic_cast <parcelleItem *> (itemList[i]);
                item->setPenColor(color);
            }
            if (itemList[i]->type() == 65537)
            {
                QColor      color   = dialog->currentColor();
                QVariant    variant = color;
                QString     rgb     = variant.toString();
                MyPolygone *item    = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setPenColor(color);
            }
            if (itemList[i]->type() == 65539)
            {
                QColor      color   = dialog->currentColor();
                QVariant    variant = color;
                QString     rgb     = variant.toString();
                MyPolyline *item    = dynamic_cast <MyPolyline *> (itemList[i]);
                item->setPenColor(color);
            }
        }
    }
    //items_vers_listeparcelles();

    delete dialog;

    scene->update();
}

void detail_parcelle::on_toolButton_CouleurFond_clicked()
{
    QColorDialog *dialog = new QColorDialog(this);

    dialog->show();
    int resultCode = dialog->exec();
    if (resultCode == QDialog::Accepted)
    {
        QColor   color   = dialog->currentColor();
        QVariant variant = color;
        QString  rgb     = variant.toString();
        ui->label_couleurPolyline_P_background->setText(rgb);
        ui->label_couleurPolyline_P_background->setStyleSheet("QLabel { background-color : " + rgb + "}");
    }

    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536)
            {
                QColor        color   = dialog->currentColor();
                QVariant      variant = color;
                QString       rgb     = variant.toString();
                parcelleItem *item    = dynamic_cast <parcelleItem *> (itemList[i]);
                item->setColor(color);
            }
            if (itemList[i]->type() == 65537)
            {
                QColor      color   = dialog->currentColor();
                QVariant    variant = color;
                QString     rgb     = variant.toString();
                MyPolygone *item    = dynamic_cast <MyPolygone *> (itemList[i]);
                item->setColor(color);
            }
        }
    }
    delete dialog;
    scene->update();
}

void detail_parcelle::on_pushButton_zoomIn_clicked()
{
    zoomGraphicsView(2);
}

void detail_parcelle::on_pushButton_ZoomOut_clicked()
{
    zoomGraphicsView(3);
}

void detail_parcelle::zoomGraphicsView(int ratio)
{   // valeur ratio in-> 2  out-> 3)
    if (ratio == 3 && m_ZoomRatio > 0.5)
    {
        m_ZoomRatio = m_ZoomRatio * 0.8;
        ui->graphicsView->scale(0.8, 0.8);
    }
    if (ratio == 2 && m_ZoomRatio < 3)
    {
        m_ZoomRatio = m_ZoomRatio * 1.25;
        ui->graphicsView->scale(1.25, 1.25);
    }
    ui->label_zoom->setText(QString::number(m_ZoomRatio, 'f', 2));
}

void detail_parcelle::on_toolButton_modification_clicked()
{
    m_mode = 0;
    ui->toolButtonUtilisation->setChecked(false);
    ui->toolButton_modification->setChecked(true);
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 65536)  // MyItem
        {
            parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
            item->setMode(0);
        }
        if (itemList[i]->type() == 65537)  // MyPolygone
        {
            MyPolygone *Pitem = dynamic_cast <MyPolygone *> (itemList[i]);
            Pitem->setMode(0);
        }
        if (itemList[i]->type() == 65539)  // MyPolyline
        {
            MyPolyline *Poline = dynamic_cast <MyPolyline *> (itemList[i]);
            Poline->setMode(0);
        }
    }
}

void detail_parcelle::on_toolButtonUtilisation_clicked()
{
    m_mode = 1;

    ui->toolButtonUtilisation->setChecked(true);
    ui->toolButton_modification->setChecked(false);
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->type() == 65536) // MyItem
        {
            parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
            item->setMode(1);
        }
        if (itemList[i]->type() == 65537) // MyPolygone
        {
            MyPolygone *Pitem = dynamic_cast <MyPolygone *> (itemList[i]);
            Pitem->setMode(1);
        }
        if (itemList[i]->type() == 65539) // MyPolyline
        {
            MyPolyline *Poline = dynamic_cast <MyPolyline *> (itemList[i]);
            Poline->setMode(1);
        }
    }
}

void detail_parcelle::on_toolButton_clicked()
{
    QList <QGraphicsItem *> itemList = scene->items();
    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList[i]->isSelected())
        {
            if (itemList[i]->type() == 65536) // MyItem
            {
                parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
                item->setNom(util::apos(ui->lineEdit_Nom_item->text()));
                item->setTexte(util::apos(ui->textEdit_plan_commentaires->document()->toPlainText()));
            }
        }
    }
    scene->update();
}

void detail_parcelle::on_toolButton_2_clicked()
{
    close();
}

void detail_parcelle::on_toolButton_supprimer_clicked()
{   // supprimer objet
    int ret = QMessageBox::warning(this, tr("Suppression"),
                                   tr("Cet objet peut être supprimer.\n"
                                      "Confirmer la suppression de l'objet ?"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel);

    switch (ret)
    {
    case QMessageBox::Ok:
    {
        QList <QGraphicsItem *> itemList = scene->items();
        for (int i = 0; i < itemList.size(); i++)
        {
            if (itemList[i]->isSelected())
            {
                if (itemList[i]->type() == 65536) // MyItem
                {
                    parcelleItem *item = dynamic_cast <parcelleItem *> (itemList[i]);
                    scene->removeItem(item);
                }
            }
        }
        scene->update();
        break;
    }

    case QMessageBox::Cancel:
    {
        break;
    }

    default:
        break;
    }
}

void detail_parcelle::on_toolButton_devant_clicked()
{   // mettre devant
    if (scene->selectedItems().isEmpty())
    {
        return;
    }

    QGraphicsItem *         selectedItem = scene->selectedItems().first();
    QList <QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;

    for(QGraphicsItem * item : overlapItems)
    {
        zValue = item->zValue() + 0.1;
        qDebug() << zValue;
    }
    selectedItem->setZValue(zValue);
}

void detail_parcelle::on_pushButton_Affiches_fiche_clicked()
{
    int Id = ui->lineEdit_Id_Item->text().toInt();

    // int Id = ui->lineEditUuid->text().toInt();

    if (Id >= 0)
    {
        QString   fileName       = getfileNameSQL();
        Cultures *fiche_Cultures = new Cultures(Id, 0);
        fiche_Cultures->show();
    }
}
