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
#include <QApplication>
#include <QUrl>
#include <QKeyEvent>
#include <QFileInfo>
#include <QMessageBox>
#include <QAction>
#endif

#include "KEditorView.h"

#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <KTextEditor/View>

#include "BitmapFactory.h"
#include "MainWindow.h"

using namespace Gui;
namespace Gui {
class KEditorViewP {
public:
    KTextEditor::Document *doc;
    KTextEditor::View *view;
    bool aboutToClose;
};
}


KEditorView::KEditorView(QWidget* parent)
    : MDIView(0,parent,Qt::WindowFlags())
{

    d = new KEditorViewP;
    d->aboutToClose = false;
    KTextEditor::Editor *editor = KTextEditor::Editor::instance();
    d->doc = editor->createDocument(this);
    d->view = d->doc->createView(this);
    setCentralWidget(d->view);
}

KEditorView::~KEditorView(){
    delete d->view;
    delete d->doc;
    delete d;
}

bool KEditorView::canClose(void) {
    if (d->doc->isModified()) {
        this->setFocus(); // raises the view to front
        switch( QMessageBox::question(this, tr("Unsaved document"),
                                    tr("The document has been modified.\n"
                                       "Do you want to save your changes?"),
                                     QMessageBox::Yes|QMessageBox::Default, QMessageBox::No,
                                     QMessageBox::Cancel|QMessageBox::Escape))
    {
        case QMessageBox::Yes:
            return saveFile();
        case QMessageBox::No:
            return true;
        case QMessageBox::Cancel:
            return false;
        default:
            return false;
    }
    }
    else {
        return true;
    }
}

void KEditorView::openFile(const QString &fileName, bool readOnly) {
    QUrl url = QUrl::fromLocalFile(fileName);
    d->doc->setReadWrite(!readOnly);
    d->doc->openUrl(url);
    QFileInfo fi(fileName);
    QString ext = fi.suffix().toLower();
    if (ext == QLatin1String("py") || ext == QLatin1String("fcmacro") ||
        ext == QLatin1String("fcscript")) {
        setWindowIcon(Gui::BitmapFactory().iconFromTheme("applications-python"));
    }
    else {
        setWindowIcon(Gui::BitmapFactory().iconFromTheme("TextDocument.svg"));
    }
    if (readOnly) {
        setWindowTitle(fi.fileName() + QLatin1String("  [Read-only]"));
    }
    else {
        setWindowTitle(fi.fileName());
    }
}

bool KEditorView::event(QEvent* e) {
    // HACK: when keys are pressed QEvent::ShortcutOverride is emitted. If we
    // accept all the key events except for the modifiers and other special keys,
    // then text will be added to the text field and shortcuts (like Ctrl+A or Ctrl+S)
    // will also be enabled.
    if (e->type() == QEvent::ShortcutOverride)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);
        int key = ke->key();
        // no modifiers and other special keys
        if (!ke->modifiers() & (key != Qt::Key_Backspace) & (key != Qt::Key_Insert)
            & (key != Qt::Key_Delete) & (key != Qt::Key_Home) & (key != Qt::Key_End)
            & (key != Qt::Key_Left) & (key != Qt::Key_Up) & (key != Qt::Key_Right)
            & (key != Qt::Key_Down) & (key != Qt::Key_PageUp) & (key != Qt::Key_PageDown))
        {
            e->accept();
        }
    }
    return QWidget::event(e);
}

bool KEditorView::saveFile() {
    return d->doc->documentSave();
}

#include "moc_KEditorView.cpp"
