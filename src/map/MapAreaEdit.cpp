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


#include "MapAreaEdit.hpp"

#include <limits>

#include "Map.hpp"
#include "MapArea.hpp"
#include "ObjectInspectorTable.hpp"


MapAreaEdit::MapAreaEdit(Map *map) : m_map(map) {

    m_spinX.setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    m_spinY.setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    m_spinHeight.setRange(20, std::numeric_limits<int>::max());
    m_spinWidth.setRange(20, std::numeric_limits<int>::max());

    QFormLayout *mainLayout = new QFormLayout();
    mainLayout->addRow("Nom pour le scripting :", &m_editScriptingName);
    mainLayout->addRow("Position x :", &m_spinX);
    mainLayout->addRow("Position y :", &m_spinY);
    mainLayout->addRow("Largeur :", &m_spinWidth);
    mainLayout->addRow("Hauteur :", &m_spinHeight);

    m_groupBox = new QGroupBox("Editeur de propriétés", this);
    m_groupBox->setLayout(mainLayout);

    setCurrentArea(0);
}


void MapAreaEdit::setCurrentArea(MapArea *area) {

    disconnect(&m_spinX);
    disconnect(&m_spinY);
    disconnect(&m_editScriptingName);
    disconnect(&m_spinHeight);
    disconnect(&m_spinWidth);

    m_currentArea = area;

    if ( ! m_currentArea ) {
        m_groupBox->setEnabled(false);
        return;
    }
    m_groupBox->setEnabled(true);

    m_editScriptingName.setText(area->scriptingName());
    m_spinX.setValue(area->x());
    m_spinY.setValue(area->y());
    m_spinWidth.setValue(area->pixmap().width());
    m_spinHeight.setValue(area->pixmap().height());

    connect(&m_spinX, SIGNAL(valueChanged(int)), SLOT(xValueChanged()));
    connect(&m_spinY, SIGNAL(valueChanged(int)), SLOT(yValueChanged()));
    connect(&m_spinHeight, SIGNAL(valueChanged(int)), SLOT(heightChanged()));
    connect(&m_spinWidth, SIGNAL(valueChanged(int)), SLOT(widthChanged()));
    connect(&m_editScriptingName, SIGNAL(textChanged(QString)), SLOT(scriptingNameChanged(QString)));
}


void MapAreaEdit::setObjectInspectorTable(ObjectInspectorTable *table) {
    m_objectInspectorTable = table;
}


void MapAreaEdit::xValueChanged() {
    m_currentArea->setX(m_spinX.value());
}


void MapAreaEdit::yValueChanged() {
    m_currentArea->setY(m_spinY.value());
}


void MapAreaEdit::setObjectScriptingName(const QString &name) {
    m_currentArea->setScriptingName(name);
    m_editScriptingName.setText(name);
}


void MapAreaEdit::scriptingNameChanged(const QString &newName) {
    if ( ! m_currentArea ) {  // pas d'objet en cours d'édition
        return;
    }

    m_currentArea->setScriptingName(newName);

    // et il faut repercuter le changement sur l'arbre
    if ( m_objectInspectorTable ) {
        m_objectInspectorTable->updateObjectScriptingName(m_currentArea, newName);
    }
}


void MapAreaEdit::widthChanged() {
    m_currentArea->resize(QSize(m_spinWidth.value(), m_currentArea->pixmap().height()));
}

void MapAreaEdit::heightChanged() {
    m_currentArea->resize(QSize(m_currentArea->pixmap().width(), m_spinHeight.value()));
}

