#include "include/DatabaseClient.h"

DatabaseClient::DatabaseClient(QObject *parent): QObject(parent)
{
	try
	{
		_inst = new mongocxx::instance();
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
	delete _inst;
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

QJsonDocument DatabaseClient::getSingleData(const QString& collName, const QJsonDocument& query)
{
	auto bsonFilter = qJsonDocumentToBson(query);

	auto collection = (*_db)[collName.toStdString()];
	auto result = collection.find_one(bsonFilter.view());
	QJsonDocument jsonDoc;

	if (result)
	{
		jsonDoc = bsonToQJsonDocument(result->view());
	}
	return jsonDoc;
}

bool DatabaseClient::instertSingleData(const QString& collName, const QJsonDocument& data)
{
	auto collection = (*_db)[collName.toStdString()];
	auto bsonData = qJsonDocumentToBson(data);

	auto result = collection.insert_one(bsonData.view());

	if (result)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DatabaseClient::updateSingleData(const QString& collName, const QJsonDocument& query, const QJsonDocument& data)
{
	auto collection = (*_db)[collName.toStdString()];
	auto bsonQuery = qJsonDocumentToBson(query);
	auto bsonData = qJsonDocumentToBson(data);
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

bsoncxx::document::value DatabaseClient::qJsonDocumentToBson(const QJsonDocument& jsonDoc)
{
	QByteArray byteArray = jsonDoc.toJson(QJsonDocument::Compact);
	bsoncxx::document::value bsonDoc = bsoncxx::from_json(byteArray.toStdString());

	return bsonDoc;
}

QJsonDocument DatabaseClient::bsonToQJsonDocument(const bsoncxx::document::view& view)
{
	auto jsonString = bsoncxx::to_json(view);

	QByteArray jsonByte;
	jsonByte.append(jsonString);
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonByte);

	return jsonDoc;
}
