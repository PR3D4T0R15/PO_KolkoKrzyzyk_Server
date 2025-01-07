#pragma once

#include <QObject>
#include <QSettings>
#include <QFile>
#include <QString>
#include <QDebug>

class ServerSettings : public QObject
{
	Q_OBJECT

public:
	ServerSettings(QObject *parent = nullptr);
	~ServerSettings();

	QString getDbHost();
	QString getDbPort();
	QString getDbUser();
	QString getDbPass();
	QString getDb();

	QString getServerAddress();
	QString getServerPort();

private:
	QSettings* _settings;
};
