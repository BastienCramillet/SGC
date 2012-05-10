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


#include "MapArea.hpp"

MapArea::MapArea(const AreasData &datas, const QPoint &position)
    : GraphicsObject(MAP_AREA), m_areasData(datas)
{

    Log::d("MapArea") << "création d'une zone sur la carte";

    // Et on met une image
    QPixmap myPixmap(m_areasData.size);
    myPixmap.fill(m_areasData.color);
    setPixmap(myPixmap);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setPos(position);
}


MapArea::MapArea(const MapArea &other) : GraphicsObject(other) {
    Log::d("MapArea") << "recopie d'un objet existant";
    m_areasData = other.m_areasData;
    setPixmap(other.pixmap());
}


GraphicsObject* MapArea::clone() const {
    return new MapArea(*this);
}


int MapArea::xToSave() const {
    return QGraphicsPixmapItem::x();
}


int MapArea::yToSave() const {
    return QGraphicsPixmapItem::y();
}


QString MapArea::description() const {
    return m_areasData.name;
}


void MapArea::resize(const QSize &size) {
    m_areasData.size = size;
    QPixmap myPixmap(size);
    myPixmap.fill(m_areasData.color);
    setPixmap(myPixmap);
}


QColor MapArea::color() const {
    return m_areasData.color;
}

