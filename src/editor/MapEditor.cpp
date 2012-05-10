//
// Copyright (C) 2011 Xavier MICHEL (xavier.michel.mx440@gmail.com)
//
// This software is provided as-is, without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//


#include "MapEditor.hpp"

#include "Map.hpp"
#include "ScriptingArea.hpp"
#include "ObjectPalette.hpp"

#include "ObjectInspector.hpp"
#include "LoadingSplash.hpp"

#include "Settings.hpp"

MapEditor::MapEditor() : m_saveName("") {

    Log::v("editor") << "Création de l'éditeur";

    // un splash pour patienter pendant le chargement
    LoadingSplash loadingSplash;

    setWindowTitle(editorName);

    // zone centrale
    Log::v("editor") << "Création de la carte (zone centrale)";
    loadingSplash.setCurrentStep(MAP);
    m_map = new Map(this);
    setCentralWidget(m_map);

    // zone de scripting
    Log::v("editor") << "Création de la zone de scripting";
    loadingSplash.setCurrentStep(SCRIPTING);
    m_scriptingArea = new ScriptingArea(this);
    addDockWidget(Qt::BottomDockWidgetArea, m_scriptingArea);

    // inspecteur d'objets
    Log::v("editor") << "Création de l'inspecteur d'objets";
    loadingSplash.setCurrentStep(INSPECTOR);
    m_objectInspector = new ObjectInspector(m_map, this);
    addDockWidget(Qt::RightDockWidgetArea, m_objectInspector);

    // on partage l'inspecteur. Sur l'éditeur pour l'interface, sur la map pour les données
    m_map->setObjectInspectorTable(m_objectInspector->getObjectInspectorTable());

    // palette
    Log::v("editor") << "Création de la palette";
    loadingSplash.setCurrentStep(PALETTE);
    m_objectPalette = new ObjectPalette(m_map, this);
    addDockWidget(Qt::LeftDockWidgetArea, m_objectPalette);

    // le menu et la barre d'outils
    loadingSplash.setCurrentStep(MENUS);
    createMenuAndToolBar();

    connect(m_objectPalette, SIGNAL(selectedItemChanged()), m_map, SLOT(paletteItemChanged()));

    // chargement terminé
    loadingSplash.close();
}



void MapEditor::createMenuAndToolBar() {

    Log::v("editor") << "Création menu et barre d'outils";

    // création des actions

    QAction *quitAction = new QAction("&Quitter", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    //quitAction->setShortcut(QKeySequence("Ctrl+Q"));                  // en vrai je fais souvent ^q à la place de ^s et ça m'énerve alors...

    QAction *saveAction = new QAction("&Sauvegarder", this);
    saveAction->setIcon(QIcon(":/ressources/sauvegarder.png"));
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    connect(saveAction, SIGNAL(triggered()), SLOT(save()));

    QAction *saveAsAction = new QAction("&Sauvegarder sous", this);
    saveAsAction->setIcon(QIcon(":/ressources/sauvegarder.png"));
    saveAsAction->setShortcut(QKeySequence("Shift+Ctrl+S"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    QAction *loadAction = new QAction("&Charger", this);
    loadAction->setIcon(QIcon(":/ressources/charger.png"));
    loadAction->setShortcut(QKeySequence("Ctrl+L"));
    connect(loadAction, SIGNAL(triggered()), this, SLOT(load()));

    QAction *newMapAction = new QAction("&Nouvelle carte", this);
    newMapAction->setIcon(QIcon(":/ressources/nouveau.png"));
    newMapAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(newMapAction, SIGNAL(triggered()), this, SLOT(clear()));

    /*
    actionAjoutXml = new QAction("Ajouter/Modifier une image du xml", this);
    connect(actionAjoutXml, SIGNAL(triggered()), this, SLOT(ajoutImageXml()));

    actionAjoutSon = new QAction("Ajouter/Modifier un son du xml", this);
    connect(actionAjoutSon, SIGNAL(triggered()), this, SLOT(ajoutSonXml()));

    actionObjet = new QAction("Ajouter/Modifier un objet", this);
    connect(actionObjet, SIGNAL(triggered()), SLOT(ajoutObjets()));
    */

    QAction *showPaletteAction = new QAction("&Palette", this);
    showPaletteAction->setCheckable(true);
    showPaletteAction->setChecked(true);
    connect(showPaletteAction, SIGNAL(toggled(bool)), m_objectPalette, SLOT(setShown(bool)));
    connect(m_objectPalette, SIGNAL(visibilityChanged(bool)), showPaletteAction, SLOT(setChecked(bool)));

    QAction *showScriptingAction = new QAction("&Script", this);
    showScriptingAction->setCheckable(true);
    showScriptingAction->setChecked(true);
    connect(showScriptingAction, SIGNAL(toggled(bool)), m_scriptingArea, SLOT(setShown(bool)));
    connect(m_scriptingArea, SIGNAL(visibilityChanged(bool)), showScriptingAction, SLOT(setChecked(bool)));

    QAction *showObjectInspector = new QAction("&Inspecteur d'objets", this);
    showObjectInspector->setCheckable(true);
    showObjectInspector->setChecked(true);
    connect(showObjectInspector, SIGNAL(toggled(bool)), m_objectInspector, SLOT(setShown(bool)));
    connect(m_objectInspector, SIGNAL(visibilityChanged(bool)), showObjectInspector, SLOT(setChecked(bool)));

    /*
    actionNumero = new QAction("&Afficher les numéros scripting", this);
    actionNumero->setCheckable(true);
    */


    // remplissage des menus

    QMenu *fileMenu = menuBar()->addMenu("&Fichier");
    QMenu *showMenu = menuBar()->addMenu("&Afficher");
    QMenu *toolMenu = menuBar()->addMenu("&Outils");

    fileMenu->addAction(newMapAction);
    fileMenu->addAction(loadAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(quitAction);


    showMenu->addAction(showPaletteAction);
    showMenu->addAction(showObjectInspector);
    showMenu->addAction(showScriptingAction);

    /*
    showMenu->addAction(actionNumero);

    toolMenu->addAction(actionAjoutXml);
    toolMenu->addAction(actionAjoutSon);
    toolMenu->addAction(actionObjet);
    */

    //connect(actionNumero, SIGNAL(triggered()), this, SLOT(updateNumero()));



    // partie barre d'outils

    QToolBar *toolBarre = addToolBar("Outils");

    QCheckBox *checkShowGrid = new QCheckBox("Afficher la grille");
    checkShowGrid->setChecked(true);
    connect(checkShowGrid, SIGNAL(toggled(bool)), m_map, SLOT(showGrid(bool)));

    QCheckBox *checkAlignOnGrid = new QCheckBox("Aligner sur la grille");
    checkAlignOnGrid->setChecked(true);
    connect(checkAlignOnGrid, SIGNAL(toggled(bool)), m_map, SLOT(setAlignOnGrid(bool)));

    toolBarre->addAction(newMapAction);
    toolBarre->addAction(loadAction);
    toolBarre->addAction(saveAction);

    QAction *runGameAction = new QAction("Run !", this);
    runGameAction->setIcon(QIcon(":/ressources/run.png"));
    runGameAction->setShortcut(QKeySequence("Ctrl+R"));
    toolBarre->addAction(runGameAction);
    connect(runGameAction, SIGNAL(triggered()), this, SLOT(run()));

    QAction *editMapAttributes = new QAction("Editer attributs carte", this);
    connect(editMapAttributes, SIGNAL(triggered()), m_map, SLOT(openEditMapAttributes()));

    toolBarre->addSeparator();

    toolBarre->addWidget(checkShowGrid);
    toolBarre->addSeparator();
    toolBarre->addWidget(checkAlignOnGrid);

    toolBarre->addSeparator();
    toolBarre->addAction(editMapAttributes);

}


const ObjectPalette * MapEditor::getObjectPalette() const {
    return m_objectPalette;
}


void MapEditor::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Shift) {
        m_map->setShiftKeyDown(true);
    } else if (event->key() == Qt::Key_Delete) {
        m_map->deleteSelectedItems();
    } else if (event->matches(QKeySequence::Copy)) {
        m_map->copy();
    } else if (event->matches(QKeySequence::Paste)) {
        m_map->paste();
    } else if (event->matches(QKeySequence::Cut)) {
        m_map->cut();
    }
}


void MapEditor::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Shift) {
        m_map->setShiftKeyDown(false);
    }
}


