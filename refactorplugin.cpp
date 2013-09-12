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

#include <KPluginFactory>
#include <KAboutData>

#include "CppManip.hpp"

K_PLUGIN_FACTORY(RefactorPluginFactory, registerPlugin<RefactorPlugin>();)
K_EXPORT_PLUGIN(RefactorPluginFactory(
    KAboutData("kdevcppclangrefactor",
               "kdevcppclangrefactor",
               ki18n("Clang based C/C++ code refactoring support"),
               "0.0.1",
               ki18n("Clang based C/C++ code refactoring support"),
               KAboutData::License_GPL)
    .addAuthor(ki18n("Maciej Cencora"), ki18n("Author"), "m.cencora@gmail.com")))

RefactorPlugin::RefactorPlugin(QObject* parent, const QVariantList&)
    : IPlugin(RefactorPluginFactory::componentData(), parent)
{}

KDevelop::ContextMenuExtension RefactorPlugin::contextMenuExtension(KDevelop::Context* context)
{
    KDevelop::ContextMenuExtension extension;
    if (dynamic_cast<KDevelop::EditorContext*>(context)) {
        QAction* action = new QAction(i18n("Extract function"), this);
        // action->setData(QVariant::fromValue(IndexedDeclaration(declaration)));
        connect(action, SIGNAL(triggered(bool)), this, SLOT(executeExtractFunction()));
        extension.addAction(KDevelop::ContextMenuExtension::RefactorGroup, action);
    }
    return extension;
}

void RefactorPlugin::executeExtractFunction()
{
    try
    {
        SourceSelection selection;
        selection.from = 35;
        selection.to = 80;
        extractMethodInFile("Dummy", selection, "/home/maciej/refactorsample.cpp");
    }
    catch (const std::exception&)
    { }
}

#include "refactorplugin.moc"

