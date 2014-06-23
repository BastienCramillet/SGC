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


#include "Map.hpp"

#include <QtXml>

#include "MapEditor.hpp"
#include "ObjectPalette.hpp"
#include "ObjectInspectorTable.hpp"

#include "EditMapAttributes.hpp"

#include "MapObject.hpp"
#include "MapArea.hpp"

#include "MiniMapView.hpp"

#include "Settings.hpp"

#include "ObjectManager.hpp"

#include <QMouseEvent>
#include <QMessageBox>


Map::Map(MapEditor *parent)
    : QGraphicsView(parent), m_width(800), m_height(600), m_parent(parent),
    m_isShiftKeyDown(false), m_alignOnGrid(true), m_objectInspectorTable(0), m_miniMapView(0)
{

    // je met ça en esperant que cela corrigera quelques bugs de raffraichissement et que cela améliorera l'affichage
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    // association de la scene
    setScene(&m_scene);

    drawGrid();
}


void Map::resize(int width, int height) {

    m_width = width;
    m_height = height;

    drawGrid();

    Log::d("Map") << "resizing" << m_width << m_height;

    // si la minimapview existe elle se met à jour
    if ( m_miniMapView ) {
        m_miniMapView->rescaleView();
    }
}


void Map::drawGrid() {

    // on supprime l'ancienne grille
    foreach(QGraphicsItem *currentItem, m_gridLineSet) {
        delete currentItem;
    }
    m_gridLineSet.clear();

    // tracer la nouvelle grille
    QPen pen(QColor(210, 210, 210), 1, Qt::SolidLine);
    QPen pen2(QColor(150, 150, 150), 1, Qt::SolidLine); // un stylo un peu plus foncé

    for (int i = 0; i <= m_width; i+= GRID_SPACE) {
        if ( i%(5*GRID_SPACE) == 0 ) { // toutes les 5 cases on fait un trait plus foncé
            m_gridLineSet << m_scene.addLine(i, 0, i, m_height, pen2);
        } else {
            m_gridLineSet << m_scene.addLine(i, 0, i, m_height, pen);
        }
    }
    for (int j = 0; j <= m_height; j+= GRID_SPACE) {
        if ( j%(5*GRID_SPACE) == 0 ) { // toutes les 5 cases on fait un trait plus foncé
            m_gridLineSet << m_scene.addLine(0, j, m_width, j, pen2);
        } else {
            m_gridLineSet << m_scene.addLine(0, j, m_width, j, pen);
        }
    }

    foreach(QGraphicsItem *currentItem, m_gridLineSet) {
        currentItem->setFlag(QGraphicsItem::ItemStacksBehindParent, true);  // un peu beaucoup caché
        currentItem->setFlag(QGraphicsItem::ItemAcceptsInputMethod, false); // on ne touche pas à cette ligne !
    }
}


void Map::clear() {

    m_objectInspectorTable->clear();

    // suppression des objets
    foreach(GraphicsObject *currentObject, m_objectSet) {
        m_objectSet.remove(currentObject);
        delete currentObject;
    }
}


void Map::wheelEvent(QWheelEvent *event) {

    QList<QGraphicsItem *> selectedItems = m_scene.selectedItems();

    if ( ! selectedItems.empty() ) { // des éléments sont sélectionnés

        foreach(QGraphicsItem *currentItem, selectedItems) {

            if ( static_cast<GraphicsObject*>(currentItem)->mapType() != MAP_AREA ) { // sinon la rotation est interdite

                if ( event->orientation() == Qt::Vertical && event->delta() < 0 ) {
                    currentItem->setRotation( currentItem->rotation() + 5 );
                } else if ( event->orientation() == Qt::Vertical && event->delta() > 0 ){
                    currentItem->setRotation( currentItem->rotation() - 5 );
                }
            }
        }

        m_objectInspectorTable->forceGraphicsObjectEditorUpdate();  // pour mettre à jour la rotation

    }/*
    else if ( m_isShiftKeyDown ) { // tentative d'implémentation du zoom
        if ( event->orientation() == Qt::Vertical && event->delta() < 0 ) {
            scale(0.9, 0.9);
        } else if ( event->orientation() == Qt::Vertical && event->delta() > 0 ){
            scale(1.1, 1.1);
        }
    }*/
    else { // aucun item n'était selectionné, on scroll normalement
        QGraphicsView::wheelEvent(event);
    }
}


