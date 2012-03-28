

#include<QtGui>
#include<QtCore>
#include"SongListView.h"
#include"shared.h"
#include"log.h"



SongListView::SongListView(DataSet<Song*>& songs, QWidget * parent)
    :QTableView(parent)
{
    LOG_API();
    delegate=new SongListItemDelegate();
    model=new SongListStandardItemModel(songs);

    this->setModel(model);
    this->setItemDelegate(delegate);
    this->setFrameShape(QFrame::NoFrame);
    this->resizeColumnsToContents();
    this->setFocusPolicy(Qt::NoFocus);
    QPalette pll = this->palette();
    //pll.setBrush(QPalette::Base, QBrush(QColor(125,255,255,0)));
    this->setPalette(pll);
    //this->setShowGrid(false);

    //this->resizeRowsToContents();
    this->verticalHeader()->setDefaultSectionSize(32);
    //this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setMouseTracking(true);//important
    installEventFilter(this);

    connect(this, SIGNAL(signalSongAdd(Song)), model, SLOT(slotSongAdd(Song)));
    connect(this, SIGNAL(signalSongDel(Song)), model, SLOT(slotSongDel(Song)));
    //connect(delegate, SIGNAL(signalSongSelected(QString, bool)), model, SLOT(slotSongSelected(QString, bool)));

}



SongListView::~SongListView()
{
    DELETE(delegate);
    DELETE(model);
}


void SongListView::keyPressEvent (QKeyEvent * keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_Select:
        case Qt::Key_Enter:
        case Qt::Key_Return:
        {
            QModelIndex newidx = currentIndex().sibling(currentIndex().row(), 3);
            QVariant var=model->data(newidx,Qt::CheckStateRole);
            bool selected=var.toBool();
            if(var.isValid())
                selected=selected?false:true;
            else
                selected=true;
            model->setData(newidx,selected,Qt::CheckStateRole);
            Song * song = model->songs->at(model->data(newidx.sibling(newidx.row(),3)).toInt());
            //emit signalSongSelected(model->data(newidx.sibling(newidx.row(),0)).toString(), selected);
            if(selected)
            {
                emit signalSongAdd(*song);
            }
            else
            {
                emit signalSongDel(*song);
            }
            break;
        }

        case Qt::Key_Down:
            LOG_INFO("Key_Down row %d\n",currentIndex().row());
            if(model->rowCount() == this->currentIndex().row()+1)
            {
                emit signalFocusOut();
            }
            break;

        /* for IR only */
        case Qt::Key_MediaPrevious: // page up
            this->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
            break;
        case Qt::Key_MediaNext:     // page down
            this->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
            break;
        default:
            break;
    }

    return QTableView::keyPressEvent(keyEvent);
}


void SongListView::mouseMoveEvent(QMouseEvent * event)
{
    int column=this->columnAt(event->x());
    int row=this->rowAt(event->y());
    if(column==0 && row!=-1){
        this->setCursor(Qt::PointingHandCursor);
    }
    else{
        this->setCursor(Qt::ArrowCursor);
    }
}



int SongListView::loadDataSet(DataSet<Song*>& songs)
{
    ASSERT(model);

    // remove previous data?
    // model->reset();

    for(unsigned i=0; i<songs.size(); i++)
    {
        model->setData(model->index(i, 0), QVariant(songs.at(i)->name.c_str()));
        model->setData(model->index(i, 1), QVariant(songs.at(i)->url.c_str()));
        model->setData(model->index(i, 2), QVariant(songs.at(i)->artistName.c_str()));
        model->setData(model->index(i, 3), QVariant(i));
    }
    return OK;
}


SongListItemDelegate::SongListItemDelegate(QObject * parent)
    :QItemDelegate(parent)
{
    LOG_API();
    favouritePixmap=QPixmap(":/images/song_select.png");
    notFavouritePixmap=QPixmap("");
}


