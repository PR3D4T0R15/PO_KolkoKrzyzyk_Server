#pragma once

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QRandomGenerator>
#include <bsoncxx/oid.hpp>
#include <QDateTime>

namespace jsonDoc
{
	class JsonDoc
	{
	public:
		static QString getAction(const QJsonDocument& jsonDoc);
		static QJsonObject getData(const QJsonDocument& jsonDoc);

		static QByteArray toBytes(const QJsonDocument& jsonDoc);
		static QJsonDocument toJson(const QByteArray& jsonByte);

		static QString hashData(const QString& data);
		static QJsonObject genId();

		void setJson(const QJsonDocument& jsonDoc);
		void setJson(const QJsonObject& jsonObj);

		QJsonDocument getJsonDoc();
		QJsonObject getJsonObj();

	protected:
		QJsonObject _rootObj;
	};

	class Conn : JsonDoc
	{
	public:
		Conn();
		void setConnId(const QString& connId);
	};
}
