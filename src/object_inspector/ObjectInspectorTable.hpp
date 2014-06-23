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


#ifndef OBJECTINSPECTORTABLE_HPP
#define OBJECTINSPECTORTABLE_HPP

#include "constantes.hpp"

#include "GraphicsObject.hpp"
#include <QTreeWidget>

class ObjectInspector;
class Map;
class MapObjectEdit;
class MapAreaEdit;

/*!
    \class ObjectInspectorTable
    \brief La table avec la liste des objets sur la carte
*/

class ObjectInspectorTable : public QTreeWidget {

Q_OBJECT

public:

    /*!
        \param map La carte associée à cet inspecteur
    */
    ObjectInspectorTable(Map *map);

    /*!
        \brief Ajoute une ligne dans la table qui correspondra à l'objet donné
        \param object L'objet auquel la ligne est attachée
        \param layoutNumber Si ce paramètre est renseigné on place l'objet dans le layout situé à l'ID donné. Sinon on place l'objet dans la layout courant.
    */
    void addRow(GraphicsObject *object, int layoutNumber = 0);

    /*!
        \brief Supprime la ligne associé à l'objet de l'arbre
    */
    void removeRow(GraphicsObject *object);

    /*!
        \brief Selectionne l'objet donné
        \warning Cela ne déselectionne pas les autres objets !
    */
    void select(GraphicsObject *object);

    /*!
        \brief Supprime toutes les données stockés et vide la table
    */
    void clear();

    /*!
        \brief Défini quel éditeur d'objet est associé à cet arbre
        Cet objet se signal à l'éditeur d'objet comme son associé, prêt à faire des échanges.
    */
    void setAssociatedGraphicsObjectEditor(MapObjectEdit *objectEditor);

    /*!
        \brief Défini quel éditeur de zones est associé à cet arbre
        Cet objet se signal à l'éditeur d'objet comme son associé, prêt à faire des échanges.
    */
    void setAssociatedGraphicsAreaEditor(MapAreaEdit *areaEditor);

    /*!
        \brief Force l'éditeur d'objet graphique à recharger ses données
    */
    void forceGraphicsObjectEditorUpdate();

    /*!
        \brief Met à jour le nom pour l'objet donné dans l'arbre
    */
    void updateObjectScriptingName(GraphicsObject *object, const QString &newName);

    /*!
        \brief Renvoi la liste des noms de calques sous la forme <idCalque, nom>
    */
    QMap<int, QString> getLayouts() const;

    /*!
        \brief Ajoute un calque par défaut
    */
    void addDefaultLayout();


public slots :

    /*!
        \brief Ajoute un calque dans la table
        \param name Le nom du calque à ajouter
    */
    void addLayout(const QString &name = "");

    /*!
        \brief Supprime le layout courant
    */
    void removeCurrentLayout();

    /*!
        \brief Fait monter le layout courant
    */
    void upCurrentLayout();

    /*!
        \brief Fait descendre le layout courant
    */
    void downCurrentLayout();


signals :

    /*!
        \brief Signal qui dit si le layout courant est supprimable
    */
    void currentLayoutIsDeletable(bool);

    /*!
        \brief Signal que l'on peut monter le layout courant
    */
    void currentLayoutIsUppable(bool);

    /*!
        \brief Signal que l'on peut descendre le layout courant
    */
    void currentLayoutIsDownable(bool);

    /*!
        \brief Signal quel type d'objet est selectionné, donc quel formulaire d'édition il faut afficher
    */
    void currentSelectedItemIs(GraphicsObjectType);


protected slots :

    /*!
        \brief Actions à effectuer quand la selection à changé
        Ici on va mettre à jour l'éditeur d'objet associé à la table s'il existe
    */
    void selectionUpdated();

    /*!
        \brief Un item a été double cliqué
        On centre la vue sur cet objet sur la carte et on demande d'éditer le nom de l'objet pour le scripting
    */
    void itemWasDoubleClicked(QTreeWidgetItem *item, int column);

    /*!
        \brief L'édition du nom de scripting de l'objet est terminée
        Propagation du changement de nom à l'éditeur d'objet
        \warning Il n'y a pas encore de vérifications sur les doublons dans les nom !!!!!!!!
    */
    void editionTerminated(QTreeWidgetItem *item, int column);

    /*!
        \brief Modification d'un item de type layout de la table à été modifié
        En fonction de l'état de la checkbox on affiche ou non les enfants sur la carte
    */
    void LayoutItemModified(QTreeWidgetItem *item, int column);


protected :

    /*!
        \brief Un objet vient d'être jeté sur la scène
    */
    void dropEvent(QDropEvent *event);

    /*!
        \brief Un clique de souris sur l'arbre
        Réimplémenté pour actualiser la selection des objets sur la carte
        \note On ne pouvait pas passer par le slot selectionChanged car on serait tombé dans une boucle : je selectionne sur la carte, la carte me dit que c'est selectionné donc
                je dois le selectionner ...
    */
    void mousePressEvent(QMouseEvent *event);

    /*!
        \brief Un relachement du clique de souris sur l'arbre
        Voir mousePressEvent, fait exactement la même chose
    */
    void mouseReleaseEvent(QMouseEvent *event);

    /*!
        \brief La selection des objets à changé, elle va être répercuté sur la carte !
    */
    void updateMapSelection();

    /*!
        \brief Déselectionne tous les objets sélectionnées sur la carte
        On conserve quand même le pointeur sur la carte =)
    */
    void clearMapSelection();


private :    

    QMap<QTreeWidgetItem *, GraphicsObject *> m_objectMap;      //!< Pour chaque ligne l'objet qui lui est associée

    Map *m_map;                                                 //!< La carte associé à cette table

    MapObjectEdit *m_graphicsObjectEditor;                      //!< Le formulaire associé à cette table pour l'édition d'objet (optionnel)

    MapAreaEdit *m_mapAreaEditor;                               //!< Le formulaire d'édition de zones

};

#endif // OBJECTINSPECTORTABLE_HPP
