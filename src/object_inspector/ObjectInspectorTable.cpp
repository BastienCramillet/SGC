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


#include "ObjectInspectorTable.hpp"

#include "ObjectInspector.hpp"
#include "GraphicsObject.hpp"
#include "Map.hpp"
#include "MapObjectEdit.hpp"
#include "MapAreaEdit.hpp"

#include "MapObject.hpp"
#include "MapArea.hpp"

#include "ObjectManager.hpp"

ObjectInspectorTable::ObjectInspectorTable(Map *map)
    : m_map(map), m_graphicsObjectEditor(0)
{

    QStringList tableHeadersLabel;
    tableHeadersLabel << "Nom" << "Type";
    setColumnCount(2);
    setHeaderLabels(tableHeadersLabel);

    setAlternatingRowColors(true);

    setAnimated(true);

    setSelectionMode(QAbstractItemView::ExtendedSelection);

    clear();
    addDefaultLayout();

    connect(this, SIGNAL(itemSelectionChanged()), SLOT(selectionUpdated()));
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), SLOT(itemWasDoubleClicked(QTreeWidgetItem*,int)));

    // connexion pour gérer les layouts
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)), SLOT(LayoutItemModified(QTreeWidgetItem*,int)));

    setDragDropMode(QAbstractItemView::DragDrop);
}


void ObjectInspectorTable::addRow(GraphicsObject *object, int layoutNumber) {

    QTreeWidgetItem *current_item = currentItem();

    // si rien n'est sélectionné, on selectionne au moins un calque (le premier qu'on trouve...)
    if ( ! current_item ) {
        m_objectMap.key(0)->setSelected(true);
        current_item = m_objectMap.key(0);
    }

    QTreeWidgetItem *parent;  // sous quel parent on va placer cet item
    if ( layoutNumber != 0 ) { // numéro de layout spécifié
        parent = topLevelItem(-layoutNumber-1);
    } else { // la numéro de layout n'est pas renseigné
        if ( ! m_objectMap[current_item] ) { // on est sur un layout, on se placera dessous
            parent = current_item;
        } else {  // on est sur un item, on se placera au même niveau
            parent = current_item->parent();
        }
    }

    QStringList itemStringList;
    itemStringList << object->scriptingName() << object->description();
    QTreeWidgetItem *item = new QTreeWidgetItem(itemStringList);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled);

    item->setIcon(1, object->pixmap());


    parent->addChild(item);

    Log::d("ObjectInspectorTable") << "addRow, zIndex = " << -indexOfTopLevelItem(parent) - 1;

    object->setZValue(-indexOfTopLevelItem(parent) - 1);
    object->setVisible( parent->checkState(0) == Qt::Checked );

    parent->setExpanded(true);
    m_objectMap[item] = object;
}


void ObjectInspectorTable::removeRow(GraphicsObject *object) {
    // apparement Qt sait géré un élément supprimé directement
    delete m_objectMap.key(object);
}


void ObjectInspectorTable::mousePressEvent(QMouseEvent *event) {
    QTreeWidget::mousePressEvent(event);

    if ( selectedItems().count() == 1 && m_objectMap[selectedItems().at(0)] == 0 ) { // seul un calque est sélectionné, on selectionne les sous élements sous le calque
        QTreeWidgetItem *selectedLayout = selectedItems().at(0);
        for (int i=0; i<selectedLayout->childCount(); ++i) {
            selectedLayout->child(i)->setSelected(true);
        }
        // on vient de sélectionner un layout donc on peut agir dessus
        emit currentLayoutIsUppable( indexOfTopLevelItem(selectedLayout) > 0 );
        emit currentLayoutIsDownable( indexOfTopLevelItem(selectedLayout) < topLevelItemCount() - 1 );
    } else {
        emit currentLayoutIsUppable(false);
        emit currentLayoutIsDownable(false);
    }

    updateMapSelection();
}


void ObjectInspectorTable::dropEvent(QDropEvent *event) {
    if ( event->source() == this ) {

        QTreeWidgetItem *target = itemAt(event->pos());

        if ( target ) {
            if ( m_objectMap[target] ) { // on est pas sur un layout !
                target = target->parent();
            }

            foreach (QTreeWidgetItem *it, selectedItems()) {

                if ( m_objectMap[it] ) { // on est pas sur un layout !

                    // re retrouve le layout source
                    QTreeWidgetItem *source = it;
                    if ( m_objectMap[source] ) { // on est pas sur un layout !
                        source = source->parent();
                    }

                    // source != target ?
                    if (source != target) {
                        source->removeChild(it);
                        target->addChild(it);
                        m_objectMap[it]->setZValue(-indexOfTopLevelItem(target) - 1);
                        Log::d("ObjectInspectorTable") << "dropEvent, zIndex = " << -indexOfTopLevelItem(target) - 1;
                        m_objectMap[it]->setVisible( target->checkState(0) == Qt::Checked );
                        target->setExpanded(true);
                    } else {
                        Log::w("ObjectInspectorTable") << "drop event, source == target";
                    }
                }
            }

            Log::d("ObjectInspectorTable") << "drop event, target = " << target->data(0, Qt::DisplayRole).toString();
        }

        event->accept();
    } else { // mauvais format mine
        Log::w("dropEvent") << "Drop non accepté, pas la bonne source";
        event->ignore();
    }
}


