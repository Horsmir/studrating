#include <QtGui/QApplication>
#include "studrating.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    studrating foo;
    foo.show();
    return app.exec();
}
