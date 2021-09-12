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

#ifndef GUI_DIALOG_DLGSETTINGSCUSTOMEDITOR_H
#define GUI_DIALOG_DLGSETTINGSCUSTOMEDITOR_H


#include "PropertyPage.h"

namespace Gui {
namespace Dialog {
class Ui_DlgSettingsCustomEditor;

/**
 * This class implements a preference page for the custom editor settings.
 * You can choose the desired custom editor to be used by FreeCAD to edit
 * text, python, or macro files.
 */
class DlgSettingsCustomEditorImp : public PreferencePage
{
    Q_OBJECT

public:
    DlgSettingsCustomEditorImp(QWidget* parent = nullptr);
    ~DlgSettingsCustomEditorImp();

public:
    void saveSettings();
    void loadSettings();

protected:
    void changeEvent(QEvent *e);

private:
    std::unique_ptr<Ui_DlgSettingsCustomEditor> ui;
};


}; // namespace Dialog
}; // namespace Gui

#endif // GUI_DIALOG_DLGSETTINGSCUSTOMEDITOR_H
