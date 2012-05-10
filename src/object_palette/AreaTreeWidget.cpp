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


#include "AreaTreeWidget.hpp"

#include "AreaTreeWidgetItem.hpp"

#include "Settings.hpp"

AreaTreeWidget::AreaTreeWidget() {
    header()->hide();
    setIconSize(QSize(40, 40));
    setAnimated(true);
}


void AreaTreeWidget::mousePressEvent(QMouseEvent *event) {

    QTreeWidget::mousePressEvent(event);

    QTreeWidgetItem *item = currentItem();
    if ( ! item || item->type() == -1) {    // pas d'objet ou un menu sélectionné
        return;
    }

    AreaTreeWidgetItem *current_item = static_cast<AreaTreeWidgetItem *>(item);

    AreasData data = current_item->getAreasData();

    Log::i("AreaTreeWidget") << "drag started :" << data.name;

    // voir http://doc.trolltech.com/4.4/draganddrop-draggableicons-dragwidget-cpp.html
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << data;

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("data/x-area", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    QPixmap pixDrag(data.size); // taille par défaut des zones
    pixDrag.fill(data.color);
    drag->setPixmap(pixDrag);
    drag->exec();

}
