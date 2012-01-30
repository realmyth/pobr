/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri 6. Jan 14:20:49 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOtw_rz_obraz;
    QAction *actionWyostrz;
    QAction *actionWykrywaj;
    QAction *actionRozja_nij;
    QAction *actionWykrywaj_kraw_dzie;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QMenu *menuMenu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(566, 422);
        actionOtw_rz_obraz = new QAction(MainWindow);
        actionOtw_rz_obraz->setObjectName(QString::fromUtf8("actionOtw_rz_obraz"));
        actionWyostrz = new QAction(MainWindow);
        actionWyostrz->setObjectName(QString::fromUtf8("actionWyostrz"));
        actionWykrywaj = new QAction(MainWindow);
        actionWykrywaj->setObjectName(QString::fromUtf8("actionWykrywaj"));
        actionRozja_nij = new QAction(MainWindow);
        actionRozja_nij->setObjectName(QString::fromUtf8("actionRozja_nij"));
        actionWykrywaj_kraw_dzie = new QAction(MainWindow);
        actionWykrywaj_kraw_dzie->setObjectName(QString::fromUtf8("actionWykrywaj_kraw_dzie"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mdiArea = new QMdiArea(centralWidget);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));

        gridLayout->addWidget(mdiArea, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 566, 21));
        menuMenu = new QMenu(menuBar);
        menuMenu->setObjectName(QString::fromUtf8("menuMenu"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuMenu->menuAction());
        menuMenu->addAction(actionOtw_rz_obraz);
        menuMenu->addAction(actionWyostrz);
        menuMenu->addAction(actionRozja_nij);
        menuMenu->addAction(actionWykrywaj);
        menuMenu->addAction(actionWykrywaj_kraw_dzie);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOtw_rz_obraz->setText(QApplication::translate("MainWindow", "Otw\303\263rz obraz", 0, QApplication::UnicodeUTF8));
        actionOtw_rz_obraz->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionWyostrz->setText(QApplication::translate("MainWindow", "Wyostrz", 0, QApplication::UnicodeUTF8));
        actionWykrywaj->setText(QApplication::translate("MainWindow", "Wykrywaj", 0, QApplication::UnicodeUTF8));
        actionRozja_nij->setText(QApplication::translate("MainWindow", "Rozja\305\233nij", 0, QApplication::UnicodeUTF8));
        actionWykrywaj_kraw_dzie->setText(QApplication::translate("MainWindow", "Wykrywaj kraw\304\231dzie", 0, QApplication::UnicodeUTF8));
        menuMenu->setTitle(QApplication::translate("MainWindow", "Menu", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
