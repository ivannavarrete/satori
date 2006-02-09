
#include <iostream>
#include <cstring>
#include "ui/txt/txtmain.h"
#include "ui/qt/qtmain.h"


int main(int argc, char *argv[]) {
	if (argc == 1)
		return ExecTxtUI(argc, argv);

	if (argc == 2 && strcmp(argv[1], "-g") == 0)
		return ExecQtUI(argc, argv);

	std::cout << "usage: satori [-g]" << std::endl;
	return 0;
}
