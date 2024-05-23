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

	QJsonDocument getSingleData(const QString& collName,const QJsonDocument& filter);
	bool instertSingleData(const QString& collName, const QJsonDocument& data);
	bool updateSingleData(const QString& collName, const QJsonDocument& filter, const QJsonDocument& data);
	bool deleteSingleData(const QString& collName, const QJsonDocument& filter);

private:
	mongocxx::uri getUrl();
	QString getDbName();
	bsoncxx::document::value qJsonDocumentToBson(const QJsonDocument& jsonDoc);
	QJsonDocument bsonToQJsonDocument(const bsoncxx::document::view& view);


private:
	mongocxx::instance* _inst;
	mongocxx::client* _client;
	mongocxx::database* _db;
};
