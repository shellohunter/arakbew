
#include "SongListView.hpp"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    QWidget * mainwindow = new QWidget();
    mainwindow->resize(330, 120+10);
    //mainwindow->setFixedSize(330, 120+10);
    //mainwindow->setAutoFillBackground(true);

    SongItemView * item1 = new SongItemView(mainwindow);
    SongItemView * item2 = new SongItemView(mainwindow);
    SongItemView * item3 = new SongItemView(mainwindow);
    SongItemView * item4 = new SongItemView(mainwindow);
    SongItemView * item5 = new SongItemView(mainwindow);

    QVBoxLayout * vbox = new QVBoxLayout();
    vbox->addWidget(item1);
    vbox->addWidget(item2);
    vbox->addWidget(item3);
    vbox->addWidget(item4);
    vbox->addWidget(item5);

    mainwindow->setLayout(vbox);
    item1->setFocus();
    mainwindow->show();
    app.exec();
    return 0;
}
