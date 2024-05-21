#include "include/TcpServer.h"
#include <QDebug>

TcpServer::TcpServer(QObject* parent): QObject(parent)
{
	_tcp_server = new QTcpServer(this);
}

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



void TcpServer::startServer(const QHostAddress address, const quint16 port) const
{
	bool serverStatus = _tcp_server->listen(address, port);

	if (serverStatus)
	{
		qDebug() << "Server started. IP: " << _tcp_server->serverAddress().toString() << " PORT: " << _tcp_server->serverPort();
		connect(_tcp_server, &QTcpServer::newConnection, this, &TcpServer::clientConnected);
	}
	else
	{
		qDebug() << "Server ERROR";
		qDebug() << _tcp_server->errorString();
	}
}

void TcpServer::clientConnected()
{
	QTcpSocket* clientSocket = _tcp_server->nextPendingConnection();

	connect(clientSocket, &QAbstractSocket::connected, this, &TcpServer::clientConnected);
	connect(clientSocket, &QAbstractSocket::disconnected, this, &TcpServer::clientDisconnected);
	connect(clientSocket, &QAbstractSocket::readyRead, this, &TcpServer::receiveData);

	clientSocket->write("hello\n");

	_clients_list.insert(clientSocket);
	_clients_count++;
	qDebug() << "client connected";
	qDebug() << "Total users: " << _clients_count;
}

void TcpServer::clientDisconnected()
{
	QTcpSocket* sender = dynamic_cast<QTcpSocket*>(QObject::sender());

	_clients_list.remove(sender);
	_clients_count--;
	qDebug() << "client disconnected";
}

void TcpServer::receiveData()
{
	QTcpSocket* sender = dynamic_cast<QTcpSocket*>(QObject::sender());

	QByteArray data = sender->readAll();

	qDebug() << data.toStdString();
}

void TcpServer::sendDataAll()
{
}

void TcpServer::sendData(QString user)
{
}
