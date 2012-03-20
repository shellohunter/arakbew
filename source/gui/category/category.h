#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include <QtGui>
#include <QtCore>
#include "../GuiManager.h"
#include "../widget/button.h"

typedef enum
{
    E_category_singer_male,
    E_category_singer_female,
    E_category_singer_band,
    E_category_singer_pingying,
    E_category_song_pingying,
    E_category_song_favorite,

}ECategoryType;

class Category;

class MBtnCategory : public QPushButton
{
Q_OBJECT

public:
    MBtnCategory(const QString & text, ECategoryType type, Category * parent = 0);
    void mousePressEvent(QMouseEvent *event);
    void highlight(bool hilight);
    ECategoryType type;
signals:
    void signalMouseEvent(MBtnCategory * button);
};


class Category : public GuiModule
{
Q_OBJECT

public:
    Category(QWidget * parent = NULL);
    ~Category();

    int resume();
    int pause();
    int init();
    int exit();
    int processMessage(int msg, void * data);

    typedef struct
    {
        Button * focus;
        Button * btn_left;
        Button * btn_right;
    }Navigation;

    MBtnCategory * btn_cates[6];

#if 0
    MBtnCategory * btn_male;
    MBtnCategory * btn_female;
    MBtnCategory * btn_band_male;
    MBtnCategory * btn_band_female;
    MBtnCategory * btn_band;
#endif

    Button * btn_return;
    QWidget * root;

protected:
    int keyPressEvent(QObject * obj, QKeyEvent * event);
    bool eventFilter(QObject * obj, QEvent * event);
    void moveFocus(MBtnCategory * btn);
#if 0
    void initNavigation();
    MBtnCategory * execNavigation(QKeyEvent * keyEvent);
    void moveFocus(MBtnCategory * btn);
#endif

private:

    Navigation * navi;
    MBtnCategory * focus_btn;
    QWidget * parentWidget;
    Button * focusAgent;

public slots:
#if 0
    void mouseClick(MBtnCategory * button);
#endif
    void slotReturnButton();
    void slotCategorySelected(MBtnCategory * button);
};


#endif // CATEGORY_HPP
