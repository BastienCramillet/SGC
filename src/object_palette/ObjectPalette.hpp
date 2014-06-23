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


#ifndef OBJECTPALETTE_HPP
#define OBJECTPALETTE_HPP

#include "constantes.hpp"

#include "GraphicsObject.hpp"

#include "ObjectTreeWidget.hpp"
#include "AreaTreeWidget.hpp"

#include "AreasData.hpp"
#include <QDockWidget>

class Map;
class MapEditor;
class MiniMapView;


/*!
    \class ObjectPalette
    \brief La palette contient les différents objets que l'on peut placer sur la carte
*/

class ObjectPalette : public QDockWidget {

Q_OBJECT

public:

    /*!
        \param Map La carte servira à créer la minimapview mais ne sera pas conservée
    */
    ObjectPalette(Map *map, MapEditor *parent = 0);

    /*!
        \brief Remplis la palette avec les informations du xml
        Cela vide le contenu existant avant l'appel
    */
    void fill();

    /*!
        \brief Renvoi le type de l'objet selectionné (zone ou objet)
        \throws Renvoie une exeption (PALETTE_INVALIDE_SELECTED_ITEM) si l'objet sélectionné est invalide
    */
    GraphicsObjectType selectedObjectType() const;

    /*!
        \brief Renvoi l'ID de l'objet selectionné
        \throws Renvoie une exeption (PALETTE_INVALIDE_SELECTED_ITEM) si l'objet sélectionné est invalide (n'est pas un objet)
    */
    QString selectedObjectID() const;

    /*!
        \brief Renvoi le données de la zone selectionnées
        \throws Renvoie une exeption (PALETTE_INVALIDE_SELECTED_ITEM) si l'objet sélectionné est invalide (n'est pas une zone)
    */
    AreasData selectedAreaDatas() const;


signals :

     /*!
        \brief Est émis quand l'objet selectionné change
     */
     void selectedItemChanged();


protected slots :

     /*!
        \brief Quand l'objet sélectionné a changé, ce slot va emettre le signal selectedItemChanged();
     */
     void emitSelectedItemChanged();


protected:

    QTabWidget m_tabWidget;                 //!< Le conteneur des onglets

    ObjectTreeWidget m_charactersTree;      //!< arbre des perso

    ObjectTreeWidget m_decorsTree;          //!< Arbre des décors

    AreaTreeWidget m_areaTree;              //!< Arbre des zones

    MiniMapView *m_minMapView;              //!< La mini carte
};

#endif // OBJECTPALETTE_HPP