void Map::mousePressEvent(QMouseEvent *event) {

    if (m_isShiftKeyDown) { // ajout d'un objet sur la carte
        try {

            GraphicsObject *newObject;

            if ( m_parent->getObjectPalette()->selectedObjectType() == MAP_OBJECT ) {
                QString objectID = m_parent->getObjectPalette()->selectedObjectID();
                ObjectData *objectData = ObjectManager::getObjectData(objectID);
                newObject = new MapObject(objectID, mapToScene(event->pos()).toPoint() - QPoint(objectData->width/2, objectData->height/2));
            }
            else if ( m_parent->getObjectPalette()->selectedObjectType() == MAP_AREA ) {
                AreasData datas = m_parent->getObjectPalette()->selectedAreaDatas();
                newObject = new MapArea(datas, mapToScene(event->pos()).toPoint() - QPoint(datas.size.width()/2, datas.size.height()/2));
            }
            else {
                Log::e("Map::mousePressEvent") << "Type d'objet selectionné invalide";
            }

            m_objectSet << newObject;
            m_scene.addItem(newObject);
            m_objectInspectorTable->addRow(newObject);

            if ( m_alignOnGrid ) { // il faut aligner sur la grille
                realignOnGrid(newObject);
            }

        } catch(...) {
        }
    } else { // on traite l'évenement de façon classique
        QGraphicsView::mousePressEvent(event);
    }

    commitNewSelection();   // pour que ce soit géré par l'inspecteur
}


void Map::mouseReleaseEvent(QMouseEvent *event) {

    // déjà on continu le comportement classique
    QGraphicsView::mouseReleaseEvent(event);

    commitNewSelection();   // pour que ce soit géré par l'inspecteur

    // faut-il réaligner ?
    if ( ! m_alignOnGrid ) {
        return;
    }

    foreach(QGraphicsItem *currentItem, m_scene.selectedItems()) {
        realignOnGrid(static_cast<GraphicsObject *>(currentItem));
    }

    m_objectInspectorTable->forceGraphicsObjectEditorUpdate();
}


void Map::showGrid(bool show) {
    foreach(QGraphicsLineItem *currentObject, m_gridLineSet) {
        currentObject->setVisible(show);
    }
}


void Map::paletteItemChanged() {
    setShiftKeyDown(m_isShiftKeyDown);
}


void Map::setShiftKeyDown(bool isDown) {
    m_isShiftKeyDown = isDown;
    if (m_isShiftKeyDown) { // curseur objet à placer
        try {
            if ( m_parent->getObjectPalette()->selectedObjectType() == MAP_OBJECT ) {
                ObjectData *objectData = ObjectManager::getObjectData(m_parent->getObjectPalette()->selectedObjectID());
                setCursor(QCursor(objectData->getImage()));
            }
            else if ( m_parent->getObjectPalette()->selectedObjectType() == MAP_AREA ) {
                AreasData datas = m_parent->getObjectPalette()->selectedAreaDatas();
                QPixmap cursorPixmap(datas.size);
                cursorPixmap.fill(datas.color);
                setCursor(QCursor(cursorPixmap));
            }
            else {
                Log::e("Map::setShiftKeyDown") << "Type d'objet selectionné invalide";
            }

        } catch(...) {
            setCursor(QCursor());
        }
    } else { // curseur normal
        setCursor(QCursor());
    }
}


