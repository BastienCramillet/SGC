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


#ifndef MAPAREAEDIT_HPP
#define MAPAREAEDIT_HPP

#include "constantes.hpp"

#include <QLineEdit>
#include <QSpinBox>
#include <QGroupBox>

class Map;
class MapArea;
class ObjectInspectorTable;

/*!
    \class MapAreaEdit
    \brief Un widget qui permet de modifier les attributs d'une zone (nom, x, largeur, ...)
*/

class MapAreaEdit : public QWidget {

Q_OBJECT

public:

    /*!
        \param map La carte qui contient les objets à éditer
    */
    MapAreaEdit(Map *map);

    /*!
        \brief Défini l'objet en cours d'édition
    */
    void setCurrentArea(MapArea *object);

    /*!
        \brief Défini la table associé à cet éditeur d'objet
    */
    void setObjectInspectorTable(ObjectInspectorTable *table);

    /*!
        \brief Défini le nom de la zone en cours au niveau scripting
        Il est enregitrer immédiatement dans l'objet concerné
    */
    void setObjectScriptingName(const QString &name);


protected slots :

    /*!
        \brief Nouvelle valeur pour la position sur x de l'objet
    */
    void xValueChanged();

    /*!
        \brief Nouvelle valeur pour la position sur y de l'objet
    */
    void yValueChanged();

    /*!
        \brief Le nom de l'objet pour le scripting à changé
    */
    void scriptingNameChanged(const QString &newName);

    /*!
        \brief La largeur de la zone à changée
    */
    void widthChanged();

    /*!
        \brief La hauteur de la zone à changée
    */
    void heightChanged();


protected :

    Map *m_map;                                     //!< La carte qui contient les zones à éditer

    QLineEdit m_editScriptingName;                  //!< Le nom de l'objet pour le scripting

    QSpinBox m_spinX;                               //!< Le widget qui sert à saisir la position sur x de l'objet courant

    QSpinBox m_spinY;                               //!< Le widget qui sert à saisir la position sur y de l'objet courant

    QSpinBox m_spinWidth;                           //!< Le widget qui sert à saisir la largeur de la zone

    QSpinBox m_spinHeight;                          //!< Le widget qui sert à saisir la hauteur de la zone

    MapArea *m_currentArea;                         //!< La zone en cours d'édition

    ObjectInspectorTable *m_objectInspectorTable;   //!< La table qui m'est associée

    QGroupBox *m_groupBox;                          //!< Le regroupement de toutes les propriétés de l'objet

};

#endif // MAPAREAEDIT_HPP
