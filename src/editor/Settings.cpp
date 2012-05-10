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


#include "Settings.hpp"

#include <QtXml>

// création de l'instance static
Settings Settings::m_sharedSettings;


Settings::Settings() {
    loadXML();
}

void Settings::loadXML() {

    QDomDocument doc;

    QFile f(EDITOR_DATA_DIR + "/editorsSettings.xml");
    if ( ! f.open(QIODevice::ReadOnly) ) {
        Log::e("Settings") << "Impossible d'ouvrir" << f.fileName() << ":" << f.errorString();
        return;
    }
    doc.setContent(&f);
    f.close();

    QDomElement root = doc.documentElement();

    QDomNode n = root.firstChild();
    while( !n.isNull() )
    {
        QDomElement e = n.toElement();
        if( !e.isNull() )
        {

            if ( e.tagName() == "editorsDecorsCategories" ) {

                QDomNodeList childrens = e.childNodes();
                for (int i=0; i<childrens.size(); ++i) {
                    Settings::m_sharedSettings.m_decorsEditorsCategories << childrens.at(i).toElement().attribute("name");
                }

            } else if ( e.tagName() == "editorsCharactersCategories" ) {

                QDomNodeList childrens = e.childNodes();
                for (int i=0; i<childrens.size(); ++i) {
                    Settings::m_sharedSettings.m_charactersEditorsCategories << childrens.at(i).toElement().attribute("name");
                }

            } else if ( e.tagName() == "gameCharacteresType" ) {

                QDomNodeList childrens = e.childNodes();
                for (int i=0; i<childrens.size(); ++i) {
                    Settings::m_sharedSettings.m_charactersGamesCategories << childrens.at(i).toElement().attribute("name");
                }

            } else if ( e.tagName() == "gameDecorsType" ) {

                QDomNodeList childrens = e.childNodes();
                for (int i=0; i<childrens.size(); ++i) {
                    Settings::m_sharedSettings.m_decorsGamesCategories << childrens.at(i).toElement().attribute("name");
                }

            } else if ( e.tagName() == "triggers" ) {

                QDomNodeList childrens = e.childNodes();
                for (int i=0; i<childrens.size(); ++i) {
                    Settings::m_sharedSettings.m_triggerList << childrens.at(i).toElement().attribute("name");
                }

            } else if ( e.tagName() == "executables" ) {

                QDomNodeList childrens = e.childNodes();
                for (int i=0; i<childrens.size(); ++i) {
                    if ( childrens.at(i).toElement().attribute("os", "") == "windows" ) {
                        Settings::m_sharedSettings.m_windowsExecutable = childrens.at(i).toElement().attribute("value", "");
                    } else if ( childrens.at(i).toElement().attribute("os", "") == "linux" ) {
                        Settings::m_sharedSettings.m_linuxExecutable = childrens.at(i).toElement().attribute("value", "");
                    }
                }

            } else if ( e.tagName() == "areas") {

                int width = e.toElement().attribute("width").toInt();
                int height = e.toElement().attribute("height").toInt();
                m_sharedSettings.m_areasDefaultDatas.size = QSize(width, height);
                m_sharedSettings.m_areasDefaultDatas.name = "Zone";      // Nom de la zone par défaut !!!
                m_sharedSettings.m_areasDefaultDatas.color = QColor(e.toElement().attribute("color"));

                QDomNodeList childrens = e.childNodes();
                for (int i=0; i<childrens.size(); ++i) {
                    AreasData ad;
                    int width = childrens.at(i).toElement().attribute("width").toInt();
                    int height = childrens.at(i).toElement().attribute("height").toInt();
                    ad.size = QSize(width, height);
                    ad.color = QColor(childrens.at(i).toElement().attribute("color"));
                    ad.name = childrens.at(i).toElement().attribute("name");
                    Settings::m_sharedSettings.m_areasList << ad;
                }
            }


        }
        n = n.nextSibling();
    }

}

QStringList Settings::getDecorsEditorsCategories() {
    return Settings::m_sharedSettings.m_decorsEditorsCategories;
}

QStringList Settings::getCharactersEditorsCategories() {
    return Settings::m_sharedSettings.m_charactersEditorsCategories;
}

QStringList Settings::getDecorsGamesCategories() {
    return Settings::m_sharedSettings.m_decorsGamesCategories;
}

QStringList Settings::getCharactersGamesCategories() {
    return Settings::m_sharedSettings.m_charactersGamesCategories;
}

QStringList Settings::getTriggerList() {
    return Settings::m_sharedSettings.m_triggerList;
}

QString Settings::getWindowsExecutable() {
    return Settings::m_sharedSettings.m_windowsExecutable;
}

QString Settings::getLinuxExecutable() {
    return Settings::m_sharedSettings.m_linuxExecutable;
}

QList<AreasData> Settings::getAreasList() {
    return Settings::m_sharedSettings.m_areasList;
}

AreasData Settings::getAreasDefaultDatas() {
    return m_sharedSettings.m_areasDefaultDatas;
}
