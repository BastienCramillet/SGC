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


#include "LoadingSplash.hpp"

LoadingSplash::LoadingSplash() {
}


void LoadingSplash::setCurrentStep(LoadingSplashStep step) {

    Log::v("Loading splash") << "current step changed : " << step;

    QPixmap currentPixmap;

    switch(step) {

    case MAP :
        currentPixmap.load(":/ressources/splashs/step-1.png");
        showMessage("Création de la carte ...", Qt::AlignBottom);
        break;

    case SCRIPTING :
        currentPixmap.load(":/ressources/splashs/step-2.png");
        showMessage("Création de la zone de scripting ...", Qt::AlignBottom);
        break;

    case INSPECTOR :
        currentPixmap.load(":/ressources/splashs/step-3.png");
        showMessage("Création de l'inspecteur d'objets ...", Qt::AlignBottom);
        break;

    case PALETTE :
        currentPixmap.load(":/ressources/splashs/step-4.png");
        showMessage("Création de la palette ...", Qt::AlignBottom);
        break;

    case MENUS :
        currentPixmap.load(":/ressources/splashs/step-5.png");
        showMessage("Création des menus ...", Qt::AlignBottom);
        break;
    }

    setPixmap(currentPixmap);
    setMask(currentPixmap.mask());

    // raffraichissement de l'affichage
    show();

    //sleep(2);
}
