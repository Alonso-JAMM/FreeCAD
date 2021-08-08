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


#ifndef EDITORCOMMANDS_H
#define EDITORCOMMANDS_H

#include <QString>
#include <QStringList>
#include "MDIView.h"


namespace Gui {


namespace CustomEditor {
    /**
     * Opens a text file using the preferred custom text editor.
     */
    void openTextFile(const QString &filename, bool readOnly = false);

    /**
     * Checks if preferred editor is a custom editor
     */
    bool isEnabled();

    /**
     * Adds a debug marker to the given filename at the given line
     */
    void showDebugMarker(const QString& filename, int line);

    /**
     * Removes the debug marker in the given filename
     */
    void hideDebugMarker(const QString& filename);

    /**
     * Gets a list of all the undo actions of the custom text editor
     */
    QStringList getUndoActions(MDIView* mdi);

    /**
     * Gets a list of all the redo actions of the custom text editor
     */
    QStringList getRedoActions(MDIView* mdi);

    /**
     * Checks whether the MDIView object is a custom text editor
     */
    bool isCustomEditor(MDIView* mdi);
};

}; // namespace Gui


#endif