void Map::dropEvent(QDropEvent *event) {

    // drop d'objet
    if ( event->mimeData()->hasFormat("data/x-objectID") ) {

        // voir http://doc.trolltech.com/4.4/draganddrop-draggableicons-dragwidget-cpp.html
        QByteArray itemData = event->mimeData()->data("data/x-objectID");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString objectID;
        dataStream >> objectID;
        Log::i("dropEvent") << "Objet id jeté sur la carte :" << objectID;

        GraphicsObject *newObject = new MapObject(objectID, mapToScene(event->pos()).toPoint());
        m_objectSet << newObject;
        m_scene.addItem(newObject);
        m_objectInspectorTable->addRow(newObject);

        if ( m_alignOnGrid ) { // il faut aligner sur la grille
            realignOnGrid(newObject);
        }

        event->accept();
    }

    // drop de zone
    else if ( event->mimeData()->hasFormat("data/x-area") )  {

        QByteArray itemData = event->mimeData()->data("data/x-area");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        AreasData ad;
        dataStream >> ad;
        Log::i("dropEvent") << "Nom zone jetée sur la carte :" << ad.name;

        GraphicsObject *newObject = new MapArea(ad, event->pos() - mapFromScene(0, 0));
        m_objectSet << newObject;
        m_scene.addItem(newObject);
        m_objectInspectorTable->addRow(newObject);

        if ( m_alignOnGrid ) { // il faut aligner sur la grille
            realignOnGrid(newObject);
        }

        event->accept();
    }

    else { // mauvais format mime
        Log::w("dropEvent") << "Un format mime non accepté ?";
        event->ignore();
    }
}


void Map::dragEnterEvent(QDragEnterEvent *event) {
    if ( event->mimeData()->hasFormat("data/x-objectID") || event->mimeData()->hasFormat("data/x-area") ) {
        event->accept();
    } else { // mauvais format mine
        Log::w("dropEvent") << "Un format mime non accepté ?";
        event->ignore();
    }
}


void Map::dragMoveEvent(QDragMoveEvent *event) {
    if ( event->mimeData()->hasFormat("data/x-objectID") || event->mimeData()->hasFormat("data/x-area") ) {
        event->accept();
    } else { // mauvais format mime
        Log::w("dropEvent") << "Un format mime non accepté ?";
        event->ignore();
    }
}


void Map::deleteSelectedItems() {

    Log::i("map") << "destruction des objets selectionnés";

    foreach(QGraphicsItem *currentItem, m_scene.selectedItems()) {
        m_objectSet.remove(static_cast<GraphicsObject *>(currentItem));
        m_objectInspectorTable->removeRow(static_cast<GraphicsObject *>(currentItem));
        delete currentItem;
    }
}


int Map::width() const {
    return m_width;
}


int Map::height() const {
    return m_height;
}


QString Map::mapName() const {
    return m_mapName;
}


void Map::setMapName(const QString &name) {
    m_mapName = name;
}


void Map::cut() {
    QList<QGraphicsItem *> selectedItems = m_scene.selectedItems();

    if ( ! selectedItems.empty() ) { // des éléments sont sélectionnés

        Log::i("map") << "coupe des objets selectionnés (" << selectedItems.size() << ")";

        clearBuffer();

        foreach(QGraphicsItem *currentItem, selectedItems) {
            GraphicsObject *currentGraphicsObject = static_cast<GraphicsObject *>(currentItem);
            m_scene.removeItem(currentItem);
            m_objectSet.remove(currentGraphicsObject);
            m_objectInspectorTable->removeRow(currentGraphicsObject);
            m_buffer << currentGraphicsObject;
        }
    }
}


void Map::copy() {
    QList<QGraphicsItem *> selectedItems = m_scene.selectedItems();

    if ( ! selectedItems.empty() ) { // des éléments sont sélectionnés

        Log::i("map") << "copie des objets selectionnés (" << selectedItems.size() << ")";

        clearBuffer();

        foreach(QGraphicsItem *currentItem, selectedItems) {
            m_buffer << (static_cast<GraphicsObject *>(currentItem))->clone();
        }
    }
}


void Map::paste() {

    Log::i("map") << "collage sur la carte des objets dans le buffer (" << m_buffer.size() << ")";

    // déselection des objets précédemment sélectionnés
    m_scene.clearSelection();

    // ajout des objets dans le buffer
    foreach(GraphicsObject *currentGraphicsObject, m_buffer) {
        GraphicsObject *newGraphicsObject = currentGraphicsObject->clone();
        m_objectSet << newGraphicsObject;
        m_scene.addItem(newGraphicsObject);
        m_objectInspectorTable->addRow(newGraphicsObject);
        newGraphicsObject->setSelected(true);
    }
}


