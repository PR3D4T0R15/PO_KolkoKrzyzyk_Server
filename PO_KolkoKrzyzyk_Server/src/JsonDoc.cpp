#include "include/JsonDoc.h"

//JsonDoc class
QString jsonDoc::JsonDoc::getAction(const QJsonDocument& jsonDoc)
{
	QString action;

	if (!jsonDoc.isNull())
	{
		QJsonObject jsonObj = jsonDoc.object();
		action = jsonObj["action"].toString();
	}

	return action;
}

QJsonObject jsonDoc::JsonDoc::getData(const QJsonDocument& jsonDoc)
{
	QJsonObject array;

	if (!jsonDoc.isNull())
	{
		QJsonObject jsonObj = jsonDoc.object();
		array = jsonObj["data"].toObject();
	}

	return array;
}

QByteArray jsonDoc::JsonDoc::toBytes(const QJsonDocument& jsonDoc)
{
	return jsonDoc.toJson();
}

QJsonDocument jsonDoc::JsonDoc::toJson(const QByteArray& jsonByte)
{
	return QJsonDocument::fromJson(jsonByte);
}

QString jsonDoc::JsonDoc::hashData(const QString& data)
{
	QByteArray dataByte = data.toUtf8();

	return QCryptographicHash::hash(dataByte, QCryptographicHash::Sha256).toHex();
}

QJsonObject jsonDoc::JsonDoc::genId()
{
	QJsonObject idObject;
	bsoncxx::oid id;

	idObject["$oid"] = QString::fromStdString(id.to_string());

	return idObject; //add to "_id"
}

void jsonDoc::JsonDoc::setJson(const QJsonDocument& jsonDoc)
{
	_rootObj = jsonDoc.object();
}

void jsonDoc::JsonDoc::setJson(const QJsonObject& jsonObj)
{
	_rootObj = jsonObj;
}

QJsonDocument jsonDoc::JsonDoc::getJsonDoc()
{
	QJsonDocument jsonDoc(_rootObj);

	return jsonDoc;
}

QJsonObject jsonDoc::JsonDoc::getJsonObj()
{
	return _rootObj;
}



//Conn class
jsonDoc::Conn::Conn()
{
	_rootObj["action"] = "connection";

	QJsonArray dataArr;
	_rootObj["data"] = dataArr;
}

void jsonDoc::Conn::setConnId(const QString& connId)
{
	QJsonArray dataArr = _rootObj["data"].toArray();

	QJsonObject dataArrObj;
	dataArrObj["connId"] = connId;
	dataArr.append(dataArrObj);

	_rootObj["data"] = dataArr;
}
