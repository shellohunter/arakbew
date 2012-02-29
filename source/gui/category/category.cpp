#include "category.h"
#include "../background/Background.h"
#include "../login/Login.h"


MBtnCategory::MBtnCategory(const QString & text, ECategoryType type, Category * parent)
                        :QPushButton(text, parent->root)
{
    LOG_API();

    QObject::connect(this, SIGNAL(signalMouseEvent(MBtnCategory *)), parent, SLOT(slotCategorySelected(MBtnCategory *)));
    this->setStyleSheet("color:red; border:1px solid #AAAAAA; background-color: white;");
    this->setFixedSize(135,128);
    this->setFocusPolicy(Qt::NoFocus);
    this->type = type;
}


void MBtnCategory::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit signalMouseEvent(this);
    }
}


void MBtnCategory::highlight(bool hilight)
{
    if(hilight)
    {
        QString normalCSS = this->styleSheet();
        QString focusedCSS = normalCSS.replace("solid #AAAAAA","solid red");
        focusedCSS = normalCSS.replace("background-color: whitesmoke;","background-color: lightgray;");
        LOG_VERBOSE("<category> %s, %s\n", qPrintable(normalCSS), qPrintable(focusedCSS));
        this->setStyleSheet(focusedCSS);
    }
    else
    {
        QString focusedCSS = this->styleSheet();
        QString normalCSS = focusedCSS.replace("solid red", "solid #AAAAAA");
        normalCSS = focusedCSS.replace("background-color: lightgray;","background-color: whitesmoke;");
        LOG_VERBOSE("<category> %s, %s\n", qPrintable(focusedCSS), qPrintable(normalCSS));
        this->setStyleSheet(normalCSS);
    }

}


Category::Category(QWidget * parent)
    : GuiModule(GUI_MODULE_CATEGORY), parentWidget(parent)
{
    LOG_API();

    init();

    return;
}


Category::~Category()
{
    LOG_API();

    exit();

    return;
}



int Category::init()
{
    LOG_API();

    //QPalette palette = this->palette();
    //palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/dialog_background.png")));
    //this->setPalette(palette);

    root = new QWidget(parentWidget);
    root->setAutoFillBackground(true);
    root->setGeometry(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

    focusAgent = new QPushButton(root);
    focusAgent->setGeometry(0,0,0,0);
    focusAgent->setFocus();
    focusAgent->installEventFilter(this);

    btn_cates[0]    = new MBtnCategory(tr("Male"), E_category_singer_male, this);
    btn_cates[1]    = new MBtnCategory(tr("Female"), E_category_singer_female, this);
    btn_cates[2]    = new MBtnCategory(tr("Band"), E_category_singer_band, this);
    btn_cates[3]    = new MBtnCategory(tr("Pingying Song"), E_category_song_pingying, this);
    btn_cates[4]    = new MBtnCategory(tr("Pingying Singer"), E_category_singer_pingying, this);
    btn_cates[5]    = new MBtnCategory(tr("My Favorite"), E_category_song_favorite, this);

    QGridLayout  *layoutGrid = new QGridLayout();
    for(int i=0; i<6; i++)
        layoutGrid->addWidget(btn_cates[i], i/3, i%3);


    btn_return      = new QPushButton(tr("Return"), this->root);
    btn_return->setGeometry(SCREEN_WIDTH-210, SCREEN_HEIGHT-30, 200, 20);

#if 0

    layoutGrid->addWidget(btn_male, 0,0);
    layoutGrid->addWidget(btn_female, 0,1);
    layoutGrid->addWidget(btn_band, 0,2);
    layoutGrid->addWidget(btn_band_male, 1,0);
    layoutGrid->addWidget(btn_band_female, 1,1);
    layoutGrid->addWidget(btn_return, 1,2);
    this->initNavigation();
#endif

    //this->addLayout(layoutGrid);
    root->setLayout(layoutGrid);
    root->setFocusPolicy(Qt::NoFocus);
    this->moveFocus(btn_cates[0]);

    connect(btn_return, SIGNAL(clicked()), this, SLOT(slotReturnButton()));

    root->hide();

    return OK;
}


int Category::resume()
{
    LOG_API();
    focusAgent->setFocus();
    root->show();
    return OK;
}


int Category::pause()
{
    LOG_API();
    root->hide();
    return OK;
}


int Category::exit()
{
    LOG_API();
    DELETE(root);
    return OK;
}


int Category::processMessage(int msg, void * data)
{
    LOG_API();
    return OK;
}




void Category::slotCategorySelected(MBtnCategory * button)
{
    LOG_VERBOSE("<category> %s(%s)", __FUNCTION__,qPrintable(button->text()));

    GuiEvent event;
    event.type = GUI_EVENT_RESUME_PAGE;
    switch(button->type)
    {
        case E_category_singer_male:
        case E_category_singer_female:
        case E_category_singer_band:
        case E_category_singer_pingying:
            event.data.moduleName = (char *)GUI_MODULE_SINGERLIST;
            break;
        case E_category_song_favorite:
        case E_category_song_pingying:
            event.data.moduleName = (char *)GUI_MODULE_SEARCHRESULT;
            break;
    }

    sendEvent(&event);
}


void Category::slotReturnButton()
{
    LOG_API();
    GuiEvent event;
    event.type = GUI_EVENT_PAUSE_PAGE;
    sendEvent(&event);
}


void Category::moveFocus(MBtnCategory * btn)
{
    if(!btn) return;

    if(this->focus_btn) this->focus_btn->highlight(false);

    btn->highlight(true);
    this->focus_btn = btn;
}




int Category::keyPressEvent(QObject * obj, QKeyEvent * event)
{
    static int cur_idx = 0;
    int        new_idx = cur_idx;
    switch(event->key())
    {

        case Qt::Key_Select:
            LOG_INFO("<category> (Key_Select)\n");
            slotCategorySelected(btn_cates[cur_idx]);
            return OK;
        case Qt::Key_Return:
            LOG_INFO("<category> (Key_Return)\n");
            slotCategorySelected(btn_cates[cur_idx]);
            return OK;
        case Qt::Key_Back:
            LOG_INFO("<category> (Key_Back)\n");
            slotReturnButton();
            return OK;
        case Qt::Key_Escape:
            LOG_INFO("<category> (Key_Escape)\n");
            slotReturnButton();
            return OK;
        case Qt::Key_Up:
            new_idx = abs(cur_idx-3)%6;
            break;
        case Qt::Key_Down:
            new_idx = (cur_idx+3)%6;
            break;
        case Qt::Key_Left:
            new_idx = abs(cur_idx-1)%6;
            break;
        case Qt::Key_Right:
            new_idx = (cur_idx+1)%6;
            break;
        default:
            break;
    }

    LOG_VERBOSE("new hlt idx %d.\n", new_idx);

    btn_cates[cur_idx]->highlight(false);
    btn_cates[new_idx]->highlight(true);
    cur_idx = new_idx;
    return OK;
}



bool Category::eventFilter(QObject * obj, QEvent * event)
{
    if((event->type() == QEvent::KeyPress))
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        LOG_VERBOSE("<category> Key %d Pressed!\n", keyEvent->key());
        switch(keyEvent->key())
        {
            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_Left:
            case Qt::Key_Right:
            case Qt::Key_Select:
            case Qt::Key_Return:
            case Qt::Key_Back:
            case Qt::Key_Escape:
                keyPressEvent(obj, keyEvent);
                event->accept();
                return true;
            default:
                break;
        }
    }

    return false;
}


