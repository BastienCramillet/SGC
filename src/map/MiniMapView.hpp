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


#ifndef MINIMAPVIEW_HPP
#define MINIMAPVIEW_HPP

#include "constantes.hpp"

class Map;

const int miniMapViewSize = 200;

/*!
    \class MiniMapView
    \brief Un aperçu de la vrai map, en mode tout mini
*/

class MiniMapView : public QGraphicsView {

Q_OBJECT

public:

    MiniMapView(Map *map);

    /*!
        \brief Mise à jour de la mini carte pour qu'elle englobe toute la carte
    */
    void rescaleView();


protected :

    /*!
        \brief Clique sur la mini carte
    */
    void mousePressEvent(QMouseEvent *event);


    Map *m_map;         //!< La carte que je représente


private :

    float m_factor;     //!< Le dernier facteur de resize utilisé, pour l'annuler

};

#endif // MINIMAPVIEW_HPP
