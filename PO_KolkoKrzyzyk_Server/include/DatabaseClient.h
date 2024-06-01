#pragma once

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QByteArray>

#include "include/ServerSettings.h"
#include "include/JsonDoc.h"

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

class DatabaseClient : public QObject
{
	Q_OBJECT

public:
	DatabaseClient(QObject *parent = nullptr);
	~DatabaseClient();

	void testConn();

	QJsonObject find_one(const QString& collName,const QJsonObject& filter, const QJsonObject& projection);
	QJsonDocument find(const QString& collName, const QJsonObject& filter, const QJsonObject& projection, const QJsonObject& sort);
	bool insert_one(const QString& collName, const QJsonObject& data);
	bool update_one(const QString& collName, const QJsonObject& filter, const QJsonObject& data);


private:
	static mongocxx::uri getUrl();
	static QString getDbName();
	static bsoncxx::document::value qJsonObjToBson(const QJsonObject& jsonObj);
	static QJsonObject bsonToQJsonObj(const bsoncxx::document::view& view);


private:
	mongocxx::client* _client;
	mongocxx::database* _db;
};
