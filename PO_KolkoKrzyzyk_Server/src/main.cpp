#include <QCoreApplication>
#include "include/TcpServer.h"
#include "include/GameManager.h"
#include "include/DatabaseClient.h"
#include "include/GameSessionManager.h"
#include <mongocxx/instance.hpp>
#include "QDebug"

int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);

	mongocxx::instance instance{};

	//db test
	DatabaseClient* dbClient = new DatabaseClient;
	dbClient->testConn();
	dbClient->deleteLater();

	TcpServer* server = new TcpServer(&app);
	GameManager* gameManager = new GameManager(&app);
	GameSessionManager* gameSessionManager = new GameSessionManager(&app);

	QObject::connect(server, &TcpServer::newDataFromClient, gameManager, &GameManager::getRequest);
	QObject::connect(gameManager, &GameManager::sendResponse, server, &TcpServer::sendDataToClient);
	QObject::connect(gameManager, &GameManager::sendToSessionManager, gameSessionManager, &GameSessionManager::receiveData);
	QObject::connect(gameSessionManager, &GameSessionManager::sendData, server, &TcpServer::sendDataToClient);

	server->startServer();

	return app.exec();
}