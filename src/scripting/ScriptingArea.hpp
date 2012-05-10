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


#ifndef SCRIPTINGAREA_HPP
#define SCRIPTINGAREA_HPP

#include "constantes.hpp"

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qsciapis.h>


class MapEditor;

/*!
    \class ScriptingArea
    \brief Zone texte qui sert à entrer le script associé à la carte en cours d'édition
*/

class ScriptingArea : public QDockWidget {

public:

    ScriptingArea(MapEditor *parent = 0);

    /*!
        \brief Sauvegarde le script dans le fichier dont le nom et le nom seulement et donné en paramètre (juste le nom du fichier, pas le chemin !)
    */
    void save(const QString &name) const;

    /*!
        \brief Charger le contenu du fichier dont le nom (pas le chemin !) est donné en paramètre
    */
    void load(const QString &name);

    /*!
        \brief Effacer le script et remet les informations de base (date, auteur)
    */
    void clear();


protected :

     QsciScintilla m_textZone;              //!< Zone de texte

};

#endif // SCRIPTINGAREA_HPP
