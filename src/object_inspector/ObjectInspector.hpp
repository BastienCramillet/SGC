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


#ifndef EDITOBJECTDOCK_HPP
#define EDITOBJECTDOCK_HPP

#include "constantes.hpp"

#include "GraphicsObject.hpp"

#include "MapObjectEdit.hpp"
#include "MapAreaEdit.hpp"

#include <QStackedWidget>
#include <QDockWidget>

class MapEditor;
class GraphicsObject;
class Map;
class ObjectInspectorTable;


/*!
    \class ObjectInspector
    \brief Le dock dans lequel on va trouver la liste des objets qui sont sur la carte avec la possibilité de les éditer
*/

class ObjectInspector : public QDockWidget {

Q_OBJECT

public:

    ObjectInspector(Map *map, MapEditor *parent = 0);


    /*!
        \brief Obtient un pointeur sur la table créée
    */
    ObjectInspectorTable* getObjectInspectorTable();


protected slots :

    /*!
        \brief Demande d'afficher le bon objet sur la pile
    */
    void setCurrentEdit(GraphicsObjectType type);


protected :

    ObjectInspectorTable *m_tableObjectList;    //!< Liste de tous les objets présents sur la carte

    Map *m_map;                                 //!< La carte attachée à cet inspecteur

    MapObjectEdit m_graphicsObjectEditor;       //!< Le formulaire d'édition d'objets

    MapAreaEdit m_mapAreaEditor;                //!< Le formulaire d'édition de zones

    QStackedWidget *m_stack;                    //!< La pile des formulaires d'édition

};

#endif // EDITOBJECTDOCK_HPP
