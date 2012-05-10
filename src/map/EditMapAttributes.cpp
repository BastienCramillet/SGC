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

#include "EditMapAttributes.hpp"

#include <limits>

#include "Map.hpp"

EditMapAttributes::EditMapAttributes(Map *map, QWidget *parent)
    : QDialog(parent), m_map(map) {

    setWindowTitle(editorName);

    QFormLayout *mainLayout = new QFormLayout();

    // le nom
    m_editMapName.setText(m_map->mapName());
    mainLayout->addRow("Nom de la carte :", &m_editMapName);

    // largeur
    m_editWidth.setMaximum(std::numeric_limits<int>::max());
    m_editWidth.setValue(m_map->width());
    mainLayout->addRow("Largeur de la carte :", &m_editWidth);

    // hauteur
    m_editHeight.setMaximum(std::numeric_limits<int>::max());
    m_editHeight.setValue(m_map->height());
    mainLayout->addRow("Hauteur de la carte :", &m_editHeight);

    // boutons annuler et valider
    QPushButton *cancelButton   = new QPushButton("Annuler");
    QPushButton *validateButton = new QPushButton("Valider");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(validateButton);

    mainLayout->addRow(buttonLayout);

    connect(cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(validateButton, SIGNAL(clicked()), SLOT(commit()));

    // affichage
    setLayout(mainLayout);
    setModal(true);
    show();
}


void EditMapAttributes::commit() {
    m_map->setMapName(m_editMapName.text());
    m_map->resize(m_editWidth.value(), m_editHeight.value());
    close();
}

