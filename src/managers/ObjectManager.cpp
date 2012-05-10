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


#include "ObjectManager.hpp"

#include <QtXml>

// création de l'instance de ObjectManager partagée
ObjectManager ObjectManager::m_sharedObjectManager;


ObjectManager::ObjectManager() {
    loadXml();
}


ObjectManager::~ObjectManager() {
    for(QMap<QString, ObjectData *>::iterator it = m_ressources.begin(); it != m_ressources.end(); ++it) {
        delete it.value();
    }
}


void ObjectManager::loadXml() {

    QDomDocument doc;

    QFile f(DATA_DIR + "/objects.xml");
    if ( ! f.open(QIODevice::ReadOnly) ) {
        Log::e("ObjectManager") << "Impossible d'ouvrir" << f.fileName() << ":" << f.errorString();
        return;
    }
    doc.setContent(&f);
    f.close();

    QDomNode n = doc.documentElement().firstChild();

    while( ! n.isNull() ) {

        // informations générales sur l'objet
        ObjectData *objectData = new ObjectData;
        objectData->id              = n.toElement().attribute("id");
        objectData->description     = n.toElement().attribute("description");
        objectData->width           = n.toElement().attribute("width", "0").toInt();
        objectData->height          = n.toElement().attribute("height", "0").toInt();
        objectData->gameClass       = n.toElement().attribute("gameClass");
        objectData->gameFactoryInfo = n.toElement().attribute("gameFactoryInfo");
        objectData->editorCategory  = n.toElement().attribute("editorCategory");

        objectData->isDestructible  = ( n.toElement().attribute("isDestructible") == "true" );
        objectData->isOnBackground  = ( n.toElement().attribute("isOnBackground") == "true" );
        objectData->isMovable       = ( n.toElement().attribute("isMovable") == "true" );
        objectData->resistance      = n.toElement().attribute("resistance", "1").toInt();
        objectData->density         = n.toElement().attribute("density", "1").toFloat();
        objectData->friction        = n.toElement().attribute("friction", "1").toFloat();

        // récupération des sous-éléments (images, sons, éphémères)
        QDomNodeList childNodes = n.childNodes();
        for (int i=0; i<childNodes.count(); ++i) {

            QDomElement e = childNodes.at(i).toElement();

            // images
            if ( e.tagName() == "images" ) {
                QDomNodeList imageNodes = e.childNodes();
                for (int j=0; j<imageNodes.count(); ++j) {
                    QDomElement domNodeElement = imageNodes.at(j).toElement();
                    objectData->images.insert(domNodeElement.attribute("triggered"), domNodeElement.attribute("id"));
                }
            }
            // sons
            else if ( e.tagName() == "sounds" ) {
                QDomNodeList soundNodes = e.childNodes();
                for (int j=0; j<soundNodes.count(); ++j) {
                    QDomElement domNodeElement = soundNodes.at(j).toElement();
                    objectData->sounds.insert(domNodeElement.attribute("triggered"), domNodeElement.attribute("id"));
                }
            }
            // éphémères
            //else if ( e.tagName() == "ephemerals") {
            //
            //}

        }


        m_ressources[objectData->id] = objectData;

        n = n.nextSibling();
    }

}


ObjectData* ObjectManager::getObjectData(const QString &id) {
    return m_sharedObjectManager.m_ressources[id];
}


QMap<QString, ObjectData *> ObjectManager::getObjectMap() {
    return m_sharedObjectManager.m_ressources;
}


