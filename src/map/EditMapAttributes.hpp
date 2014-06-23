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


#ifndef EDITMAPATTRIBUTES_HPP
#define EDITMAPATTRIBUTES_HPP

#include "constantes.hpp"
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

class Map;

/*!
    \class EditMapAttributes
    \brief Fenêtre de dialog permetant d'éditer les attributs de la carte (nom, largeur, hauteur...)
    Quand la fenêtre est validée, les modifications sont automatique propagées à la carte
*/

class EditMapAttributes : public QDialog {

Q_OBJECT

public:

    /*!
        \param map La carte sur laquelle on agit
    */
    EditMapAttributes(Map *map, QWidget *parent = 0);


protected slots :
    /*!
        \brief Validation des modifications
    */
    void commit();


protected :

    Map *m_map;                 //!< La carte concernée

    QLineEdit m_editMapName;    //!< Le widget qui permet de fixer le nom de la carte

    QSpinBox m_editWidth;       //!< Un widget qui permet de fixer la largeur de la carte

    QSpinBox m_editHeight;      //!< Un widget qui permet de fixer la hauteur de la carte

};

#endif // EDITMAPATTRIBUTES_HPP
