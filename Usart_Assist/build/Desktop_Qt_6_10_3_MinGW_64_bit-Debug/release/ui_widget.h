/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *label_PortState;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *Button_Open;
    QPushButton *Button_Close;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_PortName;
    QComboBox *comboBox_PortName;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_Baudrate;
    QComboBox *comboBox_Baudrate;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_Databits;
    QComboBox *comboBox_Databits;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_Stopbits;
    QComboBox *comboBox_Stopbits;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_Paritybits;
    QComboBox *comboBox_ParityBits;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_Flowcontrol;
    QComboBox *comboBox_Flowcontrol;
    QPlainTextEdit *plainTextEdit_ReceiveWindow;
    QPlainTextEdit *plainTextEdit_SendWindow;
    QPushButton *Button_Send;
    QPushButton *Button_SendClear;
    QLabel *label_FileState;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *pushButton_FileOpen;
    QPushButton *pushButton_FileSend;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(800, 598);
        label_PortState = new QLabel(Widget);
        label_PortState->setObjectName("label_PortState");
        label_PortState->setGeometry(QRect(50, 260, 191, 21));
        QFont font;
        font.setPointSize(9);
        label_PortState->setFont(font);
        label_PortState->setScaledContents(false);
        layoutWidget = new QWidget(Widget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(40, 290, 195, 30));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        Button_Open = new QPushButton(layoutWidget);
        Button_Open->setObjectName("Button_Open");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Button_Open->sizePolicy().hasHeightForWidth());
        Button_Open->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(Button_Open);

        Button_Close = new QPushButton(layoutWidget);
        Button_Close->setObjectName("Button_Close");
        sizePolicy.setHeightForWidth(Button_Close->sizePolicy().hasHeightForWidth());
        Button_Close->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(Button_Close);

        layoutWidget1 = new QWidget(Widget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(60, 30, 162, 199));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_PortName = new QLabel(layoutWidget1);
        label_PortName->setObjectName("label_PortName");
        sizePolicy.setHeightForWidth(label_PortName->sizePolicy().hasHeightForWidth());
        label_PortName->setSizePolicy(sizePolicy);
        label_PortName->setMinimumSize(QSize(0, 0));
        QFont font1;
        font1.setPointSize(10);
        label_PortName->setFont(font1);

        horizontalLayout_2->addWidget(label_PortName);

        comboBox_PortName = new QComboBox(layoutWidget1);
        comboBox_PortName->setObjectName("comboBox_PortName");
        comboBox_PortName->setEnabled(true);
        sizePolicy.setHeightForWidth(comboBox_PortName->sizePolicy().hasHeightForWidth());
        comboBox_PortName->setSizePolicy(sizePolicy);
        comboBox_PortName->setMinimumSize(QSize(83, 25));
        comboBox_PortName->setMaximumSize(QSize(83, 25));

        horizontalLayout_2->addWidget(comboBox_PortName);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_Baudrate = new QLabel(layoutWidget1);
        label_Baudrate->setObjectName("label_Baudrate");
        sizePolicy.setHeightForWidth(label_Baudrate->sizePolicy().hasHeightForWidth());
        label_Baudrate->setSizePolicy(sizePolicy);
        label_Baudrate->setFont(font1);

        horizontalLayout_3->addWidget(label_Baudrate);

        comboBox_Baudrate = new QComboBox(layoutWidget1);
        comboBox_Baudrate->addItem(QString());
        comboBox_Baudrate->addItem(QString());
        comboBox_Baudrate->addItem(QString());
        comboBox_Baudrate->addItem(QString());
        comboBox_Baudrate->addItem(QString());
        comboBox_Baudrate->setObjectName("comboBox_Baudrate");
        sizePolicy.setHeightForWidth(comboBox_Baudrate->sizePolicy().hasHeightForWidth());
        comboBox_Baudrate->setSizePolicy(sizePolicy);
        comboBox_Baudrate->setMinimumSize(QSize(83, 25));
        comboBox_Baudrate->setMaximumSize(QSize(83, 25));

        horizontalLayout_3->addWidget(comboBox_Baudrate);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_Databits = new QLabel(layoutWidget1);
        label_Databits->setObjectName("label_Databits");
        sizePolicy.setHeightForWidth(label_Databits->sizePolicy().hasHeightForWidth());
        label_Databits->setSizePolicy(sizePolicy);
        label_Databits->setFont(font1);

        horizontalLayout_4->addWidget(label_Databits);

        comboBox_Databits = new QComboBox(layoutWidget1);
        comboBox_Databits->addItem(QString());
        comboBox_Databits->addItem(QString());
        comboBox_Databits->addItem(QString());
        comboBox_Databits->addItem(QString());
        comboBox_Databits->setObjectName("comboBox_Databits");
        sizePolicy.setHeightForWidth(comboBox_Databits->sizePolicy().hasHeightForWidth());
        comboBox_Databits->setSizePolicy(sizePolicy);
        comboBox_Databits->setMinimumSize(QSize(83, 25));
        comboBox_Databits->setMaximumSize(QSize(83, 25));

        horizontalLayout_4->addWidget(comboBox_Databits);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_Stopbits = new QLabel(layoutWidget1);
        label_Stopbits->setObjectName("label_Stopbits");
        sizePolicy.setHeightForWidth(label_Stopbits->sizePolicy().hasHeightForWidth());
        label_Stopbits->setSizePolicy(sizePolicy);
        label_Stopbits->setFont(font1);

        horizontalLayout_5->addWidget(label_Stopbits);

        comboBox_Stopbits = new QComboBox(layoutWidget1);
        comboBox_Stopbits->addItem(QString());
        comboBox_Stopbits->addItem(QString());
        comboBox_Stopbits->addItem(QString());
        comboBox_Stopbits->setObjectName("comboBox_Stopbits");
        sizePolicy.setHeightForWidth(comboBox_Stopbits->sizePolicy().hasHeightForWidth());
        comboBox_Stopbits->setSizePolicy(sizePolicy);
        comboBox_Stopbits->setMinimumSize(QSize(83, 25));
        comboBox_Stopbits->setMaximumSize(QSize(83, 25));

        horizontalLayout_5->addWidget(comboBox_Stopbits);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_Paritybits = new QLabel(layoutWidget1);
        label_Paritybits->setObjectName("label_Paritybits");
        sizePolicy.setHeightForWidth(label_Paritybits->sizePolicy().hasHeightForWidth());
        label_Paritybits->setSizePolicy(sizePolicy);
        label_Paritybits->setFont(font1);

        horizontalLayout_6->addWidget(label_Paritybits);

        comboBox_ParityBits = new QComboBox(layoutWidget1);
        comboBox_ParityBits->addItem(QString());
        comboBox_ParityBits->addItem(QString());
        comboBox_ParityBits->addItem(QString());
        comboBox_ParityBits->addItem(QString());
        comboBox_ParityBits->addItem(QString());
        comboBox_ParityBits->setObjectName("comboBox_ParityBits");
        sizePolicy.setHeightForWidth(comboBox_ParityBits->sizePolicy().hasHeightForWidth());
        comboBox_ParityBits->setSizePolicy(sizePolicy);
        comboBox_ParityBits->setMinimumSize(QSize(83, 25));
        comboBox_ParityBits->setMaximumSize(QSize(83, 25));

        horizontalLayout_6->addWidget(comboBox_ParityBits);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_Flowcontrol = new QLabel(layoutWidget1);
        label_Flowcontrol->setObjectName("label_Flowcontrol");
        sizePolicy.setHeightForWidth(label_Flowcontrol->sizePolicy().hasHeightForWidth());
        label_Flowcontrol->setSizePolicy(sizePolicy);
        label_Flowcontrol->setFont(font1);

        horizontalLayout_7->addWidget(label_Flowcontrol);

        comboBox_Flowcontrol = new QComboBox(layoutWidget1);
        comboBox_Flowcontrol->addItem(QString());
        comboBox_Flowcontrol->addItem(QString());
        comboBox_Flowcontrol->addItem(QString());
        comboBox_Flowcontrol->setObjectName("comboBox_Flowcontrol");
        sizePolicy.setHeightForWidth(comboBox_Flowcontrol->sizePolicy().hasHeightForWidth());
        comboBox_Flowcontrol->setSizePolicy(sizePolicy);
        comboBox_Flowcontrol->setMinimumSize(QSize(83, 25));
        comboBox_Flowcontrol->setMaximumSize(QSize(83, 25));

        horizontalLayout_7->addWidget(comboBox_Flowcontrol);


        verticalLayout->addLayout(horizontalLayout_7);

        plainTextEdit_ReceiveWindow = new QPlainTextEdit(Widget);
        plainTextEdit_ReceiveWindow->setObjectName("plainTextEdit_ReceiveWindow");
        plainTextEdit_ReceiveWindow->setGeometry(QRect(280, 20, 470, 300));
        plainTextEdit_SendWindow = new QPlainTextEdit(Widget);
        plainTextEdit_SendWindow->setObjectName("plainTextEdit_SendWindow");
        plainTextEdit_SendWindow->setGeometry(QRect(280, 349, 470, 140));
        Button_Send = new QPushButton(Widget);
        Button_Send->setObjectName("Button_Send");
        Button_Send->setGeometry(QRect(650, 500, 93, 28));
        Button_SendClear = new QPushButton(Widget);
        Button_SendClear->setObjectName("Button_SendClear");
        Button_SendClear->setGeometry(QRect(540, 500, 93, 28));
        label_FileState = new QLabel(Widget);
        label_FileState->setObjectName("label_FileState");
        label_FileState->setGeometry(QRect(50, 340, 201, 19));
        layoutWidget2 = new QWidget(Widget);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(40, 370, 195, 30));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        pushButton_FileOpen = new QPushButton(layoutWidget2);
        pushButton_FileOpen->setObjectName("pushButton_FileOpen");

        horizontalLayout_8->addWidget(pushButton_FileOpen);

        pushButton_FileSend = new QPushButton(layoutWidget2);
        pushButton_FileSend->setObjectName("pushButton_FileSend");

        horizontalLayout_8->addWidget(pushButton_FileSend);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label_PortState->setText(QCoreApplication::translate("Widget", "TextLabel", nullptr));
        Button_Open->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200", nullptr));
        Button_Close->setText(QCoreApplication::translate("Widget", "\345\205\263\351\227\255", nullptr));
        label_PortName->setText(QCoreApplication::translate("Widget", "\344\270\262\345\217\243\345\217\267\357\274\232", nullptr));
        label_Baudrate->setText(QCoreApplication::translate("Widget", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        comboBox_Baudrate->setItemText(0, QCoreApplication::translate("Widget", "4800", nullptr));
        comboBox_Baudrate->setItemText(1, QCoreApplication::translate("Widget", "9600", nullptr));
        comboBox_Baudrate->setItemText(2, QCoreApplication::translate("Widget", "14400", nullptr));
        comboBox_Baudrate->setItemText(3, QCoreApplication::translate("Widget", "19200", nullptr));
        comboBox_Baudrate->setItemText(4, QCoreApplication::translate("Widget", "115200", nullptr));

        label_Databits->setText(QCoreApplication::translate("Widget", "\346\225\260\346\215\256\344\275\215\357\274\232", nullptr));
        comboBox_Databits->setItemText(0, QCoreApplication::translate("Widget", "5", nullptr));
        comboBox_Databits->setItemText(1, QCoreApplication::translate("Widget", "6", nullptr));
        comboBox_Databits->setItemText(2, QCoreApplication::translate("Widget", "7", nullptr));
        comboBox_Databits->setItemText(3, QCoreApplication::translate("Widget", "8", nullptr));

        label_Stopbits->setText(QCoreApplication::translate("Widget", "\345\201\234\346\255\242\344\275\215\357\274\232", nullptr));
        comboBox_Stopbits->setItemText(0, QCoreApplication::translate("Widget", "1", nullptr));
        comboBox_Stopbits->setItemText(1, QCoreApplication::translate("Widget", "1.5", nullptr));
        comboBox_Stopbits->setItemText(2, QCoreApplication::translate("Widget", "2", nullptr));

        label_Paritybits->setText(QCoreApplication::translate("Widget", "\346\240\241\351\252\214\344\275\215\357\274\232", nullptr));
        comboBox_ParityBits->setItemText(0, QCoreApplication::translate("Widget", "NONE", nullptr));
        comboBox_ParityBits->setItemText(1, QCoreApplication::translate("Widget", "EVEN", nullptr));
        comboBox_ParityBits->setItemText(2, QCoreApplication::translate("Widget", "ODD", nullptr));
        comboBox_ParityBits->setItemText(3, QCoreApplication::translate("Widget", "SPACE", nullptr));
        comboBox_ParityBits->setItemText(4, QCoreApplication::translate("Widget", "MARK", nullptr));

        label_Flowcontrol->setText(QCoreApplication::translate("Widget", "\346\265\201\346\216\247\345\210\266\357\274\232", nullptr));
        comboBox_Flowcontrol->setItemText(0, QCoreApplication::translate("Widget", "NONE", nullptr));
        comboBox_Flowcontrol->setItemText(1, QCoreApplication::translate("Widget", "RTS/CTS", nullptr));
        comboBox_Flowcontrol->setItemText(2, QCoreApplication::translate("Widget", "XON/XOFF", nullptr));

        Button_Send->setText(QCoreApplication::translate("Widget", "\345\217\221\351\200\201", nullptr));
        Button_SendClear->setText(QCoreApplication::translate("Widget", "\346\270\205\351\231\244", nullptr));
        label_FileState->setText(QCoreApplication::translate("Widget", "TextLabel", nullptr));
        pushButton_FileOpen->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
        pushButton_FileSend->setText(QCoreApplication::translate("Widget", "\345\217\221\351\200\201\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
