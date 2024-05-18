#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSet>
#include <QByteArray>
#include <QString>


class TcpServer : public QObject
{
	Q_OBJECT

public:
	explicit TcpServer(QObject *parent = nullptr);
	~TcpServer();

	void startServer(const QHostAddress address = QHostAddress::AnyIPv4, quint16 port = 2390) const;

private:
	

public slots:


private slots:
	void newConnection();
	void appendToSocketList(QTcpSocket* socket);

	void deleteSocket();



signals:


private:
	QTcpServer* _tcp_server;
	QSet<QTcpSocket*> _clients_list;
	quint8 _clients_count;
};
