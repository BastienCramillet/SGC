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

#include "FileSelector.hpp"

#include <QHBoxLayout>
#include <QFileDialog>


FileSelector::FileSelector(const QString &allowedFile, const QString &dirPath, const QString &windowsTitle, const QString &buttonLabel)
    : m_windowsTitle(windowsTitle), m_dirPath(dirPath), m_allowedFile(allowedFile)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    mainLayout->addWidget(&m_lineEdit);
    mainLayout->addWidget(&m_btn_browse);

    m_lineEdit.setReadOnly(true);

    m_btn_browse.setText(buttonLabel);
    connect(&m_btn_browse, SIGNAL(clicked()), SLOT(selectFile()));
}


void FileSelector::selectFile() {
    QString file = QFileDialog::getOpenFileName(this, m_windowsTitle, m_dirPath, m_allowedFile);
    if ( ! file.isEmpty() ) {
        selectFile(file);
    }
}


void FileSelector::selectFile(const QString &filePath) {
    m_selectedFilePath = filePath;
    m_lineEdit.setText(filePath.split("/").last() + "  (" + filePath + ")");
    m_lineEdit.setCursorPosition(0);
    emit selectedFileChanged(filePath);
}


QString FileSelector::getSelectedFilePath() const {
    return m_selectedFilePath;
}
