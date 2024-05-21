#include "include/ServerSettings.h"

ServerSettings::ServerSettings(QObject *parent): QObject(parent)
{
	QString filePath = "config.ini";

	QFile file(filePath);

	if (file.exists())
	{
		_settings = new QSettings(filePath, QSettings::IniFormat);
	}
	else
	{
		qCritical() << "Settings file " << filePath << " not found";
	}
}

ServerSettings::~ServerSettings()
{
	_settings->deleteLater();
}


QString ServerSettings::getDbHost()
{
	return _settings->value("Database/host").toString();
}

QString ServerSettings::getDbUser()
{
	return _settings->value("Database/user").toString();
}

QString ServerSettings::getDbPass()
{
	return _settings->value("Database/password").toString();
}

QString ServerSettings::getDb()
{
	return _settings->value("Database/database").toString();
}
