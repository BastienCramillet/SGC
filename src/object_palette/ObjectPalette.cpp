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


#include "ObjectPalette.hpp"

#include <QtXml>

#include "MapEditor.hpp"

#include "ObjectTreeWidgetItem.hpp"
#include "AreaTreeWidgetItem.hpp"

#include "ObjectManager.hpp"

#include "Map.hpp"
#include "MiniMapView.hpp"


ObjectPalette::ObjectPalette(Map *map, MapEditor *parent)
    : QDockWidget(parent)
{
    setWindowTitle("Palette");

    m_tabWidget.addTab(&m_areaTree, "Zones");
    m_tabWidget.addTab(&m_decorsTree, "Décors");
    m_tabWidget.addTab(&m_charactersTree, "Personnages");

    fill();

    m_minMapView = new MiniMapView(map);

    QWidget *dockContent = new QWidget(this);
    setWidget(dockContent);

    QVBoxLayout *dockLayout = new QVBoxLayout();
    dockLayout->addWidget(&m_tabWidget);
    dockLayout->addWidget(m_minMapView);

    dockContent->setLayout(dockLayout);

    connect(&m_areaTree, SIGNAL(itemSelectionChanged()), SLOT(emitSelectedItemChanged()));
    connect(&m_charactersTree, SIGNAL(itemSelectionChanged()), SLOT(emitSelectedItemChanged()));
    connect(&m_decorsTree, SIGNAL(itemSelectionChanged()), SLOT(emitSelectedItemChanged()));
}


void ObjectPalette::fill() {

    m_decorsTree.clear();
    m_charactersTree.clear();
    m_areaTree.clear();

    // ajout des zones

    // zone par défaut
    m_areaTree.addTopLevelItem(new AreaTreeWidgetItem(Settings::getAreasDefaultDatas()));

    // et les autres zones
    foreach(AreasData ad, Settings::getAreasList()) {
        m_areaTree.addTopLevelItem(new AreaTreeWidgetItem(ad));
    }

    // les autres éléments
    QString currentString;
    QMap<QString, QTreeWidgetItem*> categoryMap;

    // ajout des catégories de décor
    foreach(currentString, Settings::getDecorsEditorsCategories()) {
        Log::v("ObjectPalette") << "Adding decor's category :" << currentString;
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(-1);
        treeItem->setText(0, currentString);
        m_decorsTree.addTopLevelItem(treeItem);
        categoryMap[currentString] = treeItem;
    }

    // ajout des catégories de personnage
    foreach(currentString, Settings::getCharactersEditorsCategories()) {
        Log::v("ObjectPalette") << "Adding characters's category :" << currentString;
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(-1);
        treeItem->setText(0, currentString);
        m_charactersTree.addTopLevelItem(treeItem);
        categoryMap[currentString] = treeItem;
    }

    // ajout des objets
    QMap<QString, ObjectData *> objectMap = ObjectManager::getObjectMap();
    QList<QString> objectKeys = objectMap.keys();

    Log::v("ObjectPalette") << "Going to add objects";
    foreach(QString currentID, objectKeys) {
        categoryMap[ objectMap[currentID]->editorCategory ]->addChild(new ObjectTreeWidgetItem(currentID));
    }
}


void ObjectPalette::emitSelectedItemChanged() {
    emit selectedItemChanged();
}


GraphicsObjectType ObjectPalette::selectedObjectType() const {

    GraphicsObjectType ret = ( m_tabWidget.currentWidget() == &m_areaTree ? MAP_AREA : MAP_OBJECT );

    QTreeWidgetItem *currentItem = qobject_cast<QTreeWidget *>(m_tabWidget.currentWidget())->currentItem();
    if ( ! currentItem || currentItem->type() == -1) {
        Log::w("ObjectPalette") << "objet sélectionné invalide";
        throw PALETTE_INVALID_SELECTED_ITEM;
    }

    return ret;
}


QString ObjectPalette::selectedObjectID() const {

    try {
        if ( selectedObjectType() != MAP_OBJECT ) {
            throw PALETTE_INVALID_SELECTED_ITEM;
        }
    } catch(...) {
        throw PALETTE_INVALID_SELECTED_ITEM;
    }

    QTreeWidgetItem *currentItem = qobject_cast<QTreeWidget *>(m_tabWidget.currentWidget())->currentItem();
    return static_cast<ObjectTreeWidgetItem *>(currentItem)->objectID();
}


AreasData ObjectPalette::selectedAreaDatas() const {
    try {
        if ( selectedObjectType() != MAP_AREA ) {
            throw PALETTE_INVALID_SELECTED_ITEM;
        }
    } catch(...) {
        throw PALETTE_INVALID_SELECTED_ITEM;
    }

    QTreeWidgetItem *currentItem = qobject_cast<QTreeWidget *>(m_tabWidget.currentWidget())->currentItem();
    AreaTreeWidgetItem *area = static_cast<AreaTreeWidgetItem *>(currentItem);
    return area->getAreasData();
}

