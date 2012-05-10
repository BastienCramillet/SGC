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


#ifndef MAPAREA_HPP
#define MAPAREA_HPP

#include "GraphicsObject.hpp"

#include "AreasData.hpp"

/*!
    \class MapArea
    \brief Une zone de la carte
*/

class MapArea : public GraphicsObject {

public:

    MapArea(const AreasData &datas, const QPoint &position);

    MapArea(const MapArea &other);

    virtual GraphicsObject* clone() const;

    /*!
        \brief Renvoi la position sur x du point en haut à gauche de l'objet
    */
    int xToSave() const;

    /*!
        \brief Renvoi la position sur y du point en haut à gauche de l'objet
    */
    int yToSave() const;

    /*!
        \brief Renvoi la description (le nom) de la zone
    */
    QString description() const;

    /*!
        \brief Redimmensionne la zone avec la taille donnée
    */
    void resize(const QSize &size);

    /*!
        \brief Renvoi la couleur de la zone
    */
    QColor color() const;


protected :

    AreasData m_areasData;      //!< Les données de cette zone


};

#endif // MAPAREA_HPP
