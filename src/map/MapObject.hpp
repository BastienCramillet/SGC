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


#ifndef MAPOBJECT_HPP
#define MAPOBJECT_HPP

#include "GraphicsObject.hpp"


/*!
    \class MapObject
    \brief Un objet qui va sur la carte
*/

class MapObject : public GraphicsObject {

public:

    /*!
        \param objectID L'identifiant de l'objet
        \param position La position du point en haut à gauche de l'objet
    */
    MapObject(const QString &objectID, const QPoint &position);

    MapObject(const MapObject &other);

    virtual GraphicsObject* clone() const;

    /*!
        \brief Défini l'image à utiliser comme image onCreate()
    */
    void setImageIDOnCreate(const QString &imageId);

    /*!
        \brief Renvoi l'id de l'objet
    */
    QString objectID() const;

    /*!
        \brief Renvoi l'id de l'image utilisée onCreate
    */
    QString imageOnCreateID() const;

    /*!
        \brief Renvoi la position sur x du centre de l'objet
    */
    int xToSave() const;

    /*!
        \brief Renvoi la position sur y du centre de l'objet
    */
    int yToSave() const;

    /*!
        \brief L'objet est destructible dans le jeu ?
    */
    bool isDestructible() const;

    /*!
        \brief Défini si l'objet est destructible dans le jeu
    */
    void setDestructible(bool destructible);

    /*!
        \brief L'objet est movible dans le jeu ?
    */
    bool isMovable() const;

    /*!
        \brief Défini si l'objet est movible dans le jeu
    */
    void setMovable(bool movable);

    /*!
        \brief L'objet est en arrière plan dans le jeu ?
    */
    bool isOnBackground() const;

    /*!
        \brief Défini si l'objet est en arrière plan dans le jeu
    */
    void setOnBackground(bool onBackground);

    /*!
        \brief Renvoi la résistance (vie) de l'objet
    */
    int resistance() const;

    /*!
        \brief Défini la résistance de l'objet
    */
    void setResistance(int resistance);

    /*!
        \brief Renvoi la densité de l'objet
    */
    float density() const;

    /*!
        \brief Défini la densité de l'objet
    */
    void setDensity(float density);

    /*!
        \brief Renvoi la friction de l'objet
    */
    float friction() const;

    /*!
        \brief Défini la friction de l'objet
    */
    void setFriction(float friction);

    /*!
        \brief Renvoi la description de l'objet
    */
    QString description() const;


protected :

    QString m_objectId;         //!< Identifiant de l'objet

    QString m_imageIdOnCreate;  //!< L'identifiant de l'image utilisée onCreate

    bool m_isDestructible;      //!< Cet objet est destructible dans le jeu ?

    bool m_isMovable;           //!< Cet objet est movible dans le jeu?

    bool m_isOnBackground;      //!< Cet objet est en arrière plan dans le jeu ?

    int m_resistance;           //!< La résistance (ou vie) de cet objet

    float m_density;            //!< La densité de cet objet

    float m_friction;           //!< La friction de cet objet

};

#endif // MAPOBJECT_HPP
