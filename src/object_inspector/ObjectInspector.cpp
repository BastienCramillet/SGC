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


#include "ObjectInspector.hpp"

#include "MapEditor.hpp"
#include "ObjectInspectorTable.hpp"

ObjectInspector::ObjectInspector(Map *map, MapEditor *parent)
    : QDockWidget(parent), m_map(map), m_graphicsObjectEditor(map), m_mapAreaEditor(map)
{
    setWindowTitle("Inspecteur d'objet");

    m_tableObjectList = new ObjectInspectorTable(map);
    m_tableObjectList->setAssociatedGraphicsObjectEditor(&m_graphicsObjectEditor);
    m_tableObjectList->setAssociatedGraphicsAreaEditor(&m_mapAreaEditor);

    // boutons de gestion de l'arbre
    QPushButton *btnNewLayout = new QPushButton("+");
    connect(btnNewLayout, SIGNAL(clicked()), m_tableObjectList, SLOT(addLayout()));

    QPushButton *btnRemoveLayout = new QPushButton("-");
    btnRemoveLayout->setEnabled(false);
    connect(m_tableObjectList, SIGNAL(currentLayoutIsDeletable(bool)), btnRemoveLayout, SLOT(setEnabled(bool)));
    connect(btnRemoveLayout, SIGNAL(clicked()), m_tableObjectList, SLOT(removeCurrentLayout()));

    QPushButton *btnUpLayout = new QPushButton("^");
    btnUpLayout->setEnabled(false);
    connect(m_tableObjectList, SIGNAL(currentLayoutIsUppable(bool)), btnUpLayout, SLOT(setEnabled(bool)));
    connect(btnUpLayout, SIGNAL(clicked()), m_tableObjectList, SLOT(upCurrentLayout()));

    QPushButton *btnDownLayout = new QPushButton("v");
    btnDownLayout->setEnabled(false);
    connect(m_tableObjectList, SIGNAL(currentLayoutIsDownable(bool)), btnDownLayout, SLOT(setEnabled(bool)));
    connect(btnDownLayout, SIGNAL(clicked()), m_tableObjectList, SLOT(downCurrentLayout()));

    QHBoxLayout *layoutBtn = new QHBoxLayout();
    layoutBtn->addWidget(btnNewLayout);
    layoutBtn->addWidget(btnRemoveLayout);
    layoutBtn->addWidget(btnUpLayout);
    layoutBtn->addWidget(btnDownLayout);


    // un widget qui contient l'arbre et ses boutons
    QVBoxLayout *treeLayout = new QVBoxLayout();
    treeLayout->addWidget(m_tableObjectList);
    treeLayout->addLayout(layoutBtn);

    QWidget *treeContainer = new QWidget();
    treeContainer->setLayout(treeLayout);

    // pile des formulaires
    m_stack = new QStackedWidget();
    m_stack->addWidget(&m_graphicsObjectEditor);
    m_stack->addWidget(&m_mapAreaEditor);

    // remplissage du conteneur principal
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(treeContainer);
    splitter->addWidget(m_stack);

    m_stack->setMinimumHeight(m_graphicsObjectEditor.height()); // sinon il se cache...

    setCurrentEdit(MAP_OBJECT);

    setWidget(splitter);

    connect(m_tableObjectList, SIGNAL(currentSelectedItemIs(GraphicsObjectType)), SLOT(setCurrentEdit(GraphicsObjectType)));
}


void ObjectInspector::setCurrentEdit(GraphicsObjectType type) {
    m_stack->setCurrentIndex(static_cast<int>(type));
}


ObjectInspectorTable* ObjectInspector::getObjectInspectorTable() {
    return m_tableObjectList;
}
