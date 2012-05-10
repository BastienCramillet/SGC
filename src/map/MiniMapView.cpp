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


#include "MiniMapView.hpp"

#include "Map.hpp"

MiniMapView::MiniMapView(Map *map)
    : m_map(map), m_factor(0)
{
    setScene(map->scene());
    setFixedSize(miniMapViewSize, miniMapViewSize);

    horizontalScrollBar()->setVisible(false);
    verticalScrollBar()->setVisible(false);
    setInteractive(false);

    map->setMiniMapView(this);
    rescaleView();
}


void MiniMapView::rescaleView() {
    Log::i("MiniMapView") << "I'm rescaling !" << m_map->width() << m_map->height();
    int max = std::max(m_map->width(), m_map->height());

    if ( m_factor != 0 ) { // annulation du scale précédent
        scale(m_factor, m_factor);
        setSceneRect(0, 0, m_map->width(), m_map->height());
    }

    m_factor = max / miniMapViewSize;
    scale(1/m_factor, 1/m_factor);
}


void MiniMapView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
    QPointF realPoint = mapToScene(event->posF().toPoint());
    Log::i("MiniMapView") << "pressEvent, at" << realPoint.x() << realPoint.y();
    m_map->centerOn(realPoint);
}

