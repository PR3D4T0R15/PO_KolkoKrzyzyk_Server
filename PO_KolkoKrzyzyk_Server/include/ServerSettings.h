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
	QString getDbUser();
	QString getDbPass();
	QString getDb();

private:
	QSettings* _settings;
};
