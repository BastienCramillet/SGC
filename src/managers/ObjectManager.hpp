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


#ifndef OBJECTMANAGER_HPP
#define OBJECTMANAGER_HPP

#include "constantes.hpp"

#include "ImageManager.hpp"

#include "toolBox.hpp"


/*!
    \struct ObjectData
    \brief Les données qui sont attachées à un objet
*/
struct ObjectData {

    QString id;                             //!< L'ID de cet objet
    QString description;                    //!< Une courte description de l'objet
    int width;                              //!< La largeur de l'objet
    int height;                             //!< La hauteur de l'objet
    QString gameClass;                      //!< Le nom de la super classe de l'objet dans le jeu
    QString gameFactoryInfo;                //!< Un info supplémentaire pour le jeu, dans le cas de création de sous-objets dans une factory par exemple
    QString editorCategory;                 //!< Le libellé de la catégorie de l'objet dans l'éditeur

    bool isDestructible;                    //!< Cet objet est-il destructible par défaut ?
    bool isOnBackground;                    //!< Cet objet est-il en arrière plan par défaut ?
    bool isMovable;                         //!< Cet objet est-il movible par défaut ?
    int resistance;                         //!< Résistance (ou vie) de l'objet par défaut
    float density;                          //!< Densité de l'objet par défaut
    float friction;                         //!< Friction de l'objet par défaut

    QMultiMap<QString, QString> images;     //!< Liste des images associées à l'objet <trigger, id>
    QMultiMap<QString, QString> sounds;     //!< Liste des sons associés à l'objet <trigger, id>
    QMultiMap<QString, QString> ephemerals; //!< Liste des éphérmères associés à l'objet <trigger, id>

    /*!
        \brief Renvoi l'image "normale" de l'objet (à sa création), avec la bonne taille
        \param imageID, si le paramêtre est renseigné on utilise l'identifiant donné, sinon on en tire un aléatoirement parmi les images "normales" de l'objet
    */
    QPixmap getImage(const QString &imageID = "") const {
        if ( ! imageID.isEmpty() ) {
            return ImageManager::pixmapFromTileset(imageID).scaled(width, height);
        }
        return ImageManager::pixmapFromTileset(getImageDataID()).scaled(width, height);
    }

    /*!
        \brief Renvoie l'ID d'une image dite "normale" de l'objet, son image à la création
        \warning Ne pas utiliser cette méthode pour récupérer l'image même via l'imageManager, ou alors prenez en compte la taille de l'image (scalled) !
    */
    QString getImageDataID() const {
        QMultiMap<QString, QString>::const_iterator it = images.find("onCreate");
        QVector<QString> possibleImages;
        while (it != images.end() && it.key() == "onCreate") {
            possibleImages.push_back(it.value());
            ++it;
        }
        return possibleImages[random(0, possibleImages.size()-1)];
    }
};


/*!
    \class ObjectManager
    \brief Cette classe va charger les données concernant chaque objet depuis un xml puis les conserver.
    On pourra alors simplement lui demandé quelles sont les informations sur une ressource donnée depuis une classe externe.
*/
class ObjectManager {

public :

    /*!
        \brief Retourne les informations pour un objet donné
    */
    static ObjectData* getObjectData(const QString &id);

    /*!
        \brief Retourne la liste des objets
    */
    static QMap<QString, ObjectData *> getObjectMap();


protected :

    ObjectManager();

    ~ObjectManager();

    /*!
        \brief Chargement du xml qui contient les données
    */
    void loadXml();



    static ObjectManager m_sharedObjectManager;  //!< ObjectManager partagé par toutes les instances

    QMap<QString, ObjectData *> m_ressources;        //!< Les ressources <id, données>
};

#endif // OBJECTMANAGER_HPP
