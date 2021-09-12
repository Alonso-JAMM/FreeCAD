/***************************************************************************
 *   Copyright (c) 2021 Alonso Mendoza <jalonsomendoza32@gmail.com>        *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"
#ifndef _PreComp_
#endif

#include "CustomEditor.h"

#if defined BUILD_KTEXTEDITOR
#include "KTextEditor/KEditorView.h"
#include "MainWindow.h"
#endif

using namespace Gui;

#if defined BUILD_KTEXTEDITOR
void CustomEditor::openTextFile(const QString &filename, bool readOnly) {
    KEditorView* edit = new KEditorView(getMainWindow());
    edit->openFile(filename, readOnly);
    getMainWindow()->addWindow(edit);
}

bool CustomEditor::isEnabled() {
    ParameterGrp::handle hGrp = App::GetApplication().GetUserParameter().
        GetGroup("BaseApp")->GetGroup("Preferences")->GetGroup("CustomEditor");
    bool use_keditor = hGrp->GetBool("EnableKTextEditor", false);

    if (use_keditor) {
        return true;
    }
    else {
        return false;
    }
}

void CustomEditor::showDebugMarker(const QString& filename, int line) {
    (void)filename;
    (void)line;
}

void CustomEditor::hideDebugMarker(const QString& filename) {
    (void)filename;
}

bool CustomEditor::isCustomEditor(MDIView* mdi) {
    (void)mdi;
    return false;
}

QStringList CustomEditor::getUndoActions(MDIView* mdi) {
    (void)mdi;
    QStringList vecUndos = QStringList();
    return vecUndos;
}

QStringList CustomEditor::getRedoActions(MDIView* mdi) {
    (void)mdi;
    QStringList vecUndos = QStringList();
    return vecUndos;
}


#else

void CustomEditor::openTextFile(const QString &filename, bool readOnly) {
    (void)filename;
    (void)readOnly;
}

bool CustomEditor::isEnabled() {
    return false;
}

void CustomEditor::showDebugMarker(const QString& filename, int line) {
    (void)filename;
    (void)line;
}

void CustomEditor::hideDebugMarker(const QString& filename) {
    (void)filename;
}

bool CustomEditor::isCustomEditor(MDIView* mdi) {
    (void)mdi;
    return false;
}

QStringList CustomEditor::getUndoActions(MDIView* mdi) {
    (void)mdi;
    QStringList vecUndos = QStringList();
    return vecUndos;
}

QStringList CustomEditor::getRedoActions(MDIView* mdi) {
    (void)mdi;
    QStringList vecUndos = QStringList();
    return vecUndos;
}

#endif // BUILD_KTEXTEDITOR
