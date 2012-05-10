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


#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "constantes.hpp"

#include "AreasData.hpp"

/*!
    \class Settings
    \brief Les parametres de l'éditeur, chargés depuis un xml.
    On utilise ici un pattern singleton car on ne charge ces paramètres qu'une seule fois en mémoire.
*/

class Settings {

public:

    /*!
        \brief Renvoie la liste des catégories de décors de l'éditeur (il s'agit seulement d'un système de rangement)
        Par exemple, on peut avoir :
            - des éléments décoratifs
            - des éléments interactis
    */
    static QStringList getDecorsEditorsCategories();

    /*!
        \brief Renvoie la liste des ccatégories qui gèrent les décors dans le jeu (il faut ensuite les gérer au chargement du jeu, dans une fabrique par exemple)
        Par exemple, on peut avoir ;
            - Une classe décor très générale, juste un quelque chose
            - Une classe qui va hériter de décor et qui ajoute un petit plus. Par exemple un décor mobile (un assenceur).
    */
    static QStringList getDecorsGamesCategories();

    /*!
        \brief Renvoie la liste des catégories de personnages pour l'éditeur (il s'agit seulement d'un système de rangement)
        Par exemple, on peut avoir :
            - des montres de base
            - des boss
    */
    static QStringList getCharactersEditorsCategories();

    /*!
        \brief Renvoie la liste des catégories de personnage pour le jeu (il faut bien sûr les gérer dans le jeu lui-même (dans une fabrique par exemple))
        Par exemple on peut avoir :
            - des monstres
            - des alliés
    */
    static QStringList getCharactersGamesCategories();

    /*!
        \brief Renvoie la liste des déclencheurs possibles pour les associer à un objet/personnage
        Par exemple on peut avoir :
            - onCreate
            - onDestroy
            - onJump
            - ...
    */
    static QStringList getTriggerList();

    /*!
        \brief Renvoi le chemin vers l'executable windows du jeu
    */
    static QString getWindowsExecutable();

    /*!
        \brief Renvoi le chemin vers l'executable linux du jeu
    */
    static QString getLinuxExecutable();

    /*!
        \brief Renvoi la liste des zones à mettre dans l'éditeur par défaut
    */
    static QList<AreasData> getAreasList();

    /*!
        \brief Obtient les données de la zone par défaut
    */
    static AreasData getAreasDefaultDatas();


private:

    Settings();

    /*!
        \brief Charger le xml contenant les infos
    */
    static void loadXML();


    static Settings m_sharedSettings;           //!< L'instance de paramètres

    QStringList m_decorsEditorsCategories;      //!< Liste des catégories de décors pour l'éditeur

    QStringList m_decorsGamesCategories;        //!< Liste des catégories de décors pour le jeu

    QStringList m_charactersEditorsCategories;  //!< Liste des catégories de personnage pour l'éditeur

    QStringList m_charactersGamesCategories;    //!< Liste des catégories de personnage pour le jeu

    QStringList m_triggerList;                  //!< Liste des déclencheur possibles

    QString m_windowsExecutable;                //!< Chemin vers l'executable windows du jeu (pour tester la carte)

    QString m_linuxExecutable;                  //!< Chemin vers l'executable linux du jeu (pour tester la carte)

    QList<AreasData> m_areasList;               //!< Liste des zones

    AreasData m_areasDefaultDatas;              //!< Données de LA zone par défaut
};


#endif // PARAMETERS_HPP
