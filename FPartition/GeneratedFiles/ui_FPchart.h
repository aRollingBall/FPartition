/********************************************************************************
** Form generated from reading UI file 'FPchart.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FPCHART_H
#define UI_FPCHART_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>
#include <qchartview.h>

QT_BEGIN_NAMESPACE

class Ui_FPChart
{
public:
    QChartView *widget;

    void setupUi(QWidget *FPChart)
    {
        if (FPChart->objectName().isEmpty())
            FPChart->setObjectName(QStringLiteral("FPChart"));
        FPChart->resize(400, 300);
        widget = new QChartView(FPChart);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(40, 40, 271, 241));

        retranslateUi(FPChart);

        QMetaObject::connectSlotsByName(FPChart);
    } // setupUi

    void retranslateUi(QWidget *FPChart)
    {
        FPChart->setWindowTitle(QApplication::translate("FPChart", "FPChar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FPChart: public Ui_FPChart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FPCHART_H
