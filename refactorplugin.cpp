/*
 * Copyright (c) 2013 Maciej Cencora <m.cencora@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "refactorplugin.h"
#include <interfaces/contextmenuextension.h>
#include <language/interfaces/editorcontext.h>
#include <QAction>
#include <QString>

#include <KPluginFactory>
#include <KAboutData>
#include <KTextEditor/Document>
#include <KMessageBox>

#include <CppManip.hpp>

#include "sourcemodificationsapplier.hpp"
#include "ui/extractfunctiondialog.h"

K_PLUGIN_FACTORY(RefactorPluginFactory, registerPlugin<RefactorPlugin>();)
K_EXPORT_PLUGIN(RefactorPluginFactory(
    KAboutData("kdevcpprefactor",
               "kdevcpprefactor",
               ki18n("C/C++ code refactoring support"),
               "0.0.1",
               ki18n("C/C++ code refactoring support"),
               KAboutData::License_GPL)
    .addAuthor(ki18n("Maciej Cencora"), ki18n("Author"), "m.cencora@gmail.com")))

RefactorPlugin::RefactorPlugin(QObject* parent, const QVariantList&)
    : IPlugin(RefactorPluginFactory::componentData(), parent)
{}

KDevelop::ContextMenuExtension RefactorPlugin::contextMenuExtension(KDevelop::Context* context)
{
    KDevelop::ContextMenuExtension extension;
    if ((this->context = dynamic_cast<KDevelop::EditorContext*>(context))) {
        QAction* action = new QAction(i18n("Extract function"), this);
        // action->setData(QVariant::fromValue(IndexedDeclaration(declaration)));
        connect(action, SIGNAL(triggered(bool)), this, SLOT(showExtractFunction()));
        extension.addAction(KDevelop::ContextMenuExtension::RefactorGroup, action);
    }
    return extension;
}
namespace
{
cppmanip::SourceSelection rangeToSourceSelection(const KTextEditor::Range& range)
{
    cppmanip::SourceSelection ss;
    ss.from.col = range.start().column();
    ss.from.row = range.start().line();
    ss.to.col = range.end().column();
    ss.to.row = range.start().line();

    return ss;
}
}


void RefactorPlugin::showExtractFunction()
{
    ExtractFunctionDialog *d = new ExtractFunctionDialog();
    connect(d, SIGNAL(accepted(QString)), this, SLOT(executeExtractFunction(QString)));
    d->exec();
    delete d;
}

void RefactorPlugin::executeExtractFunction(const QString& functionName)
{
    try
    {
        KTextEditor::Range range = context->view()->selectionRange();
        if (range.isEmpty())
        {
            range = KTextEditor::Range(context->view()->cursorPosition(),
                                       context->view()->cursorPosition());
        }
        cppmanip::SourceSelection selection = rangeToSourceSelection(range);
        QString fileName = context->url().path();
        cppmanip::SourceReplacements reps = cppmanip::extractFunctionInFile(functionName.toAscii().constData(),
                                                                            selection,
                                                                            fileName.toLocal8Bit().constData());
        SourceModificationsApplier app;
        app.apply(context->view()->document(), reps);
        context->view()->setSelection(KTextEditor::Range());
    }
    catch (const cppmanip::ExtractMethodError& e)
    {
        KMessageBox::error(nullptr, e.what(), i18n("Function extraction failed"));
    }
    catch (const std::exception& e)
    {
        KMessageBox::error(nullptr, i18n("Internal error: ") + e.what(), i18n("Function extraction failed"));
    }
}

#include "refactorplugin.moc"

