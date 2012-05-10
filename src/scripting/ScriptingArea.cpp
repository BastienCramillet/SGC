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


#include "ScriptingArea.hpp"

#include <QtXml>

#include "MapEditor.hpp"

ScriptingArea::ScriptingArea(MapEditor *parent) : QDockWidget(parent) {

    setWindowTitle("Zone de scripting");


    // partie zone de texte

    QsciLexerLua *lexerLua = new QsciLexerLua();
    m_textZone.setLexer(lexerLua);
    m_textZone.setUtf8(true);
    m_textZone.setMarginLineNumbers(1, true); // numérotation des lignes
    m_textZone.setMarginWidth(1, 30); // agrandissement de la marge

    //this->setAutoCompletionSource(AcsAPIs);
    //this->setAutoCompletionThreshold(1);

    QsciLexerLua *lexLua= new QsciLexerLua();
    QsciAPIs *api = new QsciAPIs(lexLua);
    if ( ! api->load(":/ressources/autocompetion.api") ) {
        Log::e("ScriptingArea") << "Erreur chargement autocompletion.api";
    }
    api->prepare();
    m_textZone.setAutoCompletionSource(QsciScintilla::AcsAPIs);
    m_textZone.setLexer(lexLua);
    m_textZone.setAutoCompletionThreshold(1);

    clear();


    // partie arbre des fonctions possibles

    QTreeWidget *treeDoc = new QTreeWidget();
    treeDoc->setHeaderHidden(true);

    QDomDocument doc;

    QFile f(EDITOR_DATA_DIR + "/scriptingFunctions.xml");
    if ( ! f.open(QIODevice::ReadOnly) ) {
        Log::e("Editor") << "erreur ouverture " << f.fileName() << " : " << f.errorString();
    }
    doc.setContent(&f);
    f.close();

    QDomElement root = doc.documentElement();

    QDomNode n = root.firstChild();
    while( ! n.isNull() ) {

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, n.toElement().tagName());
        treeDoc->insertTopLevelItem(0, item);

        QDomNode instance = n.firstChild();
        while ( ! instance.isNull() ) {
            QTreeWidgetItem *item2 = new QTreeWidgetItem();
            item2->setText(0, instance.toElement().attribute("signature"));
            item->addChild(item2);
            instance = instance.nextSibling();
        }

        n = n.nextSibling();
    }


    // ajout dans le layout

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(&m_textZone);
    splitter->addWidget(treeDoc);

    // on agrandit le premier widget, la zone de texte
    splitter->setStretchFactor(0, 0.75 * width());

    QHBoxLayout *scriptingDockLayout = new QHBoxLayout();
    scriptingDockLayout->addWidget(splitter);

    QWidget *scriptingDockContent = new QWidget();
    setWidget(scriptingDockContent);
    scriptingDockContent->setLayout(scriptingDockLayout);

}


void ScriptingArea::save(const QString &name) const {

    Log::i("ScriptingArea") << "sauvegarde du script dans : " << SCRIPTS_DIR << name;

    QFile fich(QString(SCRIPTS_DIR) + name);
    fich.open(QIODevice::WriteOnly);
    QByteArray ba;
    ba.append(m_textZone.text().toLatin1());
    fich.write(ba);
    fich.close();

    Log::d("ScriptingArea") << "sauvegarde terminée";
}


void ScriptingArea::load(const QString &name) {

    Log::i("ScriptingArea") << "chargement du script dans : " << SCRIPTS_DIR << name;

    QFile fich(QString(SCRIPTS_DIR) + name);
    fich.open(QIODevice::ReadOnly);
    QByteArray ba;
    ba = fich.readAll();
    m_textZone.setText(QString::fromLatin1(ba));
    fich.close();

    Log::d("ScriptingArea") << "chargement terminé";
}


void ScriptingArea::clear() {
    m_textZone.setText("--[[\nScripting\nDate : " + QDate::currentDate().toString("dd/MM/yyyy") + "\nAuteur : \n--]]\n\n");
}
