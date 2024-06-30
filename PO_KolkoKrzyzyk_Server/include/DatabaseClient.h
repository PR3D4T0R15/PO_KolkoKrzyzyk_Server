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
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <mongocxx/stdx.hpp>

class DatabaseClient : public QObject
{
	Q_OBJECT

public:
	DatabaseClient(QObject *parent = nullptr);
	~DatabaseClient();

	void testConn();

	QString getUserPassByNick(const QString& nick);
	QString getUserNameById(const QString& uuid);
	bool checkIfUserExist(const QString& username);
	QJsonDocument getPlayerRanking();
	bool createPlayer(const QJsonDocument& player);

	bool addPlayerToQueue(const QJsonObject& player);
	bool removePlayerFromQueue(const QJsonObject& player);
	bool removePlayerFromQueue(const QString& playerId);
	QJsonDocument getPlayersInQueue();
	int countPlayersInQueue();

	bool addGame(const QJsonObject& game);
	bool updateGame(const QString& uuid, const QJsonObject& data);
	QJsonObject getGame(const QString& uuid);
	bool deleteGame(const QString& uuid);

protected:
	QJsonObject findOne(const QString& collName,const QJsonObject& filter, const QJsonObject& projection);
	QJsonObject findOneById(const QString& collName, const QString& uuid, const QJsonObject& projection);
	QJsonDocument find(const QString& collName, const QJsonObject& filter, const QJsonObject& projection, const QJsonObject& sort, const int& limit);
	bool insertOne(const QString& collName, const QJsonObject& data);
	bool updateOne(const QString& collName, const QJsonObject& filter, const QJsonObject& data);
	bool updateOneById(const QString& collName, const QString& uuid, const QJsonObject& data);
	bool deleteOne(const QString& collName, const QJsonObject& filter);
	int countDocuments(const QString& collName, const QJsonObject& filter);

private:
	static mongocxx::uri getUrl();
	static QString getDbName();
	static bsoncxx::document::value qJsonObjToBson(const QJsonObject& jsonObj);
	static QJsonObject bsonToQJsonObj(const bsoncxx::document::view& view);


private:
	mongocxx::client* _client;
	mongocxx::database* _db;
};
