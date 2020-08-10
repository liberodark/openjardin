#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T18:03:06
#
unix:!mac {
 LIBS += -Wl,-rpath=\\\$$ORIGIN/../lib
}

target.path = /usr/bin
target.files = app_name qt.conf
INSTALLS += target
data.path = /usr/share/openjardin/lib
data.files = lib/*
INSTALLS += data
unix:QMAKE_RPATHDIR += /usr/lib/x86_64-linux-gnu

QT  += core gui
QT  += xml widgets
QT  += sql widgets
QT  += printsupport
QT  += xmlpatterns
QT  += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = openjardin
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
    dialogs/nouveau_projet.cpp \
    dialogs/planning.cpp \
    graphic/background.cpp \
    graphic/myitem.cpp \
    graphic/mybutton.cpp \
    graphic/parcelleitem.cpp \
    graphic/planning_item.cpp \
    graphic/mygrilleitem.cpp \
    graphic/vignette.cpp \
    mainwindow.cpp \
    graphic/mypolygone.cpp \
    graphic/mypolyline.cpp \
    graphic/mygraphicsscene.cpp \
    graphic/mytreewidgetitem.cpp \
    consts.cpp \
    dialogs/dialog_aide.cpp \
    dialogs/cultures.cpp \
    graphic/sommet.cpp \
    graphic/cote.cpp \
    dialogs/fiche_plantes.cpp \
    dialogs/configuration.cpp \
    dialogs/licenses.cpp \
    dialogs/planner.cpp \
    graphic/tache_item.cpp \
    dialogs/dialog_taches.cpp \
    dialogs/dialog_type_de_tache.cpp \
    dialogs/dialog_ressources.cpp \
    dialogs/dialog_moyens.cpp \
    dialogs/dialog_coordonnees.cpp \
    dialogs/dialog_ajout_moyen.cpp \
    dialogs/dialog_ajout_ressource.cpp \
    dialogs/dialog_type_de_moyen.cpp \
    dialogs/detail_parcelle.cpp \
    modeles/parcelle.cpp \
    utilitaires/util.cpp

HEADERS  += mainwindow.h \
    dialogs/nouveau_projet.h \
    dialogs/planning.h \
    graphic/background.h \
    graphic/myitem.h \
    graphic/mybutton.h \
    graphic/parcelleitem.h \
    graphic/planning_item.h \
    graphic/mygrilleitem.h \
    graphic/mypolygone.h \
    graphic/mypolyline.h \
    graphic/mygraphicsscene.h \
    graphic/mytreewidgetitem.h \
    consts.h \
    dialogs/dialog_aide.h \
    dialogs/cultures.h \
    graphic/sommet.h \
    graphic/cote.h \
    dialogs/fiche_plantes.h \
    dialogs/configuration.h \
    dialogs/licenses.h \
    dialogs/planner.h \
    graphic/tache_item.h \
    dialogs/dialog_taches.h \
    dialogs/dialog_type_de_tache.h \
    dialogs/dialog_ressources.h \
    dialogs/dialog_moyens.h \
    dialogs/dialog_coordonnees.h \
    dialogs/dialog_ajout_moyen.h \
    dialogs/dialog_ajout_ressource.h \
    dialogs/dialog_type_de_moyen.h \
    dialogs/detail_parcelle.h \
    graphic/vignette.h \
    modeles/parcelle.h \
    utilitaires/util.h

FORMS    += mainwindow.ui \
    dialogs/dialog_aide.ui \
    dialogs/cultures.ui \
    dialogs/fiche_plantes.ui \
    dialogs/configuration.ui \
    dialogs/licenses.ui \
    dialogs/nouveau_projet.ui \
    dialogs/planner.ui \
    dialogs/dialog_taches.ui \
    dialogs/dialog_type_de_tache.ui \
    dialogs/dialog_ressources.ui \
    dialogs/dialog_moyens.ui \
    dialogs/dialog_coordonnees.ui \
    dialogs/dialog_ajout_moyen.ui \
    dialogs/dialog_ajout_ressource.ui \
    dialogs/dialog_type_de_moyen.ui \
    dialogs/detail_parcelle.ui \
    dialogs/planning.ui

RESOURCES += \
    openjardin.qrc

TRANSLATIONS = translations/open-jardin_fr.ts \
               translations/open-jardin_en.ts \
               translations/open-jardin_es.ts \
               translations/open-jardin_it.ts
