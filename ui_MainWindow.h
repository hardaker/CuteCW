#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *modeMenu;
    QPushButton *play;
    QPushButton *changeSequence;
    QPushButton *changeWords;
    QPushButton *clearTraining;
    QPushButton *prefs;
    QLabel *helpBar;
    QHBoxLayout *horizontalLayout_5;
    QLabel *letter;
    QLabel *WPM;
    QHBoxLayout *forModes;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLabel *sequence;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLabel *lastwpm;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QLabel *avewpm;
    QSpacerItem *verticalSpacer;
    QPushButton *homeButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 300);
        MainWindow->setMinimumSize(QSize(0, 300));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 781, 281));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        modeMenu = new QPushButton(verticalLayoutWidget);
        modeMenu->setObjectName(QString::fromUtf8("modeMenu"));

        horizontalLayout->addWidget(modeMenu);

        play = new QPushButton(verticalLayoutWidget);
        play->setObjectName(QString::fromUtf8("play"));

        horizontalLayout->addWidget(play);

        changeSequence = new QPushButton(verticalLayoutWidget);
        changeSequence->setObjectName(QString::fromUtf8("changeSequence"));

        horizontalLayout->addWidget(changeSequence);

        changeWords = new QPushButton(verticalLayoutWidget);
        changeWords->setObjectName(QString::fromUtf8("changeWords"));

        horizontalLayout->addWidget(changeWords);

        clearTraining = new QPushButton(verticalLayoutWidget);
        clearTraining->setObjectName(QString::fromUtf8("clearTraining"));

        horizontalLayout->addWidget(clearTraining);

        prefs = new QPushButton(verticalLayoutWidget);
        prefs->setObjectName(QString::fromUtf8("prefs"));

        horizontalLayout->addWidget(prefs);


        verticalLayout->addLayout(horizontalLayout);

        helpBar = new QLabel(verticalLayoutWidget);
        helpBar->setObjectName(QString::fromUtf8("helpBar"));
        QFont font;
        font.setPointSize(14);
        font.setItalic(false);
        helpBar->setFont(font);
        helpBar->setWordWrap(true);

        verticalLayout->addWidget(helpBar);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        letter = new QLabel(verticalLayoutWidget);
        letter->setObjectName(QString::fromUtf8("letter"));
        QFont font1;
        font1.setPointSize(24);
        font1.setBold(true);
        font1.setWeight(75);
        letter->setFont(font1);

        horizontalLayout_5->addWidget(letter);

        WPM = new QLabel(verticalLayoutWidget);
        WPM->setObjectName(QString::fromUtf8("WPM"));
        WPM->setFont(font1);

        horizontalLayout_5->addWidget(WPM);


        verticalLayout->addLayout(horizontalLayout_5);

        forModes = new QHBoxLayout();
        forModes->setSpacing(6);
        forModes->setObjectName(QString::fromUtf8("forModes"));
        forModes->setSizeConstraint(QLayout::SetNoConstraint);

        verticalLayout->addLayout(forModes);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(200, 0));

        horizontalLayout_3->addWidget(label);

        sequence = new QLabel(verticalLayoutWidget);
        sequence->setObjectName(QString::fromUtf8("sequence"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sequence->sizePolicy().hasHeightForWidth());
        sequence->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(sequence);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(200, 0));

        horizontalLayout_4->addWidget(label_3);

        lastwpm = new QLabel(verticalLayoutWidget);
        lastwpm->setObjectName(QString::fromUtf8("lastwpm"));
        sizePolicy1.setHeightForWidth(lastwpm->sizePolicy().hasHeightForWidth());
        lastwpm->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(lastwpm);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(200, 0));

        horizontalLayout_6->addWidget(label_2);

        avewpm = new QLabel(verticalLayoutWidget);
        avewpm->setObjectName(QString::fromUtf8("avewpm"));
        sizePolicy1.setHeightForWidth(avewpm->sizePolicy().hasHeightForWidth());
        avewpm->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(avewpm);


        verticalLayout->addLayout(horizontalLayout_6);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Cute CW", 0, QApplication::UnicodeUTF8));
        modeMenu->setText(QApplication::translate("MainWindow", "Mode", 0, QApplication::UnicodeUTF8));
        play->setText(QApplication::translate("MainWindow", "Play", 0, QApplication::UnicodeUTF8));
        changeSequence->setText(QApplication::translate("MainWindow", "Sequence", 0, QApplication::UnicodeUTF8));
        changeWords->setText(QApplication::translate("MainWindow", "Change Words", 0, QApplication::UnicodeUTF8));
        clearTraining->setText(QApplication::translate("MainWindow", "Clear Training", 0, QApplication::UnicodeUTF8));
        prefs->setText(QApplication::translate("MainWindow", "Preferences", 0, QApplication::UnicodeUTF8));
        helpBar->setText(QString());
        letter->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        WPM->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Sequence:", 0, QApplication::UnicodeUTF8));
        sequence->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "Last WPM:", 0, QApplication::UnicodeUTF8));
        lastwpm->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "Average WPM:", 0, QApplication::UnicodeUTF8));
        avewpm->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
