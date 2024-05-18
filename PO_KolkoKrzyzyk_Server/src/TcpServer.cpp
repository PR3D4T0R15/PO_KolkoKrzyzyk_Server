#include "include/TcpServer.h"
#include <QDebug>

//constructor
TcpServer::TcpServer(QObject* parent): QObject(parent)
{
	_tcp_server = new QTcpServer(this);
}

//destructor
//close all connections and delete server 
TcpServer::~TcpServer()
{
	foreach(QTcpSocket* socket, _clients_list)
	{
		socket->close();
		socket->deleteLater();
	}

	_tcp_server->close();
	_tcp_server->deleteLater();
}

//start server and connect signals
void TcpServer::startServer(const QHostAddress address, const quint16 port) const
{
	bool serverStatus = _tcp_server->listen(address, port);

	if (serverStatus)
	{
		qDebug() << "Server started. IP: " << _tcp_server->serverAddress().toString() << " PORT: " << _tcp_server->serverPort();
		QObject::connect(_tcp_server, &QTcpServer::newConnection, this, &TcpServer::newConnection);
	}
	else
	{
		qDebug() << "Server ERROR";
		qDebug() << _tcp_server->errorString();
		
	}
}