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
#include <QLatin1String>
#include <QKeySequence>
#endif

#include "KEditorView.h"

#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <KTextEditor/View>
#include <KStandardAction>

#include "BitmapFactory.h"
#include "MainWindow.h"
#include "Application.h"
#include "Macro.h"

#include <Base/Interpreter.h>
#include <Base/Parameter.h>
#include <Base/Exception.h>

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

    Gui::MainWindow* mw = Gui::getMainWindow();
    connect(this, SIGNAL(copyAvailable(bool)), mw, SLOT(updateEditorActions()));
    connect(d->view, SIGNAL(selectionChanged(KTextEditor::View*)), 
            this, SLOT(selectionChanged(KTextEditor::View*)));
    
    // Remove the shortcuts to duplicated actions (these actions are handled by FreeCAD)
    QAction* copyAction = d->view->action(KStandardAction::name(KStandardAction::Copy));
    copyAction->setShortcut(QKeySequence());   
}

KEditorView::~KEditorView(){
    delete d->view;
    delete d->doc;
    delete d;
}

bool KEditorView::onMsg(const char* pMsg, const char**) {
    // don't allow any actions if the editor is being closed
    if (d->aboutToClose)
        return false;

    if (strcmp(pMsg, "Run") == 0) {
        executeScript();
        return true;
    }
    if (strcmp(pMsg, "Save") == 0) {
        saveFile();
        return true;
    }
    else if (strcmp(pMsg, "Copy") == 0) {
        QAction* copyAction = d->view->action(KStandardAction::name(KStandardAction::Copy));
        copyAction->trigger();
        return true;
    }

    return false;
}

/**
 * Checks if the action \a pMsg is available. This is for enabling/disabling
 * the corresponding buttons or menu items for this action. From EditorView
 */
bool KEditorView::onHasMsg(const char* pMsg) const {
    // don't allow any actions if the editor is being closed
    if (d->aboutToClose)
        return false;
    if (strcmp(pMsg, "Run") == 0)
        return true;
    if (strcmp(pMsg, "Save") == 0)
        return d->doc->isModified();
    else if (strcmp(pMsg, "Copy") == 0)
        return d->view->selection();
    
    return false;
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


/**
 * Runs the opened script in the macro manager.
 */
void KEditorView::executeScript() {
    // always save the macro when it is modified
    if (KEditorView::onHasMsg("Save"))
        KEditorView::onMsg("Save", 0);
    try {
        Application::Instance->macroManager()->run(Gui::MacroManager::File,fileName().toUtf8());
    }
    catch (const Base::SystemExitException&) {
        // handle SystemExit exceptions
        Base::PyGILStateLocker locker;
        Base::PyException e;
        e.ReportException();
    }
}


void KEditorView::selectionChanged(KTextEditor::View *view) {
    Q_UNUSED(view);
    copyAvailable(true);
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

void KEditorView::closeEvent(QCloseEvent* event) {
    MDIView::closeEvent(event);
    if (event->isAccepted()) {
        d->aboutToClose = true;
        Gui::MainWindow* mw = Gui::getMainWindow();
        mw->updateEditorActions();
    }
}

QString KEditorView::fileName() const {
    return d->doc->url().path();
}

bool KEditorView::saveFile() {
    return d->doc->documentSave();
}

#include "moc_KEditorView.cpp"
