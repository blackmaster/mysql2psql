#include "application/application.h"
#include <QDebug>

using namespace MySQL2PSQL::Types;

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	MySQL2PSQL::General::CApplication application;

	application.run();

	return 0;
}
