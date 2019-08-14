/********************************************************************************
** Form generated from reading UI file 'Registration.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTRATION_H
#define UI_REGISTRATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegistrationClass
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox_import;
    QPushButton *pushButton_import;
    QLabel *label_accountOfClouds;
    QGroupBox *groupBox_setting;
    QSpinBox *spinBox_initialIterations;
    QLabel *label_InitialITR;
    QLabel *label_NextITR;
    QSpinBox *spinBox_nextIteration;
    QComboBox *comboBox_fixedCloud;
    QLabel *label_fixedCloud;
    QLabel *label_movingCloud;
    QComboBox *comboBox_movingCloud;
    QPushButton *pushButton_ICPStart;
    QPushButton *pushButton_nextITR;
    QGroupBox *groupBox_result;
    QLabel *label_R;
    QLabel *label_T;
    QTextBrowser *textBrowser_R;
    QTextBrowser *textBrowser_T;
    QLabel *label_fitnessScore;
    QCheckBox *checkBox_3D;
    QLabel *label_time;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RegistrationClass)
    {
        if (RegistrationClass->objectName().isEmpty())
            RegistrationClass->setObjectName(QString::fromUtf8("RegistrationClass"));
        RegistrationClass->resize(354, 714);
        centralWidget = new QWidget(RegistrationClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBox_import = new QGroupBox(centralWidget);
        groupBox_import->setObjectName(QString::fromUtf8("groupBox_import"));
        groupBox_import->setGeometry(QRect(30, 20, 291, 71));
        pushButton_import = new QPushButton(groupBox_import);
        pushButton_import->setObjectName(QString::fromUtf8("pushButton_import"));
        pushButton_import->setGeometry(QRect(40, 30, 75, 23));
        label_accountOfClouds = new QLabel(groupBox_import);
        label_accountOfClouds->setObjectName(QString::fromUtf8("label_accountOfClouds"));
        label_accountOfClouds->setGeometry(QRect(170, 30, 71, 20));
        groupBox_setting = new QGroupBox(centralWidget);
        groupBox_setting->setObjectName(QString::fromUtf8("groupBox_setting"));
        groupBox_setting->setGeometry(QRect(30, 120, 291, 271));
        spinBox_initialIterations = new QSpinBox(groupBox_setting);
        spinBox_initialIterations->setObjectName(QString::fromUtf8("spinBox_initialIterations"));
        spinBox_initialIterations->setGeometry(QRect(190, 130, 71, 22));
        spinBox_initialIterations->setValue(1);
        label_InitialITR = new QLabel(groupBox_setting);
        label_InitialITR->setObjectName(QString::fromUtf8("label_InitialITR"));
        label_InitialITR->setGeometry(QRect(20, 130, 151, 20));
        label_NextITR = new QLabel(groupBox_setting);
        label_NextITR->setObjectName(QString::fromUtf8("label_NextITR"));
        label_NextITR->setGeometry(QRect(20, 180, 141, 16));
        spinBox_nextIteration = new QSpinBox(groupBox_setting);
        spinBox_nextIteration->setObjectName(QString::fromUtf8("spinBox_nextIteration"));
        spinBox_nextIteration->setGeometry(QRect(190, 180, 71, 22));
        spinBox_nextIteration->setValue(1);
        comboBox_fixedCloud = new QComboBox(groupBox_setting);
        comboBox_fixedCloud->setObjectName(QString::fromUtf8("comboBox_fixedCloud"));
        comboBox_fixedCloud->setGeometry(QRect(148, 25, 111, 22));
        label_fixedCloud = new QLabel(groupBox_setting);
        label_fixedCloud->setObjectName(QString::fromUtf8("label_fixedCloud"));
        label_fixedCloud->setGeometry(QRect(20, 30, 102, 12));
        label_movingCloud = new QLabel(groupBox_setting);
        label_movingCloud->setObjectName(QString::fromUtf8("label_movingCloud"));
        label_movingCloud->setGeometry(QRect(20, 80, 121, 16));
        comboBox_movingCloud = new QComboBox(groupBox_setting);
        comboBox_movingCloud->setObjectName(QString::fromUtf8("comboBox_movingCloud"));
        comboBox_movingCloud->setGeometry(QRect(150, 75, 111, 22));
        pushButton_ICPStart = new QPushButton(groupBox_setting);
        pushButton_ICPStart->setObjectName(QString::fromUtf8("pushButton_ICPStart"));
        pushButton_ICPStart->setGeometry(QRect(40, 230, 75, 23));
        pushButton_nextITR = new QPushButton(groupBox_setting);
        pushButton_nextITR->setObjectName(QString::fromUtf8("pushButton_nextITR"));
        pushButton_nextITR->setGeometry(QRect(170, 230, 75, 23));
        groupBox_result = new QGroupBox(centralWidget);
        groupBox_result->setObjectName(QString::fromUtf8("groupBox_result"));
        groupBox_result->setGeometry(QRect(30, 420, 291, 231));
        label_R = new QLabel(groupBox_result);
        label_R->setObjectName(QString::fromUtf8("label_R"));
        label_R->setGeometry(QRect(10, 30, 91, 16));
        label_T = new QLabel(groupBox_result);
        label_T->setObjectName(QString::fromUtf8("label_T"));
        label_T->setGeometry(QRect(10, 110, 111, 16));
        textBrowser_R = new QTextBrowser(groupBox_result);
        textBrowser_R->setObjectName(QString::fromUtf8("textBrowser_R"));
        textBrowser_R->setGeometry(QRect(10, 50, 261, 51));
        textBrowser_T = new QTextBrowser(groupBox_result);
        textBrowser_T->setObjectName(QString::fromUtf8("textBrowser_T"));
        textBrowser_T->setGeometry(QRect(10, 130, 261, 21));
        label_fitnessScore = new QLabel(groupBox_result);
        label_fitnessScore->setObjectName(QString::fromUtf8("label_fitnessScore"));
        label_fitnessScore->setGeometry(QRect(10, 160, 261, 16));
        checkBox_3D = new QCheckBox(groupBox_result);
        checkBox_3D->setObjectName(QString::fromUtf8("checkBox_3D"));
        checkBox_3D->setGeometry(QRect(10, 200, 81, 16));
        label_time = new QLabel(groupBox_result);
        label_time->setObjectName(QString::fromUtf8("label_time"));
        label_time->setGeometry(QRect(10, 180, 261, 16));
        RegistrationClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RegistrationClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 354, 23));
        RegistrationClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RegistrationClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        RegistrationClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RegistrationClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        RegistrationClass->setStatusBar(statusBar);

        retranslateUi(RegistrationClass);

        QMetaObject::connectSlotsByName(RegistrationClass);
    } // setupUi

    void retranslateUi(QMainWindow *RegistrationClass)
    {
        RegistrationClass->setWindowTitle(QApplication::translate("RegistrationClass", "Registration", nullptr));
        groupBox_import->setTitle(QApplication::translate("RegistrationClass", "Import", nullptr));
        pushButton_import->setText(QApplication::translate("RegistrationClass", "Import", nullptr));
        label_accountOfClouds->setText(QApplication::translate("RegistrationClass", "amount:0", nullptr));
        groupBox_setting->setTitle(QApplication::translate("RegistrationClass", "ICP Settings", nullptr));
        label_InitialITR->setText(QApplication::translate("RegistrationClass", "initial iteration times", nullptr));
        label_NextITR->setText(QApplication::translate("RegistrationClass", "next iteration times", nullptr));
        label_fixedCloud->setText(QApplication::translate("RegistrationClass", "fixed cloud model", nullptr));
        label_movingCloud->setText(QApplication::translate("RegistrationClass", "moving cloud model", nullptr));
        pushButton_ICPStart->setText(QApplication::translate("RegistrationClass", "ICP Start", nullptr));
        pushButton_nextITR->setText(QApplication::translate("RegistrationClass", "Next ITR", nullptr));
        groupBox_result->setTitle(QApplication::translate("RegistrationClass", "Result", nullptr));
        label_R->setText(QApplication::translate("RegistrationClass", "Rotation matrix", nullptr));
        label_T->setText(QApplication::translate("RegistrationClass", "Translation vector", nullptr));
        textBrowser_R->setHtml(QApplication::translate("RegistrationClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">  | 1 0 0 |</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">R=| 0 1 0 |</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">  | 0 0 1 |</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-le"
                        "ft:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p></body></html>", nullptr));
        textBrowser_T->setHtml(QApplication::translate("RegistrationClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">T = | 0 0 0 |</p></body></html>", nullptr));
        label_fitnessScore->setText(QApplication::translate("RegistrationClass", "Fitness score :", nullptr));
        checkBox_3D->setText(QApplication::translate("RegistrationClass", "3D Result", nullptr));
        label_time->setText(QApplication::translate("RegistrationClass", "Last iterration time :", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegistrationClass: public Ui_RegistrationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTRATION_H
