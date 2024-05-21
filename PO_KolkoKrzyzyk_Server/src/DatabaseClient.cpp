#include "include/DatabaseClient.h"

DatabaseClient::DatabaseClient(QObject *parent): QObject(parent)
{
	try
	{
		_inst = new mongocxx::instance();
		_client = new mongocxx::client(generateUrl());
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

mongocxx::uri DatabaseClient::generateUrl()
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