#if 0

void Category::mouseClick(MBtnCategory * button)
{
    LOG_VERBOSE("<category> dbg trace line %d, %s. %s",__LINE__, __FUNCTION__,qPrintable(button->text()));
    moveFocus(button);
}


void Category::initNavigation()
{
    static Navigation _navi_[] =
    {
        {this->btn_male, NULL, this->btn_female},
        {this->btn_female, this->btn_male, this->btn_band},
        {this->btn_band, this->btn_female, this->btn_band_male},
        {this->btn_band_male, this->btn_band, this->btn_band_female},
        {this->btn_band_female, this->btn_band_male, this->btn_return},
        {this->btn_return, this->btn_band_female, NULL},
        {NULL, NULL, NULL},
    };

    this->navi = _navi_;
}

MBtnCategory * Category::execNavigation(QKeyEvent * keyEvent)
{
    int i = 0;

    for(i=0;this->navi[i].focus != NULL;i++)
    {
        if(this->focus_btn == this->navi[i].focus)
        {
            switch(keyEvent->key())
            {
                case Qt::Key_Left:
                    LOG_VERBOSE("<category> Qt::Key_Left , %s -> %s.\n" , qPrintable(this->navi[i].focus->text()), this->navi[i].btn_left?qPrintable(this->navi[i].btn_left->text()):"None");
                    if (this->navi[i].btn_left != NULL)
                    {
                        #if 1
                        moveFocus(this->navi[i].btn_left);
                        #else
                        this->navi[i].btn_left->getFocus();
                        this->navi[i].focus->loseFocus();
                        this->focus_btn = this->navi[i].btn_left;
                        #endif
                        return this->navi[i].btn_left;
                    }
                    else
                    {
                        this->focus_btn = this->navi[i].focus;
                        return this->navi[i].focus;
                    }


                case Qt::Key_Right:
                    LOG_VERBOSE("<category> Qt::Key_Right , %s -> %s.\n" , qPrintable(this->navi[i].focus->text()), this->navi[i].btn_right?qPrintable(this->navi[i].btn_right->text()):"None");
                    if (this->navi[i].btn_right != NULL)
                    {
                        #if 1
                        moveFocus(this->navi[i].btn_right);
                        #else
                        this->navi[i].btn_right->getFocus();
                        this->navi[i].focus->loseFocus();
                        this->focus_btn = this->navi[i].btn_right;
                        #endif
                        return this->navi[i].btn_right;
                    }
                    else
                    {
                        this->focus_btn = this->navi[i].focus;
                        return this->navi[i].focus;
                    }

                default:
                    return NULL;
            }
        }
    }
    return NULL;
}

#endif
