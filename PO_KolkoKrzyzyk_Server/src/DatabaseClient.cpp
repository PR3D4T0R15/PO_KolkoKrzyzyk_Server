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

QJsonObject DatabaseClient::find_one(const QString& collName, const QJsonObject& filter, const QJsonObject& projection)
{
	auto bsonFilter = qJsonObjToBson(filter);
	auto bsonProjection = qJsonObjToBson(projection);

	mongocxx::options::find opts;
	opts.projection(bsonProjection.view());

	auto collection = (*_db)[collName.toStdString()];
	auto result = collection.find_one(bsonFilter.view(), opts);

	QJsonObject responseJsonObj;

	if (result)
	{
		responseJsonObj = bsonToQJsonObj(result->view());
	}
	return responseJsonObj;
}

QJsonDocument DatabaseClient::find(const QString& collName, const QJsonObject& filter, const QJsonObject& projection, const QJsonObject& sort)
{
	auto bsonFilter = qJsonObjToBson(filter);
	auto bsonProjection = qJsonObjToBson(projection);
	auto bsonSort = qJsonObjToBson(sort);

	mongocxx::options::find opts;
	opts.sort(bsonSort.view());
	opts.projection(bsonProjection.view());

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

bool DatabaseClient::insert_one(const QString& collName, const QJsonObject& data)
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

bool DatabaseClient::update_one(const QString& collName, const QJsonObject& filter, const QJsonObject& data)
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

bool DatabaseClient::delete_one(const QString& collName, const QJsonObject& filter)
{
	auto bsonFilter = qJsonObjToBson(filter);

	auto collection = (*_db)[collName.toStdString()];
	auto result = collection.delete_one(bsonFilter.view());

	if (result->deleted_count() != 0)
	{
		return true;
	}
	return false;
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
