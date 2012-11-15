#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include "studrating.h"


int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	app.setApplicationName(APP_NAME);
	app.setApplicationVersion(VERSION);

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), "/usr/share/qt/translations");
	app.installTranslator(&qtTranslator);

	studrating foo;
	foo.show();
	return app.exec();
}
