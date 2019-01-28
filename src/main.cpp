#include <QApplication>

#include "renamerWindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	cRenamerWindow window;
	window.show();
	return app.exec();
}
