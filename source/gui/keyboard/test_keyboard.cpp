

#include <QtGui>
#include <QtCore>


int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    QWidget * root = new QWidget();
    root->setGeometry(10, 10, 800, 600);

    QLineEdit * name = new QLineEdit(root);
    QPushButton * confirm = new QPushButton("Yes, that's me!", root);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(name);
    layout->addWidget(confirm);
    root->setLayout(layout);
    
    app.exec();    
}