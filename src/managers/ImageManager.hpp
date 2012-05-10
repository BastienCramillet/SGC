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


#ifndef IMAGEMANAGER_HPP
#define IMAGEMANAGER_HPP

#include "constantes.hpp"

/*!
    \struct ImageData
    \brief Les données qui sont attachées à une image
*/
struct ImageData {

    QString id;                 //!< L'ID de l'objet
    bool isLoaded;              //!< La ressource est-elle chargée ?
    QString description;        //!< Une courte description de l'image
    QString url;                //!< URL de la ressource
    QPixmap *image;             //!< L'image en elle-même
    int rowCount;               //!< Nombre de lignes de l'image
    int columnCount;            //!< Nombre de la colonne de l'image
    int fps;                    //!< Taux de raffraichissement de l'image
    int width;                  //!< Largeur de l'image
    int height;                 //!< Hauteur de l'image
    bool isOneShotAnim;         //!< Toutes les lignes et colonnes de l'image ne représente en fait qu'une seule animation
    bool isRectangular;         //!< Si c'est vrai, alors le jeu ne prendra pas les points de collision donnés mais les bords de l'image
    QList<QPoint> pointList;    //!< Liste des points de collision de l'objet
};

/*!
    \class ImageManager
    \brief Cette classe gère le chargement des images.
    Elle charge les données concernant les images à la création du manager (qui est un singleton). Ensuite elle charge une image seulement si celle-ci
    est demandée par une classe externe auquel cas elle renvoi un pointeur sur toutes les données de l'image.
*/

class ImageManager {


public :

    /*!
        \brief Renvoi un pointeur sur les données demandées
        Si l'image n'était pas encore chargée en mémoire, elle est chargé à ce moment là.
    */
    static ImageData* getImageData(const QString &imageDataId);

    /*!
        \brief Découpage de l'image pour n'en garder qu'une sur un tileset
        Si l'image n'était pas encore chargée en mémoire, elle est chargé à ce moment là.
    */
    static QPixmap pixmapFromTileset(const QString &imageDataId);


protected :

    ImageManager();

    ~ImageManager();

    /*!
        \brief Chargement du xml qui contient les données
    */
    void loadXml();

    /*!
        \brief Chargement d'une image si celle-ci n'est pas encore chargée
    */
    static void loadImage(const QString &imageDataId);



    static ImageManager m_sharedImageManager;   //!< ImageManager partagé par toutes les instances

    QMap<QString, ImageData *> m_ressources;    //!< Les ressources <id, données>

};

#endif // IMAGEMANAGER_HPP
