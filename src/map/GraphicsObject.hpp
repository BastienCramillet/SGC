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


#ifndef GRAPHICSOBJECT_HPP
#define GRAPHICSOBJECT_HPP

#include "constantes.hpp"
#include <QGraphicsPixmapItem>

/*!
    \enum GraphicsObjectType
    \brief Les types d'objets possibles sur la carte
    La valeur donnée correspond à l'indice de l'éditeur de l'objet sur la pile de l'inspecteur d'objets
*/
enum GraphicsObjectType {
    MAP_OBJECT = 0,     //!< Un objet, voir MapObject
    MAP_AREA   = 1      //!< Une zone, voir MapArea
};


/*!
    \class GraphicsObject
    \brief Un objet graphique qui va venir se poser sur la carte, zone ou objet.
    D'un point de vu conception, je n'ai pas réussi à obtenir le top du top, voir http://www.siteduzero.com/forum-83-621160-p1-heritage-en-diamant.html
*/

class GraphicsObject : public QGraphicsPixmapItem {

public :

    GraphicsObject(GraphicsObjectType type);

    GraphicsObject(const GraphicsObject &other);

    /*!
        \brief Voir pattern clone
    */
    virtual GraphicsObject* clone() const = 0;

    /*!
        \brief Renvoi la position sur x à sauvegarder pour l'objet
    */
    virtual int xToSave() const = 0;

    /*!
        \brief Renvoi la position sur y à sauvegarder pour l'objet
    */
    virtual int yToSave() const = 0;

    /*!
        \brief Renvoi le nom de l'objet pour le scripting
    */
    QString scriptingName() const;

    /*!
        \brief Défini le nom de l'objet pour le scripting
    */
    void setScriptingName(const QString &name);

    /*!
        \brief Renvoi le type de l'objet
    */
    GraphicsObjectType mapType() const;

    /*!
        \brief Doit renvoyer la description associée à l'objet
    */
    virtual QString description() const = 0;


protected :

    GraphicsObjectType m_type;  //!< De quel type suis-je ?

    QString m_name;             //!< Le nom de l'objet pour le scripting

};

#endif // GRAPHICSOBJECT_HPP
