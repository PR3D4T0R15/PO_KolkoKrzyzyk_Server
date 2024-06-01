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
		void setDataObject(const QString& objName, const QString& objValue);
		void setDataObject(const QString& objName, const bool& objValue);
		QString getDataObjectStr(const QString& objName);
		bool getDataObjectBool(const QString& objName);

	protected:
		QJsonObject _rootObj;
	};

	class Conn : public JsonDoc
	{
	public:
		Conn();
		void setConnId(const QString& connId);
		QString getConnId();
	};

	class Account : public JsonDoc
	{
	public:
		Account();
		void setLogin();
		void setLogout();
		void setNewaccount();
		void setUsername(const QString& username);
		QString getUsername();

		void setPassword(const QString& password);
		QString getPassword();

		void setResult(const bool& result);
		bool getResult();
	};
}
