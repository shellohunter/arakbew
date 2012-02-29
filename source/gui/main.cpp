
#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "Karaoke.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    Karaoke * x = new Karaoke();

    x->resume();
    return app.exec();
}
