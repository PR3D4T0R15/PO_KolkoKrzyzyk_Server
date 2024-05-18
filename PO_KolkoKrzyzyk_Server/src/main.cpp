#include "include/main.h"
#include "include/TcpServer.h"
#include <QCoreApplication>

int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);

	TcpServer server;

	server.startServer();


	return app.exec();
}