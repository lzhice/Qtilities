/****************************************************************************
**
** Copyright (c) 2009-2011, Jaco Naude
**
** This file is part of Qtilities which is released under the following
** licensing options.
**
** Option 1: Open Source
** Under this license Qtilities is free software: you can
** redistribute it and/or modify it under the terms of the GNU General
** Public License as published by the Free Software Foundation, either
** version 3 of the License, or (at your option) any later version.
**
** Qtilities is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public Licese
** along with Qtilities. If not, see http://www.gnu.org/licenses/.
**
** Option 2: Commercial
** Alternatively, this library is also released under a commercial license
** that allows the development of closed source proprietary applications
** without restrictions on licensing. For more information on this option,
** please see the project website's licensing page:
** http://www.qtilities.org/licensing.html
**
** If you are unsure which license is appropriate for your use, please
** contact support@qtilities.org.
**
****************************************************************************/


#include "TestFrontend.h"
#include "ui_TestFrontend.h"
#include "ITestable.h"

#include <stdio.h>
#include <time.h>

#include <QtilitiesCoreGui>
using namespace QtilitiesCoreGui;

struct Qtilities::UnitTests::TestFrontendPrivateData {
    ObserverWidget          tests_observer_widget;
    ActivityPolicyFilter*   tests_activity_filter;
    TreeNode                tests_observer;
    char **                 argv;
    int                     argc;
    QPointer<QWidget>       log_widget;
    bool                    multiple_tests;
};

Qtilities::UnitTests::TestFrontend::TestFrontend(int argc, char ** argv, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestFrontend)
{
    ui->setupUi(this);
    d = new TestFrontendPrivateData;
    d->argc = argc;
    d->argv = argv;
    d->multiple_tests = false;
    setWindowTitle(tr("Application Tester"));

    d->tests_activity_filter = d->tests_observer.enableActivityControl(ObserverHints::CheckboxActivityDisplay,ObserverHints::CheckboxTriggered);
    d->tests_activity_filter->setNewSubjectActivityPolicy(ActivityPolicyFilter::SetNewActive);

    d->tests_observer.enableCategorizedDisplay();
    d->tests_observer.setObjectName(tr("Registered Tests"));
    d->tests_observer.displayHints()->setDisplayFlagsHint(ObserverHints::ItemView | ObserverHints::ActionToolBar);

    d->tests_observer_widget.setObserverContext(&d->tests_observer);
    d->tests_observer_widget.initialize();

    if (ui->widgetTestListHolder->layout())
        delete ui->widgetTestListHolder->layout();

    QHBoxLayout* layout = new QHBoxLayout(ui->widgetTestListHolder);
    layout->setMargin(0);
    layout->addWidget(&d->tests_observer_widget);
}

Qtilities::UnitTests::TestFrontend::~TestFrontend()
{
    delete ui;
}

void Qtilities::UnitTests::TestFrontend::addTest(ITestable* test, QtilitiesCategory category) {
    if (!test)
        return;

    if (!test->objectBase())
        return;

    if (category.isValid()) {
        MultiContextProperty category_property(qti_prop_CATEGORY_MAP);
        category_property.setValue(qVariantFromValue(category),d->tests_observer.observerID());
        Observer::setMultiContextProperty(test->objectBase(),category_property);
    }

    test->objectBase()->setObjectName(test->testName());
    d->tests_observer << test->objectBase();
    d->tests_observer_widget.viewExpandAll();
}

void Qtilities::UnitTests::TestFrontend::on_btnExecute_clicked()
{
    int success_count = 0;
    int error_count = 0;

    time_t start,end;
    time(&start);

    ui->txtResults->setText(QString(tr("Testing in progress...")));
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    if (d->multiple_tests) {
        QList<QObject*> active_tests = d->tests_activity_filter->activeSubjects();
        for (int i = 0; i < active_tests.count(); i++) {
            ITestable* test = qobject_cast<ITestable*> (active_tests.at(i));
            if (test) {
                SharedProperty property(qti_prop_DECORATION,QVariant(QIcon()));
                Observer::setSharedProperty(active_tests.at(i), property);
            }
        }
    }

    // Execute the tests:
    QList<QObject*> active_tests = d->tests_activity_filter->activeSubjects();
    for (int i = 0; i < active_tests.count(); i++) {
        ITestable* test = qobject_cast<ITestable*> (active_tests.at(i));
        if (test) {
            if (test->execTest(d->argc,d->argv) == 0) {
                SharedProperty property(qti_prop_DECORATION,QVariant(QIcon(qti_icon_SUCCESS_12x12)));
                Observer::setSharedProperty(active_tests.at(i), property);
                ++success_count;
            } else {
                SharedProperty property(qti_prop_DECORATION,QVariant(QIcon(qti_icon_ERROR_12x12)));
                Observer::setSharedProperty(active_tests.at(i), property);
                ++error_count;
            }
        }
    }

    QApplication::restoreOverrideCursor();
    time(&end);
    double diff = difftime(end,start);
    ui->txtResults->setText(QString(tr("Testing completed in %1 seconds: %2 passed, %3 failed.")).arg(diff).arg(success_count).arg(error_count));
    d->multiple_tests = true;
}

void Qtilities::UnitTests::TestFrontend::on_btnShowLog_clicked()
{
    if (!d->log_widget)
         d->log_widget = LoggerGui::createLogWidget("Test Log");

    d->log_widget->show();
}
