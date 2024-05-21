#pragma once

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QByteArray>

#include "include/ServerSettings.h"

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

class DatabaseClient : public QObject
{
	Q_OBJECT

public:
	DatabaseClient(QObject *parent = nullptr);
	~DatabaseClient();

	void testConn();
	void getData(const QJsonDocument& query);
	void instertData();
	void updateData();

private:
	mongocxx::uri getUrl();
	QString getDbName();
	bsoncxx::document::value QJsonDocumentToBson(const QJsonDocument& jsonDoc);


private:
	mongocxx::instance* _inst;
	mongocxx::client* _client;
	mongocxx::database* _db;
};
