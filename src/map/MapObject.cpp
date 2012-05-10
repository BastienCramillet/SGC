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


#include "MapObject.hpp"

#include "ObjectManager.hpp"


MapObject::MapObject(const QString &objectID, const QPoint &position)
    : GraphicsObject(MAP_OBJECT), m_objectId(objectID) {

    Log::d("GraphicsObject") << "création d'un nouvel objet sur la carte";

    ObjectData *objectData = ObjectManager::getObjectData(objectID);

    // on récupère les valeurs par défaut
    m_isDestructible    = objectData->isDestructible;
    m_isMovable         = objectData->isMovable;
    m_isOnBackground    = objectData->isOnBackground;
    m_resistance        = objectData->resistance;
    m_density           = objectData->density;
    m_friction          = objectData->friction;

    // Et on met une image
    m_imageIdOnCreate = objectData->getImageDataID();
    setImageIDOnCreate(m_imageIdOnCreate);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setPos(position);
}


MapObject::MapObject(const MapObject &other) : GraphicsObject(other) {

    Log::d("GraphicsObject") << "recopie d'un objet existant";

    m_imageIdOnCreate = other.m_imageIdOnCreate;
    m_objectId = other.m_objectId;

    // on récupère les valeurs de l'autre objet
    m_isDestructible    = other.m_isDestructible;
    m_isMovable         = other.m_isMovable;
    m_isOnBackground    = other.m_isOnBackground;
    m_resistance        = other.m_resistance;
    m_density           = other.m_density;
    m_friction          = other.m_friction;

    // on gère l'image
    setImageIDOnCreate(m_imageIdOnCreate);
}


GraphicsObject* MapObject::clone() const {
    return new MapObject(*this);
}


void MapObject::setImageIDOnCreate(const QString &imageId) {
    m_imageIdOnCreate = imageId;
    setPixmap(ObjectManager::getObjectData(m_objectId)->getImage(m_imageIdOnCreate));
    setTransformOriginPoint(pixmap().width()/2, pixmap().height()/2);
}


QString MapObject::objectID() const {
    return m_objectId;
}


QString MapObject::imageOnCreateID() const {
    return m_imageIdOnCreate;
}


int MapObject::xToSave() const {
    return QGraphicsPixmapItem::x() + pixmap().width() / 2;
}


int MapObject::yToSave() const {
    return QGraphicsPixmapItem::y() + pixmap().height() / 2;
}



bool MapObject::isDestructible() const {
    return m_isDestructible;
}


void MapObject::setDestructible(bool destructible) {
    m_isDestructible = destructible;
}


bool MapObject::isMovable() const {
    return m_isMovable;
}


void MapObject::setMovable(bool movable) {
    m_isMovable = movable;
}


bool MapObject::isOnBackground() const {
    return m_isOnBackground;
}


void MapObject::setOnBackground(bool onBackground) {
    m_isOnBackground = onBackground;
}


int MapObject::resistance() const {
    return m_resistance;
}


void MapObject::setResistance(int resistance) {
    m_resistance = resistance;
}


float MapObject::density() const {
    return m_density;
}


void MapObject::setDensity(float density) {
    m_density = density;
}


float MapObject::friction() const {
    return m_friction;
}


void MapObject::setFriction(float friction) {
    m_friction = friction;
}


QString MapObject::description() const {
    return ObjectManager::getObjectData(m_objectId)->description;
}