void Map::clearBuffer() {
    Log::d("map") << "vidage du buffer de copie";
    foreach(QGraphicsItem *currentItem, m_buffer) {
        delete currentItem;
    }
    m_buffer.clear();
}


void Map::save(const QString &name) const {

    Log::i("Map") << "sauvegarde de la carte dans : " << MAPS_DIR << name;

    QDomDocument doc("CARTE");
    QDomElement root = doc.createElement("map");

    root.setAttribute("name", m_mapName);
    //root.setAttribute("carteSuivante", nextMap);
    root.setAttribute("width", m_width);
    root.setAttribute("height", m_height);
    //root.setAttribute("arrierePlan", background);

    doc.appendChild(root);

    QSet<QString> objectsToLoad;

    // création du noeud "pour l'éditeur"
    QDomElement editorsData = doc.createElement("editorsData");

    // les calques
    QDomElement layouts = doc.createElement("layouts");

    QMap<int, QString> layoutsNames = m_objectInspectorTable->getLayouts();
    foreach(int id, layoutsNames.keys()) {
        QDomElement layout = doc.createElement("layout");
        layout.setAttribute("id", id);
        layout.setAttribute("name", layoutsNames[id]);
        layouts.appendChild(layout);
    }

    // ajout des enfants de "pour l'éditeur"
    editorsData.appendChild(layouts);
    root.appendChild(editorsData);


    // quels objets faut-il charger ?
    foreach(GraphicsObject *currentObject, m_objectSet) {
        if ( currentObject->mapType() == MAP_OBJECT ) {
            objectsToLoad << dynamic_cast<MapObject*>(currentObject)->objectID();
        }
    }

    QDomElement domObjectToLoad = doc.createElement("objects");

    // ajout des ID des objets à charger dans le xml
    foreach(QString i, objectsToLoad) {
        QDomElement currentNode = doc.createElement("object");
        currentNode.setAttribute("id", i);
        domObjectToLoad.appendChild(currentNode);
    }

    // musiques à charger, rien pour l'instant
    QDomElement domMusicsToLoad = doc.createElement("musics");

    // ajout des enfants à charger
    QDomElement toLoad = doc.createElement("load");
    toLoad.appendChild(domObjectToLoad);
    toLoad.appendChild(domMusicsToLoad);
    root.appendChild(toLoad);


    // objets de la carte
    QDomElement objects = doc.createElement("objects");

    // zones de la carte
    QDomElement areas = doc.createElement("areas");

    foreach(GraphicsObject *currentObject, m_objectSet) {

        if ( currentObject->mapType() == MAP_OBJECT ) {

            MapObject *object = dynamic_cast<MapObject*>(currentObject);

            QDomElement itemNode = doc.createElement("object");

            // partie générique
            itemNode.setAttribute("x", currentObject->xToSave());
            itemNode.setAttribute("y", currentObject->yToSave());
            itemNode.setAttribute("name", currentObject->scriptingName());
            itemNode.setAttribute("rotation", static_cast<int>(currentObject->rotation()) % 360);
            itemNode.setAttribute("layout", currentObject->zValue());

            // et là pour le coup c'est moins générique ...
            itemNode.setAttribute("id", object->objectID());
            itemNode.setAttribute("imageOnCreateID", object->imageOnCreateID());
            itemNode.setAttribute("isDestructible", object->isDestructible() ? "true" : "false");
            itemNode.setAttribute("isMovable", object->isMovable() ? "true" : "false");
            itemNode.setAttribute("isOnBackground", object->isOnBackground() ? "true" : "false");
            itemNode.setAttribute("resistance", object->resistance());
            itemNode.setAttribute("density", object->density());
            itemNode.setAttribute("friction", object->friction());

            objects.appendChild(itemNode);
        }
        else if ( currentObject->mapType() == MAP_AREA ) {

            MapArea *area = dynamic_cast<MapArea*>(currentObject);

            QDomElement itemNode = doc.createElement("area");

            // partie générique
            itemNode.setAttribute("x", currentObject->xToSave());
            itemNode.setAttribute("y", currentObject->yToSave());
            itemNode.setAttribute("name", currentObject->scriptingName());
            itemNode.setAttribute("layout", currentObject->zValue());
            itemNode.setAttribute("width", currentObject->pixmap().width());
            itemNode.setAttribute("height", currentObject->pixmap().height());
            itemNode.setAttribute("description", currentObject->description());

            // et là pour le coup c'est moins générique ...
            itemNode.setAttribute("color", area->color().name());

            areas.appendChild(itemNode);

        }
        else {
            Log::e("Map") << "Saving, but but but... this type is unknown ! WTF ?";
        }
    }



    // ajout objets & zones dans le xml
    QDomElement items = doc.createElement("items");
    items.appendChild(objects);
    items.appendChild(areas);
    root.appendChild(items);

    // sauvegarde dans le fichier
    QFile file(QString(MAPS_DIR) + name);
    if( ! file.open(QIODevice::WriteOnly) ) {
        Log::e("Map") << "Impossible d'ouvrir le fichier pour sauvegarde :" << file.errorString();
        QMessageBox::critical(0, "Erreur", "Impossible d'ouvrir le fichier pour sauvegarder la carte.");
        return;
    }

    QTextStream ts(&file);
    ts << doc.toString();

    file.close();

    Log::d("Map") << "sauvegarde terminée";

}


