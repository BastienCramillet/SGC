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

/*!

    \mainpage st'editor
    \author Xavier MICHEL
    \version 0.1
    \date 03/03/2011

    Qt version requise >= 4.6

\section sec_class Classes

    \image html doc/classes.png


\section sec_tec Notes techniques

    \subsection zIndex

        Le zIndex sert à placer des objets au dessus d'autres. Pour l'éditeur, les objets au premier plan ont un zIndex de -1, puis les objets
        d'un plan N ont un zIndex de -(N+1).

        Cela permet de concerver toutes les valeurs positives pour le moteur du jeu. Notons cependant que cette limite à ses limites, si on veut
        placer un objet en arrière plan dans le jeu il faudra choisir un zIndex négatif assez grand (plus grand que le nombre de calques). On pourrait
        aussi se baser sur le nombre de calques déclarés dans le fichier de chargement.


*/
