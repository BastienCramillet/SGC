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


#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP


#include <QtGui>
#include <QtCore>
#include <QWidget>


#include "Log.hpp"


const QString editorName = "SGC - Simple Game Creator";  //!< Le nom de l'éditeur, qui va apparaître dans la barre de menu entre autres


/* Les constantes ci-dessous seront à passer dans un fichier de conf */

const QString DATA_DIR = "data/";                           //!< Le dossier qui contient les données
const QString EDITOR_DATA_DIR = "editor_data/";             //!< Le dossier qui contient les données de l'éditeur

const QString IMAGE_DIR = DATA_DIR + "/images/";            //!< Le dossier qui contient les images
const QString MAPS_DIR = DATA_DIR + "/maps/";               //!< Le dossier dans lequel les cartes sont sauvegardés
const QString SCRIPTS_DIR = DATA_DIR + "/scripts/";         //!< Le dossier dans lequel les scripts sont sauvegardés

/* ---------------------------------------------------------------- */



/*!
    \enum EditorException
    \brief Les exceptions qui peuvent être envoyé par les différents élément de l'éditeur
*/
enum EditorException {
    PALETTE_INVALID_SELECTED_ITEM                   //!< Générée si l'objet sélectionné dans la palette est invalide alors qu'on cherche à l'obtenir
};

#endif // CONSTANTES_HPP
