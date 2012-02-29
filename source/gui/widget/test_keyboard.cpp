

#include <QtGui>
#include <QtCore>

#include "keyboard.h"
#include "inputline.h"
#include "../../include/log.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    log_init();

    KeyBoard * kb = new KeyBoard("Keyboard Test");

    QWidget * root = new QWidget();
    InputLine * username = new InputLine(kb, root);
    InputLine * password = new InputLine(kb, root);
    password->echo(false);

    QPushButton * confirm = new QPushButton("Yes, that's me.", root);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(username);
    layout->addWidget(password);
    layout->addWidget(confirm);
    root->setLayout(layout);

    root->show();

    app.exec();    
}

