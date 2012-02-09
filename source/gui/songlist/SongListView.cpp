

#include<QtGui>
#include<QtCore>
#include"SongListView.hpp"



SongListView::SongListView(DataSet<Song*>& songs, QWidget * parent)
    :QTableView(parent)
{
    API();
    delegate=new SongListItemDelegate();
    model=new SongListStandardItemModel(songs);    

    this->setModel(model);
    this->setItemDelegate(delegate);

    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setMouseTracking(true);//important
    installEventFilter(this);
}

void SongListView::keyPressEvent (QKeyEvent * keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_Select:
        case Qt::Key_Enter:
        case Qt::Key_Return:
        {
            QVariant var=model->data(currentIndex().parent(),Qt::CheckStateRole);
            bool isFavourite=var.toBool();
            if(var.isValid())
                isFavourite=isFavourite?false:true;
            else
                isFavourite=true;
            model->setData(currentIndex().parent(),isFavourite,Qt::CheckStateRole);
            qDebug("Key_Select %d!!!\n", isFavourite);
            qDebug()<<currentIndex().data().toString();
            break;
        }
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

SongListItemDelegate::SongListItemDelegate(QObject * parent)
    :QItemDelegate(parent)
{
    API();
    favouritePixmap=QPixmap(":/images/song_select.png");
    notFavouritePixmap=QPixmap("");
}

void SongListItemDelegate::paint(QPainter * painter,
        const QStyleOptionViewItem & option,
        const QModelIndex & index) const
{
    API();
    if(index.column()!=3)
    {
        QItemDelegate::paint(painter,option,index);
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

bool SongListItemDelegate::editorEvent(QEvent * event,
        QAbstractItemModel * model,
        const QStyleOptionViewItem & /*option*/,
        const QModelIndex & index)
{
    API();
    if(event->type()== QEvent::MouseButtonPress && index.column()==3)
    {
        QVariant var=model->data(index,Qt::CheckStateRole);
        bool isFavourite=var.toBool();
        if(var.isValid())
            isFavourite=isFavourite?false:true;
        else
            isFavourite=true;
        model->setData(index,isFavourite,Qt::CheckStateRole);
        return true;//I have handled the event
    }

    return false;
}

SongListStandardItemModel::SongListStandardItemModel(DataSet<Song*>& songs, QObject * parent)
    :QStandardItemModel(parent)
{
    QStringList headerLabels;
    headerLabels << "Name" << "Album" << "Singer" << "Status";
    this->setHorizontalHeaderLabels(headerLabels);
    this->setRowCount(songs.size());
    this->setColumnCount(4);
    for(int i=0; i<songs.size(); i++)
    {
        this->setData(this->index(i, 0), QVariant(songs.data[i]->name.c_str())); //为每行设置值
        this->setData(this->index(i, 1), QVariant(songs.data[i]->url.c_str()));
        this->setData(this->index(i, 2), QVariant(songs.data[i]->artistName.c_str()));
    }

}


QVariant SongListStandardItemModel::data(
        const QModelIndex & index,
        int role) const
{
    API();
#if 0
    int column=index.column();

    if(role==Qt::DisplayRole && column!=3)
        return tr("heyssssss");
    if(role==Qt::ToolTipRole && column==3)
        return tr("love");
#endif
    return QStandardItemModel::data(index,role);
}

