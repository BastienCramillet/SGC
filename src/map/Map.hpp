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


#ifndef MAP_HPP
#define MAP_HPP

#include "constantes.hpp"
#include <QGraphicsView>
#include <QSet>

class MapEditor;
class GraphicsObject;
class ObjectInspectorTable;
class MiniMapView;

const int GRID_SPACE = 20;  // espacement entre deux lignes/deux colonnes de la grille


/*!
    \class Map
    \brief Représentation graphique de la carte, la zone centrale de l'éditeur
*/

class Map : public QGraphicsView {

Q_OBJECT

public:

    Map(MapEditor *parent);

    /*!
        \brief Redimensionnement de la carte en fonction des paramètres passés
        Cela recréé la grille en arrière plan
    */
    void resize(int width, int height);

    /*!
        \brief Quand la touche Maj est enfoncée, on affiche l'objet à tracer à la place de la souris
        Sinon on affiche le curseur normal
    */
    void setShiftKeyDown(bool isDown);

    /*!
        \brief Renvoi la largeur de la carte
    */
    int width() const;

    /*!
        \brief Renvoi la hauteur de la carte
    */
    int height() const;

    /*!
        \brief Renvoi le nom de la carte
    */
    QString mapName() const;

    /*!
        \brief Fixe le nouveau nom de la carte
    */
    void setMapName(const QString &name);

    /*!
        \brief Ordonne de commencer une action de copier
        Va copier les objets en mémoire pour les coller plus tard
    */
    void copy();

    /*!
        \brief Ordonne l'action de coller
        Colle les objets en mémoire tampon sur la scene
    */
    void paste();

    /*!
        \brief Supprimer les objets selectionnés
    */
    void deleteSelectedItems();

    /*!
        \brief Ordonne de commencer une action de couper
        Va couper les objets selectionnées dans la mémoire
    */
    void cut();

    /*!
        \brief Demande la création d'une nouvelle carte
    */
    void newMap();

    /*!
        \brief Sauvegarde de la carte dans le fichier dont le nom et le nom seulement et donné en paramètre (juste le nom du fichier, pas le chemin !)
    */
    void save(const QString &name) const;

    /*!
        \brief Chargement d'une carte, le nom du fichier (toujours pas son chemin) et passé en paramètre
    */
    void load(const QString &name);

    /*!
        \brief Défini l'inspecteur d'objet associé à la carte
        Cela permet de faire en sorte que ce soit directement la carte qui communique avec sans avoir besoin de passer par MapEditor
    */
    void setObjectInspectorTable(ObjectInspectorTable *inspectorTable);

    /*!
        \brief Déselectionne les objets sélectionnés
    */
    void clearSelection();

    /*!
        \brief Force la selection de l'objet donné
    */
    void selectObject(GraphicsObject *object);

    /*!
        \brief Défini quel est la minivue de la carte
    */
    void setMiniMapView(MiniMapView *miniMapView);


public slots :

    /*!
        \brief Ouvre la fenêtre qui va demander l'édition des attributs de la carte
    */
    void openEditMapAttributes();

    /*!
        \brief Montre ou cache la grille en fonction de la valeur de show
    */
    void showGrid(bool show);

    /*!
        \brief Appellé quand l'objet selectionné dans la palette à changé. Va mettre à jour le curseur si nécéssaire
    */
    void paletteItemChanged();

    /*!
        \brief Défini s'il faut aligner ou non les objets sur la grille
    */
    void setAlignOnGrid(bool align);


protected :

    /*!
        \brief Vidage entier de la map (nouvelle carte)
    */
    void clear();

    /*!
        \brief Fournit la nouvelle selection à l'inspecteur
    */
    void commitNewSelection();

    /*!
        \brief Un objet vient d'être jeté sur la scène
    */
    void dropEvent(QDropEvent *event);

    /*!
        \brief Une entré de drag sur la carte.
    */
    void dragEnterEvent(QDragEnterEvent *event);

    /*!
        \brief On déplace un objet sur la scène avant de le jetter
    */
    void dragMoveEvent(QDragMoveEvent *event);

    /*!
        \brief Mouvement de la molette de la souris
        Va faire effectuer une rotation à l'objet sélectionné s'il existe,
        Sinon on scroll la carte selon la demande
    */
    void wheelEvent(QWheelEvent *event);

    /*!
        \brief Clique de la souris
        Si Maj est appuyé, on ajoute l'item en cours sur la carte, sinon on traite l'évenement noramlement.
    */
    void mousePressEvent(QMouseEvent *event);

    /*!
        \brief Relachement du bouton de la souris
        Dans ce cas il faut réaligner les objets sélectionnés sur la grille si nécessaire
    */
    void mouseReleaseEvent(QMouseEvent *event);

    /*!
        \brief Trace la grille en arrière plan
        Toute l'ancienne grille est détruite
    */
    void drawGrid();

    /*!
        \brief Supprime tous les objets dans le buffer (couper/copier)
    */
    void clearBuffer();

    /*!
        \brief Réaligne l'objet donné en paramètre sur la grille
    */
    void realignOnGrid(GraphicsObject *object);



    QGraphicsScene m_scene;                         //!< La scene

    int m_width;                                    //!< Largeur de la carte

    int m_height;                                   //!< Hauteur de la carte

    QSet<QGraphicsLineItem *> m_gridLineSet;        //!< Liste des lignes qui composent le cadrillage de l'éditeur

    QSet<GraphicsObject *> m_objectSet;             //!< Liste des objets que l'on a placé sur la carte

    MapEditor *m_parent;                            //!< L'éditeur de carte qui m'a créé

    bool m_isShiftKeyDown;                          //!< La touche maj est-elle enfoncée ?

    QString m_mapName;                              //!< Le nom de la carte

    QSet<GraphicsObject *> m_buffer;                //!< Buffer pour les couper/copier/coller

    bool m_alignOnGrid;                             //!< Faut-il aligner les objets sur la grille ?

    ObjectInspectorTable *m_objectInspectorTable;   //!< L'inspecteur d'objet (du moins la table) associé à la carte

    MiniMapView *m_miniMapView;                     //!< Une mini vue de la carte peut se déclarer à la map
};

#endif // MAP_HPP
