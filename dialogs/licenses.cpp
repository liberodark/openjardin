#include "licenses.h"
#include "ui_licenses.h"
#include "mainwindow.h"
#include "utilitaires/util.h"

Licenses::Licenses(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Licenses)
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
}

Licenses::~Licenses()
{
    delete ui;
}
