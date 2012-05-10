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


#ifndef LOADINGSPLASH_HPP
#define LOADINGSPLASH_HPP

#include "constantes.hpp"

/*!
    \enum LoadingSplashStep
    \brief Les différentes étapes du chargement
*/
enum LoadingSplashStep {
    MAP,        //!< Chargement de la carte
    SCRIPTING,  //!< Chargement de la zone de scripting
    INSPECTOR,  //!< Chargement de l'inspecteur
    PALETTE,    //!< Chargement de la palette
    MENUS       //!< Chargement de l'interface (menu, actions)
};

/*!
    \class LoadingSplash
    \brief Le splash de chargement au lancement de l'éditeur
*/

class LoadingSplash : public QSplashScreen {

public:

    LoadingSplash();

    /*!
        \brief Défini l'étape de chargement en cours
    */
    void setCurrentStep(LoadingSplashStep step);

};

#endif // LOADINGSPLASH_HPP