void ObjectInspectorTable::mouseReleaseEvent(QMouseEvent *event) {
    QTreeWidget::mouseReleaseEvent(event);
    updateMapSelection();
}


void ObjectInspectorTable::select(GraphicsObject *object) {
    m_objectMap.key(object)->setSelected(true);
    scrollToItem(m_objectMap.key(object));  // on s'assure que l'utilisateur voit la nouvelle selection
}


void ObjectInspectorTable::updateMapSelection() {
    // on vide la selection précédente
    clearMapSelection();

    // on selectionne les nouveaux éléments
    foreach(QTreeWidgetItem *item, selectedItems()) {
        if ( m_objectMap[item] ) { // on n'est pas sur un layout
            m_map->selectObject(m_objectMap[item]);
        }
    }
}


void ObjectInspectorTable::clearMapSelection() {
    m_map->clearSelection();
}


void ObjectInspectorTable::clear() {
    Log::d("ObjectInspectorTable") << "Vidage de toutes les données stockées";
    QTreeWidget::clear();
    m_objectMap.clear();
}


void ObjectInspectorTable::setAssociatedGraphicsObjectEditor(MapObjectEdit *objectEditor) {
    m_graphicsObjectEditor = objectEditor;
    m_graphicsObjectEditor->setObjectInspectorTable(this);
}


void ObjectInspectorTable::setAssociatedGraphicsAreaEditor(MapAreaEdit *areaEditor) {
    m_mapAreaEditor = areaEditor;
    m_mapAreaEditor->setObjectInspectorTable(this);
}


void ObjectInspectorTable::forceGraphicsObjectEditorUpdate() {
    selectionUpdated();
}


void ObjectInspectorTable::selectionUpdated() {

    if ( selectedItems().size() == 1 && m_objectMap[selectedItems().at(0)]) {
        Log::v("ObjectInspectorTable") << "selected object zIndex =" << m_objectMap[selectedItems().at(0)]->zValue();
    }

    if ( ! selectedItems().isEmpty() && m_objectMap[selectedItems().last()] ) {

        GraphicsObject *gObject = m_objectMap[selectedItems().last()];

        if ( gObject->mapType() == MAP_OBJECT ) {
            m_graphicsObjectEditor->setCurrentObject(dynamic_cast<MapObject*>(gObject));
            m_mapAreaEditor->setCurrentArea(0);
            emit currentSelectedItemIs(MAP_OBJECT);
            Log::d("ObjectInspectorTable") << "selectionUpdated(), object";
        }
        else if ( gObject->mapType() == MAP_AREA ) {
            m_mapAreaEditor->setCurrentArea(dynamic_cast<MapArea*>(gObject));
            m_graphicsObjectEditor->setCurrentObject(0);
            emit currentSelectedItemIs(MAP_AREA);
            Log::d("ObjectInspectorTable") << "selectionUpdated(), area";
        }
        else {
            Log::e("ObjectInspectorTable") << "Type d'objet inconnu !";
            m_graphicsObjectEditor->setCurrentObject(0);
            m_mapAreaEditor->setCurrentArea(0);
        }

    } else {
        m_graphicsObjectEditor->setCurrentObject(0);
        m_mapAreaEditor->setCurrentArea(0);
    }

    // un seul calque sélectionné, on peut le supprimer (il faut aussi qu'il reste plus d'un calque)
    emit currentLayoutIsDeletable( selectedItems().count() == 1 && ! m_objectMap[selectedItems().at(0)] && topLevelItemCount() > 1 );
}


void ObjectInspectorTable::itemWasDoubleClicked(QTreeWidgetItem *item, int column) {

    if ( column == 1 ) { // édition interdite, c'est le type !
        editItem(item); // donc l'utilisateur est invité a éditer la première colonne =)
    }

    if ( ! m_objectMap[item] ) { // on est sur un layout, le reste ne nous concerne pas
        return;
    }

    // du coup comme on edit, on veut savoir quand ce sera fini ...
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)), SLOT(editionTerminated(QTreeWidgetItem*,int)));

    // et puis on centre la vue sur l'objet
    m_map->centerOn(m_objectMap[item]);
}


void ObjectInspectorTable::editionTerminated(QTreeWidgetItem *item, int column) {

    if ( ! m_objectMap[item] ) { // c'était un calque
        return;
    }

    Log::d("ObjectInspectorTable") << "Edition terminée, l'objet à un nouveau nom : " << item->data(0, Qt::DisplayRole).toString();

    Q_UNUSED(column);
    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)), 0, 0);

    m_objectMap[item]->setScriptingName(item->data(0, Qt::DisplayRole).toString());

    // on transmet le nouveau nom à notre éditeur s'il existe
    if ( m_objectMap[item]->mapType() == MAP_OBJECT && m_graphicsObjectEditor ) {
        m_graphicsObjectEditor->setObjectScriptingName(item->data(0, Qt::DisplayRole).toString());
    }
    else if ( m_objectMap[item]->mapType() == MAP_AREA && m_mapAreaEditor ) {
        m_mapAreaEditor->setObjectScriptingName(item->data(0, Qt::DisplayRole).toString());
    }

}


