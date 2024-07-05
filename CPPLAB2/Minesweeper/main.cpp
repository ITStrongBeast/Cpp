#include "game.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	bool flag = argc > 1 && strcmp(argv[1], "dbg") == 0;
	Game w(flag, nullptr);
	w.show();
	return a.exec();
}
