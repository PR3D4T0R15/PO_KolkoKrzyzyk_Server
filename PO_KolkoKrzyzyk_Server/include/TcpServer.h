#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSet>
#include <QMap>
#include <QByteArray>
#include <QString>
#include <QUuid>


class TcpServer : public QObject
{
	Q_OBJECT

public:
	explicit TcpServer(QObject *parent = nullptr);
	~TcpServer();

	void startServer(const QHostAddress address = QHostAddress::AnyIPv4, quint16 port = 2390) const;

	

public slots:
	void sendDataToClient(QString connId, QByteArray data);

private slots:
	void clientConnected();
	void clientDisconnected();

	void receiveData();

	

signals:
	void newDataFromClient(QString connId, QByteArray data);

private:
	QString genId();
	QTcpSocket* findSocket(QString connId);

private:
	QTcpServer* _tcp_server;
	QSet<QTcpSocket*> _clients_list;
	quint8 _clients_count;
};
