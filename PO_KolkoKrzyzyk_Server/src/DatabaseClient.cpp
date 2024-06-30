#include "include/DatabaseClient.h"

DatabaseClient::DatabaseClient(QObject *parent): QObject(parent)
{
	try
	{
		_client = new mongocxx::client(getUrl());
		_db = new mongocxx::database((*_client)[getDbName().toStdString()]);
	}
	catch (const std::exception& e)
	{
		qCritical() << "Exception: " << e.what();
		exit(-1);
	}
}

DatabaseClient::~DatabaseClient()
{
	delete _db;
	delete _client;
}


void DatabaseClient::testConn()
{
	try
	{
		auto ping_cmd = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
		mongocxx::database db = (*_client)["admin"];
		bsoncxx::document::view result = db.run_command(ping_cmd.view());
		qDebug() << "MongoDB CONNECTION....OK";
	}
	catch (const std::exception& e)
	{
		qCritical() << "Exception: " << e.what();
		exit(-1);
	}
}

QString DatabaseClient::getUserPassByNick(const QString& nick)
{
	QJsonObject query;
	QJsonObject projection; 
	query["username"] = nick;
	projection["_id"] = 0;
	projection["username"] = 0;
	projection["stats"] = 0;

	QJsonObject userJsonObj = findOne("players", query, projection);

	return userJsonObj["password"].toString();
}

QString DatabaseClient::getUserNameById(const QString& uuid)
{
	QJsonObject query;
	QJsonObject projection;
	query["_id"] = QJsonObject{ {"oid", uuid} };
	projection["_id"] = 0;
	projection["password"] = 0;
	projection["stats"] = 0;

	QJsonObject userJsonObj = findOneById("players", uuid, projection);

	return userJsonObj["username"].toString();
}

bool DatabaseClient::checkIfUserExist(const QString& username)
{
	QJsonObject filter;
	filter["username"] = username;
	QJsonObject projection;
	projection["_id"] = 0;
	projection["username"] = 1;
	projection["password"] = 0;
	projection["stats"] = 0;

	QJsonObject responseObj = findOne("players", filter, projection);

	if (responseObj.isEmpty())
	{
		return false;
	}
	return true;
}

QJsonDocument DatabaseClient::getPlayerRanking()
{
	QJsonObject filter;

	QJsonObject projection;
	projection["password"] = 0;
	projection["_id"] = 0;

	QJsonObject sort;
	sort["stats.points"] = -1;

	int limit = 5;

	QJsonDocument responseDoc = find("players", filter, projection, sort, limit);

	return responseDoc;
}

bool DatabaseClient::createPlayer(const QJsonDocument& player)
{
	return insertOne("players", player.object());
}

bool DatabaseClient::addPlayerToQueue(const QJsonObject& player)
{
	return insertOne("queue", player);
}

bool DatabaseClient::removePlayerFromQueue(const QJsonObject& player)
{
	return deleteOne("queue", player);
}

bool DatabaseClient::removePlayerFromQueue(const QString& playerId)
{
	QJsonObject query;
	query["connId"] = playerId;
	return deleteOne("queue", query);
}

QJsonDocument DatabaseClient::getPlayersInQueue()
{
	QJsonObject filter;
	QJsonObject projection;
	QJsonObject sort;
	int limit = 100;
	return find("queue", filter, projection, sort, limit);
}

int DatabaseClient::countPlayersInQueue()
{
	QJsonObject filter;

	return countDocuments("queue", filter);
}

bool DatabaseClient::addGame(const QJsonObject& game)
{
	return insertOne("game_sessions", game);
}

bool DatabaseClient::updateGame(const QString& uuid, const QJsonObject& data)
{
	return updateOneById("game_sessions", uuid, data);
}

QJsonObject DatabaseClient::getGame(const QString& uuid)
{
	QJsonObject projection, filter;
	filter["_id"] = QJsonObject{ {"oid", uuid} };

	return findOneById("game_sessions", uuid, projection);
}

bool DatabaseClient::deleteGame(const QString& uuid)
{
	QJsonObject filter;
	filter["_id"] = QJsonObject{ {"oid", uuid} };

	return deleteOne("game_sessions", filter);
}

QJsonObject DatabaseClient::findOne(const QString& collName, const QJsonObject& filter, const QJsonObject& projection)
{
	auto bsonFilter = qJsonObjToBson(filter);
	auto bsonProjection = qJsonObjToBson(projection);

	mongocxx::options::find opts;
	opts.projection(bsonProjection.view());

	auto collection = (*_db)[collName.toStdString()];
	auto result= collection.find_one(bsonFilter.view(), opts);

	QJsonObject responseJsonObj;

	if (result)
	{
		responseJsonObj = bsonToQJsonObj(result->view());
	}
	return responseJsonObj;
}

