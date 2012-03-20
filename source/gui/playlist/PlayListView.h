
#ifndef PLAYLISTVIEW_HPP
#define PLAYLISTVIEW_HPP

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "shared.h"
#include "../GuiManager.h"
#include "database.h"
#include "../widget/button.h"

class Ui
{
public:
    QWidget *widget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QListWidget *listWidget;
    Button *pushButton;
    Button *pushButton_2;
    Button *pushButton_3;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_22;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(821, 468);
        widget = new QWidget(Form);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(70, 50, 561, 331));
        widget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 80, 141, 21));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(260, 80, 161, 21));
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));
        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(60, 80, 46, 21));
        label_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 127);"));
        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(110, 110, 141, 21));
        label_4->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));
        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(60, 110, 46, 21));
        label_5->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 127);"));
        label_6 = new QLabel(widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(260, 110, 161, 21));
        label_6->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));
        label_7 = new QLabel(widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(260, 140, 161, 21));
        label_7->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));
        label_8 = new QLabel(widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(110, 140, 141, 21));
        label_8->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));
        label_9 = new QLabel(widget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(60, 140, 46, 21));
        label_9->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 127);"));
        label_10 = new QLabel(widget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(110, 170, 141, 21));
        label_10->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));
        label_11 = new QLabel(widget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(260, 170, 161, 21));
        label_11->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));
        label_12 = new QLabel(widget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(60, 170, 46, 21));
        label_12->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 127);"));
        label_13 = new QLabel(widget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(260, 200, 161, 21));
        label_13->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));
        label_14 = new QLabel(widget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(110, 200, 141, 21));
        label_14->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));
        label_15 = new QLabel(widget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(60, 200, 46, 21));
        label_15->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 127);"));
        label_16 = new QLabel(widget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(110, 230, 141, 21));
        label_16->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));
        label_17 = new QLabel(widget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(260, 230, 161, 21));
        label_17->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));
        label_18 = new QLabel(widget);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(60, 230, 46, 21));
        label_18->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 127);"));
        listWidget = new QListWidget(widget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(430, 80, 81, 71));
        listWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 127);"));
        pushButton = new Button(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(70, 290, 75, 23));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);"));
        pushButton_2 = new Button(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(170, 290, 75, 23));
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);"));
        pushButton_3 = new Button(widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(450, 290, 75, 23));
        pushButton_3->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);"));
        label_19 = new QLabel(widget);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(60, 40, 221, 31));
        label_19->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
                                                  "font: 75 18pt Arial;\n"
                                                  "color: rgb(255, 255, 255);"));
        label_20 = new QLabel(widget);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(260, 260, 161, 21));
        label_20->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));
        label_21 = new QLabel(widget);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(60, 260, 46, 21));
        label_21->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 127);"));
        label_22 = new QLabel(widget);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(110, 260, 141, 21));
        label_22->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("Form", "\347\275\256\351\241\266", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("Form", "\346\217\220\345\211\215", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("Form", "\347\247\273\345\220\216", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem3 = listWidget->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("Form", "\347\275\256\345\272\225", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem4 = listWidget->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("Form", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        listWidget->setSortingEnabled(__sortingEnabled);

        pushButton->setText(QApplication::translate("Form", "\344\270\212\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Form", "\344\270\213\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("Form", "\350\277\224\345\233\236", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("Form", "\345\267\262\351\200\211\346\255\214\346\233\262", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};



class PlayListItem : public QFrame
{
Q_OBJECT

public:
    PlayListItem(QWidget *parent = NULL) {};
    void setContact(QString & songName, QString & singerName){};
    void focusInEvent(QFocusEvent *event){};
    void focusOutEvent(QFocusEvent *event){};
    virtual ~PlayListItem(){};

private:
    QLabel * songName;
    QLabel * singerName;
    QLabel * status;

};



class PlayListView : public GuiModule
{
Q_OBJECT
public:
    PlayListView(QWidget * parent = NULL);
    ~PlayListView();

    int resume();
    int pause();
    int init();
    int exit();
    int processMessage(int msg, void * data);


    void ScrollDown(){};
    void ScrollUp(){};
    void getCurrentItem() {};


protected:
    bool eventFilter(QObject * obj, QEvent * event);


private:
    Ui * ui;
    QWidget * root;
    QWidget * parentWidget;

protected slots:
    void slotReturnButton();
    void slotContextMenu();

    void slotSongAdd(const Song &song);
    void slotSongDel(const Song &song);

};


#endif // PLAYLISTVIEW_HPP