void MapEditor::clear() {
    setWindowTitle(editorName);
    m_saveName = "";
    m_map->newMap();
    m_scriptingArea->clear();
}


void MapEditor::saveAs() {

    QString dialogResult = QFileDialog::getSaveFileName(0, "Enregistrer la carte sous", MAPS_DIR, "xml (*.xml)");
    if ( dialogResult.isEmpty() ) {
        QMessageBox::critical(0, "Erreur", "Pas de fichier selectionné, sauvegarde annulée");
        return;
    }

    m_saveName = QFileInfo(dialogResult).fileName();
    if ( ! m_saveName.endsWith(".xml") ) { // ajouter le .xml s'il n'a pas été saisi (sous linux on peut faire sans, je ne sais pas sous windows)
        m_saveName += ".xml";
    }

    save();
}


void MapEditor::save() {

    if ( m_saveName.isEmpty() ) {
        saveAs();
        if ( m_saveName.isEmpty() ) { // toujours pas de fichier selectionné ?
            return;
        }
    }

    setWindowTitle(editorName + " - " + m_saveName);

    m_map->save(m_saveName);
    QString scriptName = QString(m_saveName).replace(".xml", ".lua");
    m_scriptingArea->save(scriptName);
}


void MapEditor::load() {

    QString dialogResult = QFileDialog::getOpenFileName(this, "Ouvrir une carte", MAPS_DIR, "xml (*.xml)");

    if (dialogResult.isEmpty()) { // on n'a selectionné aucun fichier
        return;
    }

    m_saveName = QFileInfo(dialogResult).fileName();

    setWindowTitle(editorName + " - " + m_saveName);

    m_map->load(m_saveName);
    QString scriptName = QString(m_saveName).replace(".xml", ".lua");
    m_scriptingArea->load(scriptName);
}


void MapEditor::run() {

    save();

    if ( m_saveName.isEmpty() ) {   // pas de sauvegarde effectuée
        return;
    }

    QProcess *process = new QProcess(this);
    QString mapName = QFileInfo(m_saveName).baseName(); // car le jeu reçoi le nom de la carte sans son extension
    Log::i("mapEditor") << "lancement du jeu avec comme carte :" << mapName;

#if defined(Q_OS_WIN)
    process->start(Settings::getWindowsExecutable() + " " + mapName);
#elif defined(Q_OS_LINUX)
    process->start(Settings::getLinuxExecutable() + " " + mapName);
#else
#error Unknown OS
#endif

}