void SongListItemDelegate::paint(QPainter * painter,
        const QStyleOptionViewItem & option,
        const QModelIndex & index) const
{
    LOG_VERBOSE(__FUNCTION_NAME__);
    if(index.column()!=3)
    {
#if 0
        QStyleOptionViewItem itemOption(option);
        if(itemOption.state & QStyle::State_HasFocus)
            itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
        QStyledItemDelegate::paint(painter,itemOption,index);
        QPen oldPen = painter->pen();
        painter->setPen(pen);
        //painter->drawLine(option.rect.topRight(),option.rect.bottomRight());
        painter->drawLine(itemOption.rect.bottomLeft(),itemOption.rect.bottomRight());
        painter->setPen(oldPen);
#else
        QItemDelegate::paint(painter,option,index);
#endif
        return;
    }

    const QAbstractItemModel * model=index.model();
    QVariant var=model->data(index,Qt::CheckStateRole);
    if(var.isNull())
        var=false;
    const QPixmap & star=var.toBool()? favouritePixmap:notFavouritePixmap;

    int width=star.width();
    int height=star.height();
    QRect rect=option.rect;
    int x=rect.x()+rect.width()/2-width/2;
    int y=rect.y()+rect.height()/2-height/2;

    painter->drawPixmap(x,y,star);
    //QItemDelegate::paint(painter,option,index);
}

#if 0
bool SongListItemDelegate::editorEvent(QEvent * event,
        QAbstractItemModel * model,
        const QStyleOptionViewItem & /*option*/,
        const QModelIndex & index)
{
    LOG_API();
    if(index.column()==3)
    {
      if(event->type()== QEvent::MouseButtonPress ||
        (event->type()== QEvent::KeyPress && (static_cast<QKeyEvent*>(event))->key() == Qt::Key_Enter))
        {
            QModelIndex newidx = index.sibling(index.row(), 3);
            QVariant var=model->data(newidx,Qt::CheckStateRole);
            bool selected=var.toBool();
            if(var.isValid())
                selected=selected?false:true;
            else
                selected=true;
            model->setData(newidx,selected,Qt::CheckStateRole);
            emit signalSongSelected(model->data(index.sibling(index.row(),0)).toString(), selected);
            return true;//I have handled the event
        }
    }

    return false;
}
#endif

SongListStandardItemModel::SongListStandardItemModel(DataSet<Song*>& songs, QObject * parent)
    :QStandardItemModel(parent)
{
    QStringList headerLabels;
    headerLabels << "Name" << "Album" << "Singer" << "Status";
    this->setHorizontalHeaderLabels(headerLabels);
    this->setRowCount(5);
    this->setColumnCount(4);

    this->songs = &songs;
    for(unsigned i=0; i<songs.size(); i++)
    {
        this->setData(this->index(i, 0), QVariant(songs.at(i)->name.c_str()));
        this->setData(this->index(i, 1), QVariant(songs.at(i)->url.c_str()));
        this->setData(this->index(i, 2), QVariant(songs.at(i)->artistName.c_str()));
        this->setData(this->index(i, 3), QVariant(i));
    }

}


QVariant SongListStandardItemModel::data(
        const QModelIndex & index,
        int role) const
{
    LOG_VERBOSE(__FUNCTION_NAME__);

    /* for the time being, I put static data into the model.
       if this causes performance issue, then I will consider dynamic data fetching.
    */

#if 0
    if(role == Qt::CheckStateRole && index.column() != 4)
    {
        return QVariant();
    }

    switch(index.column())
    {
        case 0:
            return QVariant(songs->at(index.column())->name.c_str());
        case 1:
            return QVariant(songs->at(index.column())->url.c_str());
        case 2:
            return QVariant(songs->at(index.column())->artistName.c_str());
        default:
            break;
    }
#endif

    return QStandardItemModel::data(index,role);
}

void SongListStandardItemModel::slotSongAdd(Song song)
{
    LOG_INFO("<songlistview> \"%s\" selected. \n", song.name.c_str());
}

void SongListStandardItemModel::slotSongDel(Song song)
{
    LOG_INFO("<songlistview> \"%s\" canceled. \n", song.name.c_str());
}

