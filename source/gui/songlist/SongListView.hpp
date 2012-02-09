#ifndef SONGLISTVIEW_HPP
#define SONGLISTVIEW_HPP

#include <QtGui>
#include <QtCore>
#include "database.hpp"

class SongListView;
class SongListItemDelegate;
class SongListStandardItemModel;

class SongListView:public QTableView
{
    public:
        SongListView(DataSet<Song*>& songs, QWidget * parent=0);
        virtual ~SongListView(){}

    protected:
        void mouseMoveEvent(QMouseEvent * event);

    private:
        SongListItemDelegate * delegate;
        SongListStandardItemModel * model;        
        void keyPressEvent (QKeyEvent * keyEvent);
};

class SongListItemDelegate:public QItemDelegate
{
    public:
        SongListItemDelegate(QObject * parent=0);
        virtual ~ SongListItemDelegate(){}

        void paint(QPainter * painter,
                const QStyleOptionViewItem & option,
                const QModelIndex & index) const;
        bool editorEvent(QEvent * event,
                QAbstractItemModel * model,
                const QStyleOptionViewItem & option,
                const QModelIndex & index);
    private:
        QPixmap favouritePixmap;
        QPixmap notFavouritePixmap;

};

class SongListStandardItemModel:public QStandardItemModel
{
    public:
        SongListStandardItemModel(DataSet<Song*>& songs, QObject * parent = NULL);
        virtual ~ SongListStandardItemModel(){}
        
        QVariant data(const QModelIndex & index,
                      int role=Qt::DisplayRole) const;
};

#endif /* SONGLISTVIEW_HPP */
