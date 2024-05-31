#pragma once

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QUuid>
#include <bsoncxx/oid.hpp>

namespace jsonDoc
{
	class JsonDoc
	{
	public:
		static QString getAction(const QJsonDocument& jsonDoc);
		static QByteArray toBytes(const QJsonDocument& jsonDoc);
		static QJsonDocument toJson(const QByteArray& jsonByte);
		static QString hashData(const QString& data);
		static QJsonObject genId();
		
	};


	class SessionDoc : JsonDoc
	{
	public:
		SessionDoc();
		~SessionDoc();

		void setPlayer(const QString& playerId, const QString& connId, const QString& figure);
		QJsonArray getPlayer();

		QJsonDocument dumpSession();
		void loadSession(const QJsonDocument& jsonDoc);

	private:
		QJsonObject _jsonObject;
	};

	class AccountDoc : JsonDoc
	{
	public:
		AccountDoc();
		~AccountDoc();

		
	};
}
