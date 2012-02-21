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
Q_OBJECT

public:
    SongListView(DataSet<Song*>& songs, QWidget * parent=0);
    virtual ~SongListView();

    int loadDataSet(DataSet<Song*>& songs);

protected:
    void mouseMoveEvent(QMouseEvent * event);

private:
    SongListItemDelegate * delegate;
    SongListStandardItemModel * model;        
    void keyPressEvent (QKeyEvent * keyEvent);

signals:
    void signalSongSelected(QString song, bool selected);
};

class SongListItemDelegate:public QItemDelegate
{
Q_OBJECT

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

signals:
    void signalSongSelected(QString song, bool selected);
};

class SongListStandardItemModel:public QStandardItemModel
{
Q_OBJECT

public:
    SongListStandardItemModel(DataSet<Song*>& songs, QObject * parent = NULL);
    virtual ~ SongListStandardItemModel(){}
    
    QVariant data(const QModelIndex & index,
                  int role=Qt::DisplayRole) const;

private:
    DataSet<Song*> * songs;

protected slots:
    void slotSongSelected(QString song, bool selected);
};

#endif /* SONGLISTVIEW_HPP */
