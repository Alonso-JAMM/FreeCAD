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

#ifndef GUI_KEDITOR_H
#define GUI_KEDITOR_H

#include <Gui/MDIView.h>
#include <Gui/Window.h>

namespace KTextEditor {
class View;
class Document;
class Cursor;
class Range;
}


namespace Gui {

class KEditorViewP;

class GuiExport KEditorView : public MDIView
{
    Q_OBJECT

public:
    KEditorView(QWidget* parent);
    ~KEditorView();

    void openFile(const QString &fileName, bool readOnly = false);

    bool onMsg(const char* pMsg, const char**);
    bool onHasMsg(const char* pMsg) const;

    bool canClose(void);

    QString fileName() const;

public Q_SLOTS:
    void executeScript();

protected:
    // event handling used to solve keyboard input issues (some keyboard inputs
    // not recognized by KTextEditor)
    bool event(QEvent* e);
    void closeEvent(QCloseEvent* e);

private:
    bool saveFile();

private:
    KEditorViewP* d;
};

}; // namespace Gui

#endif
