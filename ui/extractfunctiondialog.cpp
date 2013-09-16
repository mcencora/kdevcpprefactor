/*
 * Copyright (c) 2013 Maciej <m.cencora@gmail.com>
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

#include "extractfunctiondialog.h"
#include "ui_extractfunctiondialog.h"

#include <KLocalizedString>
#include <QToolTip>

ExtractFunctionDialog::ExtractFunctionDialog()
{
    setWindowTitle(i18n("Extract function"));
    QWidget *w = new QWidget(this);
    ui = new Ui::ExtractFunction;
    ui->setupUi(w);
    ui->functionName->setFocus();
    ui->userFeedback->setVisible(false);
    ui->groupBox->setVisible(false);
    ui->groupBox_3->setVisible(false);


    setMainWidget(w);

    QRegExpValidator *v = new QRegExpValidator(QRegExp("^[a-zA-Z_][a-zA-Z0-9_]+$"));
    ui->functionName->setValidator(v);

    connect(ui->functionName, SIGNAL(textChanged(QString)), this, SLOT(onFunctionNameChange()));
}

void ExtractFunctionDialog::accept()
{
    if (!ui->functionName->hasAcceptableInput())
    {
        ui->userFeedback->setMessageType(KMessageWidget::Error);
        ui->userFeedback->setCloseButtonVisible(false);
        ui->userFeedback->setText(i18n("Invalid function name"));
        ui->userFeedback->animatedShow();
        ui->functionName->setFocus();
        return;
    }

    emit accepted(ui->functionName->text());
    QDialog::accept();
}

void ExtractFunctionDialog::onFunctionNameChange()
{
    if (ui->functionName->hasAcceptableInput())
        ui->userFeedback->animatedHide();
}
