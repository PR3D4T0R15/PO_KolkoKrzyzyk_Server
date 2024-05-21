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

void DatabaseClient::getData(const QJsonDocument& query)
{
	auto bsonFilter = QJsonDocumentToBson(query);
	try
	{
		auto collection = (*_db)["col1"];
		auto cursor = collection.find(bsonFilter.view());

		for (auto&& doc : cursor) {
			std::cout << bsoncxx::to_json(doc) << std::endl;
		}
		qDebug() << "test";
	}
	catch (const std::exception& e)
	{
		qCritical() << "Exception: " << e.what();
	}
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

bsoncxx::document::value DatabaseClient::QJsonDocumentToBson(const QJsonDocument& jsonDoc)
{
	QByteArray byteArray = jsonDoc.toJson(QJsonDocument::Compact);
	bsoncxx::document::value bsonDoc = bsoncxx::from_json(byteArray.toStdString());

	return bsonDoc;
}
