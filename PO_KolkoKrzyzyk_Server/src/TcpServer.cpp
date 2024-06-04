#include "include/TcpServer.h"
#include <QDebug>

TcpServer::TcpServer(QObject* parent): QObject(parent)
{
	_tcp_server = new QTcpServer(this);
	_clients_count = 0;
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



void TcpServer::startServer() const
{
	ServerSettings settings;

	QHostAddress address(settings.getServerAddress());
	quint16 port = settings.getServerPort().toInt();

	bool serverStatus = _tcp_server->listen(address, port);

	if (serverStatus)
	{
		qDebug() << "Server started. IP:" << _tcp_server->serverAddress().toString() << " PORT:" << _tcp_server->serverPort();
		connect(_tcp_server, &QTcpServer::newConnection, this, &TcpServer::clientConnected);
	}
	else
	{
		qDebug() << "Server ERROR";
		qDebug() << _tcp_server->errorString();
	}
}

void TcpServer::sendDataToClient(const QString& connId, const QByteArray& data)
{
	QTcpSocket* socket = findSocket(connId);
	qint64 result;

	if (socket != nullptr)
	{
		result = socket->write(data);
	}
	else
	{
		qDebug() << "ERROR sending message. No client socket";
	}

	if (result == -1)
	{
		qDebug() << "ERROR writing message to client";
	}
}

void TcpServer::clientConnected()
{
	QTcpSocket* clientSocket = _tcp_server->nextPendingConnection();

	QString connId = genId();
	clientSocket->setProperty("connId", connId);

	connect(clientSocket, &QAbstractSocket::connected, this, &TcpServer::clientConnected);
	connect(clientSocket, &QAbstractSocket::disconnected, this, &TcpServer::clientDisconnected);
	connect(clientSocket, &QAbstractSocket::readyRead, this, &TcpServer::receiveData);

	jsonDoc::Conn jsonConn;
	jsonConn.setConnId(connId);
	clientSocket->write(jsonConn.getJsonDoc().toJson());

	_clients_list.insert(clientSocket);
	_clients_count++;
	qDebug() << "client connected: " << clientSocket->peerAddress().toString() << "Total users: " << _clients_count;
}

void TcpServer::clientDisconnected()
{
	QTcpSocket* sender = dynamic_cast<QTcpSocket*>(QObject::sender());

	_clients_list.remove(sender);
	_clients_count--;
	qDebug() << "client disconnected" << sender->peerAddress().toString() << "Total users: " << _clients_count;
	sender->deleteLater();
}

void TcpServer::receiveData()
{
	QTcpSocket* sender = dynamic_cast<QTcpSocket*>(QObject::sender());

	QByteArray data = sender->readAll();
	QString connId = sender->property("connId").toString();

	qDebug() << "Packet from: " << sender->peerAddress().toString();

	emit newDataFromClient(connId, data);
}

QString TcpServer::genId()
{
	QUuid namespaceUid = QUuid::createUuid();
	QString name = "TcpServerClient";
	QUuid uid = QUuid::createUuidV5(namespaceUid, name);

	return uid.toString();
}

QTcpSocket* TcpServer::findSocket(const QString& connId)
{
	for (QTcpSocket* socket : _clients_list) {
		if (socket->property("connId") == connId)
		{
			return socket;
		}
	}

	return nullptr;
}
