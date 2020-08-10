#include "dialog_aide.h"
#include "ui_dialog_aide.h"
#include "mainwindow.h"
#include <QDebug>
#include <qsplitter.h>
#include "graphic/mytreewidgetitem.h"
#include "utilitaires/util.h"
#include <QtGui>
#include <QApplication>

dialog_Aide::dialog_Aide(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_Aide)
{
    // translator
    QTranslator translator;
    QString     fichier = ":/translations/open-jardin_" + util::getLocale();
    QSettings settings;
    QFile        iniFile(settings.fileName());
    if (iniFile.exists())
    {
        QSettings settings(iniFile.fileName(), QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        QString langue = settings.value("langue").toString();
        if(langue == "english")
            {
            // forcer la langue anglaise
                 fichier = ":/translations/open-jardin_en.ts";

             }


     }
    translator.load(fichier);
    qApp->installTranslator(&translator);

    ui->setupUi(this);
    parse_html();
    ui->treeWidget_aide->header()->setFixedHeight(10);
}

dialog_Aide::~dialog_Aide()
{
    delete ui;
}

void dialog_Aide::populate(QStringList linkNames)
{   //test remplissage du treewidget avec les liens ou ancres du texte html
    for (int i = 0; i < linkNames.count(); i++)
    {
        QString     linkName      = linkNames[i];
        QStringList splitLinkName = linkNames[i].split('/');
        if (splitLinkName.count() == 1)
        {
            {
                if (ui->treeWidget_aide->findItems(splitLinkName[0], Qt::MatchFixedString).isEmpty())
                {
                    QTreeWidgetItem * topLevelItem = new mytreewidgetitem;
                    mytreewidgetitem *myTreeItem   = static_cast <mytreewidgetitem *>(topLevelItem);
                    myTreeItem->setText(0, splitLinkName[0]);
                    myTreeItem->setAncre(0, linkName);
                    ui->treeWidget_aide->addTopLevelItem(myTreeItem);
                    splitLinkName = linkNames[i + 1].split('/');
                    if (splitLinkName.count() > 1)
                    {
                        for (int j = i + 1; j < linkNames.count(); j++)
                        {
                            linkName      = linkNames[j];
                            splitLinkName = linkNames[j].split('/');
                            if (splitLinkName.count() > 1)
                            {
                                QTreeWidgetItem * childItem   = new mytreewidgetitem;
                                mytreewidgetitem *MychildItem = static_cast <mytreewidgetitem *>(childItem);
                                MychildItem->setText(0, splitLinkName[1]);
                                MychildItem->setAncre(0, linkName);
                                myTreeItem->addChild(MychildItem);
                            }
                            else
                            {
                                i = j - 1;
                                ui->treeWidget_aide->addTopLevelItem(myTreeItem);
                                j = linkNames.count();
                            }
                        }
                    }
                }
            }
        }
    }
}

void dialog_Aide::parse_html()
{
    // extraction des liens du texte html
    QString     texte_html = ui->textBrowser_aide->toHtml();
    QStringList qList      = texte_html.split(';', QString::SkipEmptyParts);
    QStringList linkNames;

    foreach(const QString &s, qList)
    {
        // récupération des ancres
        if (s.contains("<a name="))
        {
            int        pos    = s.indexOf("<a name=");
            QStringRef s2     = s.midRef(pos + 9);
            int        posEnd = s2.indexOf(">");
            QString    s4     = s2.toString();
            QStringRef s3     = s4.leftRef(posEnd - 1);
            linkNames << s3.toString();
        }
    }
    populate(linkNames); // remplissage du treewidget
}

void dialog_Aide::on_treeWidget_aide_itemClicked(QTreeWidgetItem *item, int column)
{   //aller sur le lien choisi
    Q_UNUSED(item);
    Q_UNUSED(column);
    QList <QTreeWidgetItem *> itemList;

    itemList = ui->treeWidget_aide->selectedItems();
    foreach(QTreeWidgetItem * item, itemList)
    {
        mytreewidgetitem *item_ancre = dynamic_cast <mytreewidgetitem *> (item);
        QString           str        = "#" + item_ancre->getAncre(0);

        ui->textBrowser_aide->setSource(QUrl(str));
    }
}
