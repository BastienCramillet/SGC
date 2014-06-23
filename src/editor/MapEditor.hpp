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


#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "constantes.hpp"
#include <QMainWindow>

class Map;
class ScriptingArea;
class ObjectPalette;
class ObjectInspector;

/*!
    \class MapEditor
    \brief La fenêtre principale de l'éditeur
*/

class MapEditor : public QMainWindow {

Q_OBJECT

public:

    MapEditor();

    /*!
        \brief Renvoi la palette utilisée par l'éditeur
    */
    const ObjectPalette* getObjectPalette() const;


protected slots :

    /*!
        \brief Appuis sur le bouton nouveau
        Va vider la zone de scripting et la carte
    */
    void clear();

    /*!
        \brief Sauvegarde de la carte
    */
    void save();

    /*!
        \brief Chargement d'une carte
        Ouvre une boite de dialogue qui va servir à selectionner la carte à charger
    */
    void load();

    /*!
        \brief Sauvegarder sous
        Ouvre une boite de dialogue qui va demander sous quel nom il faut enregistrer la carte
    */
    void saveAs();

    /*!
        \brief Lance le jeu avec en argv[1] le fichier xml contenant la carte en cours
        Effectue automatiquement une sauvegarde de la carte avant de lancer le jeu
    */
    void run();


protected :

    /*!
        \brief Création du menu de la fenêtre et de la barre d'outils
        Ces créations sont groupés car elles concernent les mêmes actions donc sont intimement liées
    */
    void createMenuAndToolBar();

    /*!
        \brief Détection des touches de clavier appuyés
    */
    void keyPressEvent(QKeyEvent *);

    /*!
        \brief Détection des touches de clavier relachées
    */
    void keyReleaseEvent(QKeyEvent *);



    Map *m_map;                         //!< Carte, zone centrale de l'éditeur

    ScriptingArea *m_scriptingArea;     //!< Zone de scripting

    ObjectPalette *m_objectPalette;     //!< La palette des objets

    ObjectInspector *m_objectInspector; //!< Le dock qui permet d'espionner et de modifier les objets, la vision global de ce qui est sur la carte

    QString m_saveName;                 //!< Le nom sous lequel la carte sera sauvegardée. Par exemple plop.xml. Le script remplacera xml par lua

};

#endif // EDITOR_HPP
