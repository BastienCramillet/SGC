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


#ifndef AREATREEWIDGET_HPP
#define AREATREEWIDGET_HPP

#include "constantes.hpp"

#include <QTreeWidget>

/*!
    \class AreaTreeWidget
    \brief Un arbre qui va contenir des objets plaçable sur la carte
*/

class AreaTreeWidget : public QTreeWidget {

public:

    AreaTreeWidget();

protected :

    /*!
        \brief En cas de clique on ajoute une gestion du drag.
    */
    void mousePressEvent(QMouseEvent *event);

};

#endif // AREATREEWIDGET_HPP
