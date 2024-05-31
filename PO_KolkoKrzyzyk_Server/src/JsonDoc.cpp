#include "include/JsonDoc.h"

//JsonDoc
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



//SessionDoc
jsonDoc::SessionDoc::SessionDoc()
{
	_jsonObject["_id"] = jsonDoc::JsonDoc::genId();
	_jsonObject["starting"] = "";
	_jsonObject["round"] = 1;
	_jsonObject["EndTime"] = "";

	QJsonArray players;

	_jsonObject["players"] = players;

	QJsonArray gameField;
	gameField.append(QJsonArray({ "n", "n", "n" }));
	gameField.append(QJsonArray({ "n", "n", "n" }));
	gameField.append(QJsonArray({ "n", "n", "n" }));

	_jsonObject["gameField"] = gameField;

}

jsonDoc::SessionDoc::~SessionDoc()
{
}

void jsonDoc::SessionDoc::setPlayer(const QString& playerId, const QString& connId, const QString& figure)
{
	QJsonObject player;
	player["player"] = playerId;
	player["connId"] = connId;
	player["figure"] = figure;

	QJsonArray players = _jsonObject["players"].toArray();
	players.append(player);

	_jsonObject["players"] = players;

}

QJsonArray jsonDoc::SessionDoc::getPlayer()
{
	return _jsonObject["players"].toArray();
}

QJsonDocument jsonDoc::SessionDoc::dumpSession()
{
	QJsonDocument jsonDoc;

	jsonDoc.setObject(_jsonObject);

	return jsonDoc;
}

void jsonDoc::SessionDoc::loadSession(const QJsonDocument& jsonDoc)
{
	_jsonObject = jsonDoc.object();
}
