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

#include <fstream>

#include "MapEditor.hpp"

#include "constantes.hpp"

#include <QtGui>
#include <QApplication>
#include <QTextCodec>

//#include <Qsci/qsciscintilla.h>

int main(int argc, char *argv[]) {

    //*
    std::ofstream file("logs.txt");
    Log::setOutput(file);
    //*/

    Log::v("main") << "Entrée dans le main";

    QApplication app(argc, argv);
    app.setApplicationName(editorName);

    Q_INIT_RESOURCE(ressources);

    // encodage des fichiers
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    MapEditor mapEditor;
    mapEditor.showMaximized();

    return app.exec();
}
