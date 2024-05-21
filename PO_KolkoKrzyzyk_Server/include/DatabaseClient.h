#pragma once

#include <QObject>

class DatabaseClient : public QObject
{
	Q_OBJECT

public:
	DatabaseClient(QObject *parent);
	~DatabaseClient();
};
