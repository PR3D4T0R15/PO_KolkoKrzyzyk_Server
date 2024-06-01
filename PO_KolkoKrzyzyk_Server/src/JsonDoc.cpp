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

void jsonDoc::JsonDoc::setDataObject(const QString& objName, const QString& objValue)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	dataObj[objName] = objValue;

	_rootObj["data"] = dataObj;
}

void jsonDoc::JsonDoc::setDataObject(const QString& objName, const bool& objValue)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	dataObj[objName] = objValue;

	_rootObj["data"] = dataObj;
}

QString jsonDoc::JsonDoc::getDataObjectStr(const QString& objName)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	return dataObj["connId"].toString();
}

bool jsonDoc::JsonDoc::getDataObjectBool(const QString& objName)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	return dataObj["connId"].toBool();
}


//Conn class
jsonDoc::Conn::Conn()
{
	_rootObj["action"] = "connection";

	QJsonObject dataObj;
	_rootObj["data"] = dataObj;
}

void jsonDoc::Conn::setConnId(const QString& connId)
{
	JsonDoc::setDataObject("connId", connId);
}

QString jsonDoc::Conn::getConnId()
{
	return JsonDoc::getDataObjectStr("connId");
}



//Account class
jsonDoc::Account::Account()
{
	_rootObj["action"] = "";

	QJsonObject dataObj;
	_rootObj["data"] = dataObj;
}

void jsonDoc::Account::setLogin()
{
	_rootObj["action"] = "login";
}

void jsonDoc::Account::setLogout()
{
	_rootObj["action"] = "logout";
}

void jsonDoc::Account::setNewaccount()
{
	_rootObj["action"] = "NewAccount";
}

void jsonDoc::Account::setUsername(const QString& username)
{
	JsonDoc::setDataObject("username", username);
}

QString jsonDoc::Account::getUsername()
{
	return JsonDoc::getDataObjectStr("username");
}

void jsonDoc::Account::setPassword(const QString& password)
{
	JsonDoc::setDataObject("password", password);
}

QString jsonDoc::Account::getPassword()
{
	return JsonDoc::getDataObjectStr("password");
}

void jsonDoc::Account::setResult(const bool& result)
{
	JsonDoc::setDataObject("result", result);
}

bool jsonDoc::Account::getResult()
{
	return JsonDoc::getDataObjectBool("result");
}
