#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSet>
#include <QMap>
#include <QByteArray>
#include <QString>
#include "include/JsonDoc.h"
#include "include/ServerSettings.h"


class TcpServer : public QObject
{
	Q_OBJECT

public:
	explicit TcpServer(QObject *parent = nullptr);
	~TcpServer();

	void startServer() const;

public slots:
	void sendDataToClient(const QString& connId, const QByteArray& data);

private slots:
	void clientConnected();
	void clientDisconnected();

	void receiveData();

signals:
	void newDataFromClient(const QString& connId, const QByteArray& data);

private:
	QString genId();
	QTcpSocket* findSocket(const QString& connId);


private:
	QTcpServer* _tcp_server;
	QSet<QTcpSocket*> _clients_list;
	quint8 _clients_count;
};
