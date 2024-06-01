#include <QCoreApplication>
#include "include/TcpServer.h"
#include "include/DatabaseClient.h"
#include "include/GameManager.h"
#include "include/JsonDoc.h"
#include <mongocxx/instance.hpp>
#include <QDebug>


int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);

	mongocxx::instance instance{};

	//TcpServer* server = new TcpServer();
	//GameManager* gameManager = new GameManager();

	//QObject::connect(server, &TcpServer::newDataFromClient, gameManager, &GameManager::getRequest);
	//QObject::connect(gameManager, &GameManager::sendResponse, server, &TcpServer::sendDataToClient);

	//server->startServer();

	//test begin
	jsonDoc::Conn test0;
	test0.setConnId("sdfsdfmsv");

	jsonDoc::Conn test;

	test.setConnId("makapaka");

	qDebug() << test.getJsonDoc();
	//test end


	return app.exec();
}