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


#ifndef MAPOBJECTEDIT_HPP
#define MAPOBJECTEDIT_HPP

#include "constantes.hpp"
#include <QLineEdit>
#include <QSpinBox>
#include <QGroupBox>
#include <QCheckBox>

class Map;
class MapObject;
class ObjectInspectorTable;

/*!
    \class MapObjectEdit
    \brief Un widget qui permet de modifier les attributs d'un objet (nom, x, densité, ...)
*/

class MapObjectEdit : public QWidget {

Q_OBJECT

public:

    /*!
        \param map La carte qui contient les objets à éditer
    */
    MapObjectEdit(Map *map);

    /*!
        \brief Défini l'objet en cours d'édition
    */
    void setCurrentObject(MapObject *object);

    /*!
        \brief Défini le nom de l'objet en cours au niveau scripting
        Il est enregitrer immédiatement dans l'objet concerné
    */
    void setObjectScriptingName(const QString &name);

    /*!
        \brief Défini la table associé à cet éditeur d'objet
    */
    void setObjectInspectorTable(ObjectInspectorTable *table);


private slots :

    /*!
        \brief Nouvelle valeur pour la position sur x de l'objet
    */
    void xValueChanged();

    /*!
        \brief Nouvelle valeur pour la position sur y de l'objet
    */
    void yValueChanged();

    /*!
        \brief Nouvelle valeur pour la rotation
    */
    void rotationChanged();

    /*!
        \brief Le nom de l'objet pour le scripting à changé
    */
    void scriptingNameChanged(const QString &newName);

    /*!
        \brief La qualité destructible de l'objet a changée
    */
    void destructibilityChanged(bool newDestructibility);

    /*!
        \brief La qualité movible de l'objet a changée
    */
    void movabilityChanged(bool newMovability);

    /*!
        \brief La qualité arrière plan de l'objet a changée
    */
    void onBackgroundChanged(bool newOnBackground);

    /*!
        \brief La résistance (ou vie) de l'objet a changée
    */
    void resistanceChanged(int newResistance);

    /*!
        \brief La densité de l'objet a changée
    */
    void densityChanged(double newDensity);

    /*!
        \brief La friction de l'objet a changée
    */
    void frictionChanged(double newFriction);


private :

    Map *m_map;                                     //!< La carte qui contient les objets à éditer

    MapObject *m_currentObject;                     //!< L'objet en cours d'édition

    QLineEdit m_editScriptingName;                  //!< Le nom de l'objet pour le scripting

    QSpinBox m_spinX;                               //!< Le widget qui sert à saisir la position sur x de l'objet courant

    QSpinBox m_spinY;                               //!< Le widget qui sert à saisir la position sur y de l'objet courant

    QSpinBox m_spinRotation;                        //!< Le widget qui sert à saisir la rotation de l'objet courant

    ObjectInspectorTable *m_objectInspectorTable;   //!< La table qui m'est associée

    QGroupBox *m_groupBox;                          //!< Le regroupement de toutes les propriétés de l'objet

    QCheckBox m_checkIsDestructible;                //!< L'object est destructible dans le jeu ?

    QCheckBox m_checkIsMovable;                     //!< L'object est movible dans le jeu ?

    QCheckBox m_checkIsOnBackground;                //!< L'objet est en arrière plan dans le jeu ?

    QSpinBox m_spinResistance;                      //!< La résistance de l'objet dans le jeu

    QDoubleSpinBox m_spinDensity;                   //!< La densité de l'objet dans le jeu

    QDoubleSpinBox m_spinFriction;                  //!< La friction de l'objet dans le jeu

};

#endif // MAPOBJECTEDIT_HPP
