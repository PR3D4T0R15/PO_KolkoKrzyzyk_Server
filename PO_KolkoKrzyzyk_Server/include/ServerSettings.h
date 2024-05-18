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

	QString getHost();
	QString getUser();
	QString getPass();
	QString getDb();

private:
	QSettings* _settings;
};