void ObjectInspectorTable::LayoutItemModified(QTreeWidgetItem *item, int column) {
    Log::d("ObjectInspectorTable") << "Calque modifié, l'objet à un nouveau nom ou a été coché";

    Q_UNUSED(column);

    if ( ! m_objectMap[item] ) { // c'était un calque
        for(int i=0; i<item->childCount(); ++i) {
            m_objectMap[item->child(i)]->setVisible( item->checkState(0) == Qt::Checked );
        }

    }
}


void ObjectInspectorTable::updateObjectScriptingName(GraphicsObject *object, const QString &newName) {
    Log::d("ObjectInspectorTable") << "nouveau nom pour un objet : " << newName;
    m_objectMap.key(object)->setData(0, Qt::DisplayRole, QVariant(newName));
}


QMap<int, QString> ObjectInspectorTable::getLayouts() const {
    QMap<int, QString> layouts;
    for(int i=0; i<topLevelItemCount(); ++i) {
        layouts[-(i+1)] = topLevelItem(i)->data(0, Qt::DisplayRole).toString();
    }
    return layouts;
}


void ObjectInspectorTable::addLayout(const QString &name) {
    QStringList itemStringList;
    itemStringList << name << "";
    QTreeWidgetItem *item = new QTreeWidgetItem(itemStringList);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDropEnabled | Qt::ItemIsUserCheckable);
    addTopLevelItem(item);
    m_objectMap[item] = 0;
    item->setData(0, Qt::CheckStateRole, Qt::Checked);
}


void ObjectInspectorTable::addDefaultLayout() {
    // ajout d'au moins un calque
    addLayout("Calque par défaut");
}


void ObjectInspectorTable::removeCurrentLayout() {

    // pour le supprimer, ce doit être un layout (au top) et sans enfants, il faut aussi qu'il reste au moins un calque
    if ( selectedItems().count() != 1 || m_objectMap[selectedItems().at(0)] || selectedItems().at(0)->childCount() != 0 || topLevelItemCount() <= 1 ) {
        return;
    }

    Log::d("ObjectInspectorTable") << "Suppression du layout courant";

    QTreeWidgetItem *item = selectedItems().at(0);
    takeTopLevelItem(indexOfTopLevelItem(item));
    m_objectMap.remove(item);
    delete item;
}


void ObjectInspectorTable::upCurrentLayout() {

    // On a bien un layout ?
    if ( ! selectedItems().isEmpty() && m_objectMap[selectedItems().at(0)] ) {
        return;
    }

    Log::d("ObjectInspectorTable") << "Up du layout courant";

    bool expanded = selectedItems().at(0)->isExpanded();
    int index = indexOfTopLevelItem(selectedItems().at(0));
    QTreeWidgetItem *item = takeTopLevelItem(index);
    insertTopLevelItem(--index, item);
    item->setExpanded(expanded);
    item->setSelected(true);

    Log::i("ObjectInspectorTable") << "réajustement zIndex pour" << index << "et" << index+1;
    // réajuster zindex
    for(int i=0; i<topLevelItem(index)->childCount(); ++i) {
        m_objectMap[ topLevelItem(index)->child(i) ]->setZValue(-(index+1));
    }
    for(int i=0; i<topLevelItem(index+1)->childCount(); ++i) {
        m_objectMap[ topLevelItem(index+1)->child(i) ]->setZValue(-(index+2));
    }

    // mise à jour boutons up/down
    emit currentLayoutIsUppable( index > 0 );
    emit currentLayoutIsDownable( index < topLevelItemCount() - 1 );
}


void ObjectInspectorTable::downCurrentLayout() {

    // On a bien un layout ?
    if ( ! selectedItems().isEmpty() && m_objectMap[selectedItems().at(0)] ) {
        return;
    }

    Log::d("ObjectInspectorTable") << "Down du layout courant";

    bool expanded = selectedItems().at(0)->isExpanded();
    int index = indexOfTopLevelItem(selectedItems().at(0));
    QTreeWidgetItem *item = takeTopLevelItem(index);
    insertTopLevelItem(++index, item);
    item->setExpanded(expanded);
    item->setSelected(true);

    Log::i("ObjectInspectorTable") << "réajustement zIndex pour" << index << "et" << index-1;

    // réajuster zindex
    for(int i=0; i<topLevelItem(index)->childCount(); ++i) {
        m_objectMap[ topLevelItem(index)->child(i) ]->setZValue(-(index+1));
    }
    for(int i=0; i<topLevelItem(index-1)->childCount(); ++i) {
        m_objectMap[ topLevelItem(index-1)->child(i) ]->setZValue(-index);
    }

    // mise à jour boutons up/down
    emit currentLayoutIsUppable( index > 0 );
    emit currentLayoutIsDownable( index < topLevelItemCount() - 1 );
}


