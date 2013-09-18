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

#include "CppManip.hpp"
#include "SourceReplacement.hpp"

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
SourceSelection rangeToSourceSelection(const KTextEditor::Document& doc,
                                       const KTextEditor::Range& range)
{
    SourceSelection ss;

    for (int i = 0; i < range.start().line(); ++i)
    {
        ss.from += doc.line(i).length() + 1;
    }
    ss.to = ss.from;

    ss.from += range.start().column();

    for (int i = range.start().line(); i < range.end().line(); ++i)
    {
        ss.to += doc.line(i).length() + 1;
    }
    ss.to += range.end().column();
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
        SourceSelection selection = rangeToSourceSelection(*context->view()->document(),
                                                           range);
        QString fileName = context->url().path();
        SourceReplacements reps = extractFunctionInFile(functionName.toAscii().constData(),
                                                        selection,
                                                        fileName.toLocal8Bit().constData());
        SourceModificationsApplier app;
        app.apply(context->view()->document(), reps);
    }
    catch (const std::exception&)
    { }
}

#include "refactorplugin.moc"

