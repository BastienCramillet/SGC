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


#ifndef FILESELECTOR_HPP
#define FILESELECTOR_HPP

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QPushButton>


/*!
    \class FileSelector
    \brief Selecteur de fichier.
    Il se décompose en deux parties, un lineEdit affiche le nom du fichier et son chemin et un bouton permet de sélectionner un autre fichier.
*/

class FileSelector : public QWidget {

Q_OBJECT

public :

    /*!
        \brief Au départ aucun fichier n'est selectionné
        \param allowedFile Par exemple : "Images (*.png *.xpm *.jpg)" (voir http://doc.qt.nokia.com/latest/qfiledialog.html#getOpenFileName)
        \param dirPath Le chemin vers le dossier proposé par défaut lors de la selection d'un fichier
        \param windowsTitle Le titre de la fenêtre qui va s'ouvrir quand on appuira sur le bouton parcourir
        \param buttonLabel Le texte qui sera affiché sur le bouton
        \see http://doc.qt.nokia.com/latest/qfiledialog.html#getOpenFileName
    */
    FileSelector(
            const QString &allowedFile,
            const QString &dirPath      = "",
            const QString &windowsTitle = "Selectionnez un fichier",
            const QString &buttonLabel  = "..."
            );

    /*!
        \brief Selectionner ce fichier
    */
    void selectFile(const QString &filePath);

    /*!
        \brief Renvoi le chemin du fichier selectionné
    */
    QString getSelectedFilePath() const;


signals :

    /*!
        \brief Ce signal est émit quand le fichier selectionné change. La chaîne contient le nouveau chemin vers le fichier
    */
    void selectedFileChanged(QString);


protected slots :

    /*!
        \brief Selection d'un autre fichier lors du clique sur le bouton (ouvre une popup)
    */
    void selectFile();


protected :

    QString m_windowsTitle;     //!< Le titre de la fenêtre qui va s'ouvrir quand on appuira sur le bouton parcourir

    QString m_dirPath;          //!< Le chemin vers le dossier proposé par défaut lors de la selection d'un fichier

    QString m_selectedFilePath; //!< Le chemin vers le fichier selectionné

    QString m_allowedFile;      //!< Les formats de fichier autorisés

    QLineEdit m_lineEdit;       //!< Le line edit qui contient le nom du fichier et son chemin

    QPushButton m_btn_browse;   //!< Le bouton qui permet de parcourir pour chercher un autre fichier

};

#endif // FILESELECTOR_HPP
