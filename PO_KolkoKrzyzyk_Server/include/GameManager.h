#pragma once

#include <QObject>
#include <QDebug>
#include "include/JsonDoc.h"
#include "include/DatabaseClient.h"

class GameManager : public QObject
{
	Q_OBJECT

public:
	GameManager(QObject *parent = nullptr);
	~GameManager();

public slots:
	void getRequest(const QString& connId, const QByteArray& data);


signals:
	void sendResponse(const QString& connId, const QByteArray& data);
	void sendToSessionManager(const QString& connId, const QByteArray& data);

private:

};