void Map::load(const QString &name) {

    Log::i("Map") << "chargement de la carte dans : " << MAPS_DIR << name;

    // vider la carte précédente
    clear();

    // lecture du fichier xml
    QFile f(QString(MAPS_DIR) + name);
    if ( ! f.open(QIODevice::ReadOnly) ) {
        Log::e("map") << "Erreur au chargement de la carte : " << f.errorString();
        QMessageBox::critical(0, "Erreur", "Impossible d'ouvrir le fichier à charger");
        return;
    }

    QDomDocument doc;
    doc.setContent(&f);
    f.close();

    QDomElement root = doc.documentElement();

    // attributs à la racine
    resize(root.attribute("width").toInt(), root.attribute("height").toInt());
    m_mapName = root.attribute("name", "");

    // on parcourt le document à la recherche des objets à placer sur la carte
    QDomNode n = root.firstChild();
    while ( ! n.isNull() ) {

        QDomElement childElement = n.toElement();

        // les infos pour l'éditeur
        if ( childElement.tagName() == "editorsData" ) {

            Log::v("map") << "noeud contenant les infos trouvé";

            QDomNodeList editorsData = childElement.childNodes();

            for (int i=0; i<editorsData.size(); ++i) {

                // les calques
                if ( editorsData.at(i).toElement().tagName() == "layouts" ) {

                    Log::v("map") << "on a trouvé les calques à charger !";

                    QDomNodeList layouts = editorsData.at(i).childNodes();

                    QMap<int, QString> editorsLayouts;  // pour classer les layouts dans l'ordre
                    for (int j=0; j<layouts.size(); ++j) {
                        editorsLayouts[layouts.at(j).toElement().attribute("id").toInt()] = layouts.at(j).toElement().attribute("name");
                    }
                    QMapIterator<int, QString> it(editorsLayouts);
                    it.toBack();
                    while (it.hasPrevious()) {
                        it.previous();
                        m_objectInspectorTable->addLayout(it.value());
                    }
                }
                /*
                // les quelque chose, dans le futur =)
                else if ( editorsData.at(i).toElement().tagName() == "une autre donnée pour l'éditeur" ) {

                }
                */

            }
        }

        // on a trouvé les items qui vont sur la carte
        else if ( childElement.tagName() == "items") {

            QDomNodeList items = childElement.childNodes();

            for (int i=0; i<items.size(); ++i) {

                // objets à charger
                if ( items.at(i).toElement().tagName() == "objects" ) {
                    Log::v("map") << "noeud contenant les différents objets trouvé";

                    QDomNode objectNode = items.at(i).firstChild();
                    while ( ! objectNode.isNull() ) { // pour chaque objet

                        QDomElement objectElement = objectNode.toElement();
                        Log::v("map") << "Un objet trouvé à mettre sur la carte" << objectElement.attribute("id");

                        // récupération des données de l'image de l'objet (pour bien la placer)
                        ObjectData *objectData = ObjectManager::getObjectData(objectElement.attribute("id"));

                        MapObject *newGraphicsObject = new MapObject(
                                                                objectElement.attribute("id"), // id Objet
                                                                QPoint( // position
                                                                        objectElement.attribute("x").toFloat(), objectElement.attribute("y").toFloat())
                                                                        - QPoint(objectData->width/2, objectData->height/2
                                                                      )
                                                            );
                        newGraphicsObject->setScriptingName(objectElement.attribute("name"));

                        newGraphicsObject->setImageIDOnCreate(objectElement.attribute("imageOnCreateID"));
                        newGraphicsObject->setRotation(objectElement.attribute("rotation").toFloat());
                        newGraphicsObject->setScriptingName(objectElement.attribute("name"));
                        newGraphicsObject->setDestructible( objectElement.attribute("isDestructible") == "true" );
                        newGraphicsObject->setMovable( objectElement.attribute("isMovable") == "true" );
                        newGraphicsObject->setOnBackground( objectElement.attribute("isOnBackground") == "true" );
                        newGraphicsObject->setResistance(objectElement.attribute("resistance").toInt());
                        newGraphicsObject->setDensity(objectElement.attribute("density").toFloat());
                        newGraphicsObject->setFriction(objectElement.attribute("friction").toFloat());

                        m_objectSet << newGraphicsObject;
                        m_scene.addItem(newGraphicsObject);
                        m_objectInspectorTable->addRow(newGraphicsObject, objectElement.attribute("layout").toInt());

                        objectNode = objectNode.nextSibling();
                    }
                }

                // zones à charger
                else if ( items.at(i).toElement().tagName() == "areas" ) {

                    Log::v("map") << "noeud contenant les différentes zones trouvé";

                    QDomNode areaNode = items.at(i).firstChild();
                    while ( ! areaNode.isNull() ) { // pour chaque objet

                        QDomElement areaElement = areaNode.toElement();
                        Log::v("map") << "Une zone trouvée à mettre sur la carte" << areaElement.attribute("description");

                        AreasData ad;
                        ad.color = QColor(areaElement.attribute("color"));
                        ad.name = areaElement.attribute("description");
                        ad.size = QSize(areaElement.attribute("width").toInt(), areaElement.attribute("height").toInt());

                        MapArea *newGraphicsObject = new MapArea(
                                                                ad, // données de la zone
                                                                QPoint( // position
                                                                        areaElement.attribute("x").toFloat(), areaElement.attribute("y").toFloat()
                                                                      )
                                                            );
                        newGraphicsObject->setScriptingName(areaElement.attribute("name"));

                        m_objectSet << newGraphicsObject;
                        m_scene.addItem(newGraphicsObject);
                        m_objectInspectorTable->addRow(newGraphicsObject, areaElement.attribute("layout").toInt());

                        areaNode = areaNode.nextSibling();
                    }
                }

            }
        }

        n = n.nextSibling();
    }

    Log::d("Map") << "chargement terminé";

}