QJsonObject DatabaseClient::findOneById(const QString& collName, const QString& uuid, const QJsonObject& projection)
{
	bsoncxx::document::value query = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(uuid.toStdString())));

	auto bsonProjection = qJsonObjToBson(projection);

	mongocxx::options::find opts;
	opts.projection(bsonProjection.view());

	auto collection = (*_db)[collName.toStdString()];
	auto result = collection.find_one(query.view(), opts);

	QJsonObject responseJsonObj;

	if (result)
	{
		responseJsonObj = bsonToQJsonObj(result->view());
	}
	return responseJsonObj;
}

QJsonDocument DatabaseClient::find(const QString& collName, const QJsonObject& filter, const QJsonObject& projection, const QJsonObject& sort, const int& limit)
{
	auto bsonFilter = qJsonObjToBson(filter);
	auto bsonProjection = qJsonObjToBson(projection);
	auto bsonSort = qJsonObjToBson(sort);

	mongocxx::options::find opts;
	opts.sort(bsonSort.view());
	opts.projection(bsonProjection.view());

	if (limit != 0)
	{
		opts.limit(limit);
	}

	auto collection = (*_db)[collName.toStdString()];
	auto result = collection.find(bsonFilter.view(), opts);

	QJsonArray responseArr;
	for (auto doc : result)
	{
		std::string line = bsoncxx::to_json(doc);
		QJsonDocument lineDoc = QJsonDocument::fromJson(QByteArray::fromStdString(line));
		responseArr.append(lineDoc.object());
	}

	QJsonDocument jsonDoc(responseArr);

	return jsonDoc;
}

bool DatabaseClient::insertOne(const QString& collName, const QJsonObject& data)
{
	auto bsonData = qJsonObjToBson(data);

	auto collection = (*_db)[collName.toStdString()];
	auto result = collection.insert_one(bsonData.view());

	if (result)
	{
		return true;
	}
	return false;
}

bool DatabaseClient::updateOne(const QString& collName, const QJsonObject& filter, const QJsonObject& data)
{
	QJsonObject dataObj;
	dataObj["$set"] = data;

	auto bsonData = qJsonObjToBson(dataObj);
	auto bsonFilter = qJsonObjToBson(filter);

	auto collection = (*_db)[collName.toStdString()];
	auto result = collection.update_one(bsonFilter.view(), bsonData.view());

	if (result->modified_count() != 0)
	{
		return true;
	}
	return false;
}

bool DatabaseClient::updateOneById(const QString& collName, const QString& uuid, const QJsonObject& data)
{
	QJsonObject dataObj;
	dataObj["$set"] = data;

	auto bsonData = qJsonObjToBson(dataObj);
	bsoncxx::document::value query = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(uuid.toStdString())));

	auto collection = (*_db)[collName.toStdString()];
	auto result = collection.update_one(query.view(), bsonData.view());

	if (result->modified_count() != 0)
	{
		return true;
	}
	return false;
}

bool DatabaseClient::deleteOne(const QString& collName, const QJsonObject& filter)
{
	auto bsonFilter = qJsonObjToBson(filter);
	qDebug() << filter;
	auto collection = (*_db)[collName.toStdString()];
	auto result = collection.delete_one(bsonFilter.view());

	if (result->deleted_count() != 0)
	{
		return true;
	}
	return false;
}

int DatabaseClient::countDocuments(const QString& collName, const QJsonObject& filter)
{
	auto bsonFilter = qJsonObjToBson(filter);

	auto collection = (*_db)[collName.toStdString()];
	std::int64_t result = collection.count_documents(bsonFilter.view());

	return result;
}

mongocxx::uri DatabaseClient::getUrl()
{
	ServerSettings settings;
	QString url = "mongodb://%1:%2@%3:%4/";

	QString userName = settings.getDbUser();
	QString userPass = settings.getDbPass();
	QString ipAddr = settings.getDbHost();
	QString ipPort = settings.getDbPort();

	url = url.arg(userName).arg(userPass).arg(ipAddr).arg(ipPort);

	settings.deleteLater();

	return mongocxx::uri(url.toStdString());
}

QString DatabaseClient::getDbName()
{

	ServerSettings settings;
	QString dbName = settings.getDb();
	settings.deleteLater();

	return dbName;
}

bsoncxx::document::value DatabaseClient::qJsonObjToBson(const QJsonObject& jsonObj)
{
	QJsonDocument jsonDoc(jsonObj);
	QByteArray byteArray = jsonDoc.toJson(QJsonDocument::Compact);
	bsoncxx::document::value bsonDoc = bsoncxx::from_json(byteArray.toStdString());

	return bsonDoc;
}

QJsonObject DatabaseClient::bsonToQJsonObj(const bsoncxx::document::view& view)
{
	auto jsonString = bsoncxx::to_json(view);

	QByteArray jsonByte;
	jsonByte.append(jsonString);
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonByte);

	return jsonDoc.object();
}