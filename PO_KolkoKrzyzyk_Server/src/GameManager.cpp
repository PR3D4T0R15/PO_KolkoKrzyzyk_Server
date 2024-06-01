#include "include/GameManager.h"

GameManager::GameManager(QObject *parent): QObject(parent)
{
}

GameManager::~GameManager()
{
}

void GameManager::getRequest(const QString& connId, const QByteArray& data)
{
	QString requestAction = jsonDoc::JsonDoc::getAction(jsonDoc::JsonDoc::toJson(data));
	QJsonObject requestData = jsonDoc::JsonDoc::getData(jsonDoc::JsonDoc::toJson(data));

	if (requestAction == "login")
	{
		
		
	}
	if (requestAction == "logout")
	{
		
	}
	if (requestAction == "rankingUpdate")
	{
		
	}
}

bool GameManager::checkPassword(const QJsonObject& dataFromClient)
{
	return true;
}
