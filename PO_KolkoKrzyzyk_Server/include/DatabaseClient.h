#pragma once

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

#include "include/ServerSettings.h"

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

class DatabaseClient : public QObject
{
	Q_OBJECT

public:
	DatabaseClient(QObject *parent = nullptr);
	~DatabaseClient();

	void testConn();


private:
	mongocxx::uri generateUrl();



private:
	mongocxx::instance* _inst;
	mongocxx::client* _client;
};
