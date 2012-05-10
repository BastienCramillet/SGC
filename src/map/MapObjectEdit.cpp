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


#include "MapObjectEdit.hpp"

#include <limits>

#include "Map.hpp"
#include "MapObject.hpp"
#include "ObjectInspectorTable.hpp"


MapObjectEdit::MapObjectEdit(Map *map)
    : m_map(map), m_currentObject(0), m_objectInspectorTable(0)
{
    m_spinX.setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    m_spinY.setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    m_spinResistance.setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    m_spinDensity.setRange(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
    m_spinFriction.setRange(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
    m_spinRotation.setRange(-360, 360);

    QFormLayout *mainLayout = new QFormLayout();
    mainLayout->addRow("Nom pour le scripting :", &m_editScriptingName);
    mainLayout->addRow("Position x :", &m_spinX);
    mainLayout->addRow("Position y :", &m_spinY);
    mainLayout->addRow("Rotation :", &m_spinRotation);
    mainLayout->addRow("Destructible ?", &m_checkIsDestructible);
    mainLayout->addRow("Movible ?", &m_checkIsMovable);
    mainLayout->addRow("En arrière plan ?", &m_checkIsOnBackground);
    mainLayout->addRow("Résistance (ou vie) :", &m_spinResistance);
    mainLayout->addRow("Densité :", &m_spinDensity);
    mainLayout->addRow("Friction :", &m_spinFriction);

    m_groupBox = new QGroupBox("Editeur de propriétés", this);
    m_groupBox->setLayout(mainLayout);

    setCurrentObject(0);
}


void MapObjectEdit::setCurrentObject(MapObject *object) {

    disconnect(&m_spinX);
    disconnect(&m_spinY);
    disconnect(&m_spinRotation);
    disconnect(&m_editScriptingName);
    disconnect(&m_checkIsDestructible);
    disconnect(&m_checkIsMovable);
    disconnect(&m_checkIsOnBackground);
    disconnect(&m_spinResistance);
    disconnect(&m_spinDensity);
    disconnect(&m_spinFriction);

    m_currentObject = object;

    if ( ! m_currentObject ) {
        m_groupBox->setEnabled(false);
        return;
    }
    m_groupBox->setEnabled(true);

    m_editScriptingName.setText(object->scriptingName());
    m_spinX.setValue(object->x());
    m_spinY.setValue(object->y());
    m_spinRotation.setValue(object->rotation());
    m_checkIsDestructible.setChecked(object->isDestructible());
    m_checkIsMovable.setChecked(object->isMovable());
    m_checkIsOnBackground.setChecked(object->isOnBackground());
    m_spinResistance.setValue(object->resistance());
    m_spinDensity.setValue(object->density());
    m_spinFriction.setValue(object->friction());

    connect(&m_spinX, SIGNAL(valueChanged(int)), SLOT(xValueChanged()));
    connect(&m_spinY, SIGNAL(valueChanged(int)), SLOT(yValueChanged()));
    connect(&m_spinRotation, SIGNAL(valueChanged(int)), SLOT(rotationChanged()));
    connect(&m_editScriptingName, SIGNAL(textChanged(QString)), SLOT(scriptingNameChanged(QString)));
    connect(&m_checkIsDestructible, SIGNAL(toggled(bool)), SLOT(destructibilityChanged(bool)));
    connect(&m_checkIsMovable, SIGNAL(toggled(bool)), SLOT(movabilityChanged(bool)));
    connect(&m_checkIsOnBackground, SIGNAL(toggled(bool)), SLOT(onBackgroundChanged(bool)));
    connect(&m_spinResistance, SIGNAL(valueChanged(int)), SLOT(resistanceChanged(int)));
    connect(&m_spinDensity, SIGNAL(valueChanged(double)), SLOT(densityChanged(double)));
    connect(&m_spinFriction, SIGNAL(valueChanged(double)), SLOT(frictionChanged(double)));

}


void MapObjectEdit::xValueChanged() {
    m_currentObject->setX(m_spinX.value());
}


void MapObjectEdit::yValueChanged() {
    m_currentObject->setY(m_spinY.value());
}


void MapObjectEdit::rotationChanged() {
    m_spinRotation.setValue(m_spinRotation.value()%360);    // pour pas avoir de valeur trop absurdes
    m_currentObject->setRotation(m_spinRotation.value());
}


void MapObjectEdit::setObjectScriptingName(const QString &name) {
    m_currentObject->setScriptingName(name);
    m_editScriptingName.setText(name);
}


void MapObjectEdit::scriptingNameChanged(const QString &newName) {
    if ( ! m_currentObject ) {  // pas d'objet en cours d'édition
        return;
    }

    m_currentObject->setScriptingName(newName);

    // et il faut repercuter le changement sur l'arbre
    if ( m_objectInspectorTable ) {
        m_objectInspectorTable->updateObjectScriptingName(m_currentObject, newName);
    }
}


void MapObjectEdit::setObjectInspectorTable(ObjectInspectorTable *table) {
    m_objectInspectorTable = table;
}


void MapObjectEdit::destructibilityChanged(bool newDestructibility) {
    m_currentObject->setDestructible(newDestructibility);
}


void MapObjectEdit::movabilityChanged(bool newMovability) {
    m_currentObject->setMovable(newMovability);
}


void MapObjectEdit::onBackgroundChanged(bool newOnBackground) {
    m_currentObject->setOnBackground(newOnBackground);
}


void MapObjectEdit::resistanceChanged(int newResistance) {
    m_currentObject->setResistance(newResistance);
}


void MapObjectEdit::densityChanged(double newDensity) {
    m_currentObject->setDensity(static_cast<float>(newDensity));
}


void MapObjectEdit::frictionChanged(double newFriction) {
    m_currentObject->setFriction(static_cast<float>(newFriction));
}
