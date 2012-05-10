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


#include "ImageManager.hpp"

#include <QtXml>

ImageManager ImageManager::m_sharedImageManager;


ImageManager::ImageManager() {
    loadXml();
}


ImageManager::~ImageManager() {
    for(QMap<QString, ImageData *>::iterator it = m_ressources.begin(); it != m_ressources.end(); ++it) {
        if (it.value()->isLoaded) {
            delete it.value()->image;
        }
        delete it.value();
    }
}


void ImageManager::loadXml() {

    QDomDocument doc;

    QFile f(DATA_DIR + "/images.xml");
    if ( ! f.open(QIODevice::ReadOnly) ) {
        Log::e("ImageManager") << "Impossible d'ouvrir" + f.fileName()  << ":" << f.errorString();
        return;
    }
    doc.setContent(&f);
    f.close();


    QDomNode n = doc.documentElement().firstChild();

    while( ! n.isNull() ) {

        // informations générales sur l'image
        ImageData *imageData = new ImageData;
        imageData->id               = n.toElement().attribute("id");
        imageData->columnCount      = n.toElement().attribute("columnCount", "1").toInt();
        imageData->rowCount         = n.toElement().attribute("rowCount", "1").toInt();
        imageData->isLoaded         = false;
        imageData->image            = 0;
        imageData->description      = n.toElement().attribute("description");
        imageData->fps              = n.toElement().attribute("fps", "24").toInt();
        imageData->url              = n.toElement().attribute("url");
        imageData->width            = n.toElement().attribute("width", "0").toInt();
        imageData->height           = n.toElement().attribute("height", "0").toInt();
        imageData->isRectangular    = ( n.toElement().attribute("isRectangular") == "true" );
        imageData->isOneShotAnim    = ( n.toElement().attribute("isOneShotAnim") == "true" );

        // liste des points de collision
        QDomNodeList childNodes = n.childNodes();
        for (int i=0; i<childNodes.size(); ++i) {
            imageData->pointList.append(
                        QPoint(
                                childNodes.at(i).toElement().attribute("x", "0").toInt(),
                                childNodes.at(i).toElement().attribute("y", "0").toInt()
                                )
                    );
        }

        m_ressources[n.toElement().attribute("id")] = imageData;

        n = n.nextSibling();
    }

}


void ImageManager::loadImage(const QString &imageDataId) {
    ImageData *imageData = m_sharedImageManager.m_ressources[imageDataId];

    if ( ! imageData->isLoaded ) { // il faut charger l'image en mémoire
        QPixmap *pixmap = new QPixmap(QString(IMAGE_DIR) + imageData->url);
        if ( pixmap->isNull() ) {
            Log::e("ImageManager") << "Erreur chargement d'une image : " << imageData->url;
        } else {
            imageData->image = pixmap;
        }
        imageData->isLoaded = true; // on ne retentera pas de la charger quoi qu'il arrive
    }
}



ImageData* ImageManager::getImageData(const QString &imageDataId) {
    loadImage(imageDataId);
    return m_sharedImageManager.m_ressources[imageDataId];
}



QPixmap ImageManager::pixmapFromTileset(const QString &imageDataId) {

    loadImage(imageDataId);
    ImageData *imageData = ImageManager::getImageData(imageDataId);

    return imageData->image->copy(
                    0,
                    0,
                    imageData->image->width() / imageData->columnCount,
                    imageData->image->height() / imageData->rowCount
            );
}