void Map::setAlignOnGrid(bool align) {
    Log::v("map") << "Aligner sur la grille" << align;
    m_alignOnGrid = align;
}


void Map::setObjectInspectorTable(ObjectInspectorTable *inspectorTable) {
    m_objectInspectorTable = inspectorTable;
}


void Map::commitNewSelection() {
    m_objectInspectorTable->clearSelection();
    foreach(QGraphicsItem *currentItem, m_scene.selectedItems()) {
        m_objectInspectorTable->select(static_cast<GraphicsObject *>(currentItem));
    }
}


void Map::clearSelection() {
    m_scene.clearSelection();
}


void Map::selectObject(GraphicsObject *object) {
    (*m_objectSet.find(object))->setSelected(true);
}


void Map::realignOnGrid(GraphicsObject *object) {
    object->setX( object->x() - (static_cast<int>(object->x()) % GRID_SPACE) );
    object->setY( object->y() - (static_cast<int>(object->y()) % GRID_SPACE) );
}


void Map::newMap() {
    clear();
    m_objectInspectorTable->addDefaultLayout();
}


void Map::openEditMapAttributes() {
    new EditMapAttributes(this);
}


void Map::setMiniMapView(MiniMapView *miniMapView) {
    m_miniMapView = miniMapView;
}

