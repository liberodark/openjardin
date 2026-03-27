cmake_minimum_required(VERSION 3.16)
project(openjardin VERSION 1.08 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Sql
    Xml
    PrintSupport
    Core5Compat
)

set(SOURCES
    main.cpp
    mainwindow.cpp
    consts.cpp
    utilitaires/util.cpp
    modeles/parcelle.cpp
    graphic/background.cpp
    graphic/myitem.cpp
    graphic/mybutton.cpp
    graphic/parcelleitem.cpp
    graphic/planning_item.cpp
    graphic/mygrilleitem.cpp
    graphic/vignette.cpp
    graphic/mypolygone.cpp
    graphic/mypolyline.cpp
    graphic/mygraphicsscene.cpp
    graphic/mytreewidgetitem.cpp
    graphic/sommet.cpp
    graphic/cote.cpp
    graphic/fonditem.cpp
    graphic/myvertex.cpp
    graphic/tache_item.cpp
    dialogs/dialogrequetefamille.cpp
    dialogs/nouveau_projet.cpp
    dialogs/planning.cpp
    dialogs/dialog_aide.cpp
    dialogs/cultures.cpp
    dialogs/fiche_plantes.cpp
    dialogs/configuration.cpp
    dialogs/licenses.cpp
    dialogs/planner.cpp
    dialogs/dialog_taches.cpp
    dialogs/dialog_type_de_tache.cpp
    dialogs/dialog_ressources.cpp
    dialogs/dialog_moyens.cpp
    dialogs/dialog_coordonnees.cpp
    dialogs/dialog_ajout_moyen.cpp
    dialogs/dialog_ajout_ressource.cpp
    dialogs/dialog_type_de_moyen.cpp
    dialogs/detail_parcelle.cpp
)

set(HEADERS
    mainwindow.h
    consts.h
    utilitaires/util.h
    modeles/parcelle.h
    graphic/background.h
    graphic/myitem.h
    graphic/mybutton.h
    graphic/parcelleitem.h
    graphic/planning_item.h
    graphic/mygrilleitem.h
    graphic/vignette.h
    graphic/mypolygone.h
    graphic/mypolyline.h
    graphic/mygraphicsscene.h
    graphic/mytreewidgetitem.h
    graphic/sommet.h
    graphic/cote.h
    graphic/fonditem.h
    graphic/myvertex.h
    graphic/tache_item.h
    dialogs/dialogrequetefamille.h
    dialogs/nouveau_projet.h
    dialogs/planning.h
    dialogs/dialog_aide.h
    dialogs/cultures.h
    dialogs/fiche_plantes.h
    dialogs/configuration.h
    dialogs/licenses.h
    dialogs/planner.h
    dialogs/dialog_taches.h
    dialogs/dialog_type_de_tache.h
    dialogs/dialog_ressources.h
    dialogs/dialog_moyens.h
    dialogs/dialog_coordonnees.h
    dialogs/dialog_ajout_moyen.h
    dialogs/dialog_ajout_ressource.h
    dialogs/dialog_type_de_moyen.h
    dialogs/detail_parcelle.h
)

set(FORMS
    mainwindow.ui
    dialogs/dialog_aide.ui
    dialogs/cultures.ui
    dialogs/dialogrequetefamille.ui
    dialogs/fiche_plantes.ui
    dialogs/configuration.ui
    dialogs/licenses.ui
    dialogs/nouveau_projet.ui
    dialogs/planner.ui
    dialogs/dialog_taches.ui
    dialogs/dialog_type_de_tache.ui
    dialogs/dialog_ressources.ui
    dialogs/dialog_moyens.ui
    dialogs/dialog_coordonnees.ui
    dialogs/dialog_ajout_moyen.ui
    dialogs/dialog_ajout_ressource.ui
    dialogs/dialog_type_de_moyen.ui
    dialogs/detail_parcelle.ui
    dialogs/planning.ui
)

set(RESOURCES
    openjardin.qrc
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR};${CMAKE_SOURCE_DIR}/dialogs"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Xml
    Qt6::PrintSupport
    Qt6::Core5Compat
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
