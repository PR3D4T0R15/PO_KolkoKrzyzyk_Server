#include "include/JsonDoc.h"

//JsonDoc class
jsonDoc::JsonDoc::JsonDoc()
{
	QJsonObject dataObj;
	_rootObj["data"] = dataObj;
}

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

QJsonObject jsonDoc::JsonDoc::getData()
{
	return _rootObj["data"].toObject();
}

void jsonDoc::JsonDoc::setData(const QJsonObject& data)
{
	_rootObj["data"] = data;
}

QByteArray jsonDoc::JsonDoc::toBytes(const QJsonDocument& jsonDoc)
{
	return jsonDoc.toJson(QJsonDocument::Compact);
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

void jsonDoc::JsonDoc::setDataObject(const QString& objName, const int& objValue)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	dataObj[objName] = objValue;

	_rootObj["data"] = dataObj;
}

void jsonDoc::JsonDoc::setDataObject(const QString& objName, const QJsonArray& objValue)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	dataObj[objName] = objValue;

	_rootObj["data"] = dataObj;
}

void jsonDoc::JsonDoc::setDataObject(const QString& objName, const QJsonObject& objValue)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	dataObj[objName] = objValue;

	_rootObj["data"] = dataObj;
}

QString jsonDoc::JsonDoc::getDataObjectStr(const QString& objName)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	return dataObj[objName].toString();
}

bool jsonDoc::JsonDoc::getDataObjectBool(const QString& objName)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	return dataObj[objName].toBool();
}

QJsonArray jsonDoc::JsonDoc::getDataObjectArray(const QString& objName)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	return dataObj[objName].toArray();
}

int jsonDoc::JsonDoc::getDataObjectInt(const QString& objName)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	return dataObj[objName].toInt();
}

QJsonObject jsonDoc::JsonDoc::getDataObjectJsonObj(const QString& objName)
{
	QJsonObject dataObj = _rootObj["data"].toObject();

	return dataObj[objName].toObject();
}


//Conn class
jsonDoc::Conn::Conn() : JsonDoc()
{
	_rootObj["action"] = "connection";
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
jsonDoc::Account::Account() : JsonDoc()
{
	_rootObj["action"] = "";
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
	_rootObj["action"] = "newAccount";
}

void jsonDoc::Account::setEmptyStats()
{
	QJsonObject stats;
	stats["win"] = 0;
	stats["lost"] = 0;
	stats["points"] = 0;

	QJsonObject dataObj = _rootObj["data"].toObject();
	dataObj["stats"] = stats;
	_rootObj["data"] = dataObj;
}

void jsonDoc::Account::addWin()
{
	QJsonObject stats = _rootObj["stats"].toObject();

	int win = stats["win"].toInt();
	int points = stats["points"].toInt();

	win++;
	points = points + 2;

	stats["win"] = win;
	stats["points"] = points;

	_rootObj["stats"] = stats;

}

void jsonDoc::Account::addLost()
{
	QJsonObject stats = _rootObj["stats"].toObject();

	int lost = stats["lost"].toInt();
	int points = stats["points"].toInt();

	lost++;
	points--;

	stats["lost"] = lost;
	stats["points"] = points;

	_rootObj["stats"] = stats;
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

bool jsonDoc::Account::verifyPassword(const QString& passformUser, const QString& passfromServer)
{
	if (passformUser == passfromServer)
	{
		return true;
	}
	return false;
}

void jsonDoc::Account::setResult(const bool& result)
{
	JsonDoc::setDataObject("result", result);
}

bool jsonDoc::Account::getResult()
{
	return JsonDoc::getDataObjectBool("result");
}



//Ranking class
jsonDoc::Ranking::Ranking()
{
	_rootObj["action"] = "rankingUpdate";
}

void jsonDoc::Ranking::setRankning(const QJsonDocument& jsonDoc)
{
	QJsonArray jsonArr = jsonDoc.array();
	QJsonArray rankingArr;
	int count = 0;

	foreach(const QJsonValue & value, jsonArr)
	{
		QJsonObject jsonObj = value.toObject();
		QJsonObject jsonObjStats = jsonObj["stats"].toObject();

		QJsonObject arrObj;

		arrObj["lp"] = ++count;
		arrObj["user"] = jsonObj["username"];
		arrObj["win"] = jsonObjStats["win"];
		arrObj["lost"] = jsonObjStats["lost"];

		rankingArr.append(arrObj);
	}

	_rootObj["data"] = rankingArr;
}

QJsonDocument jsonDoc::Ranking::getRanking()
{
	QJsonArray jsonArr = _rootObj["data"].toArray();

	QJsonDocument jsonDoc(jsonArr);

	return jsonDoc;
}



//Matchmaking class
jsonDoc::Matchmaking::Matchmaking() : JsonDoc()
{
	_rootObj["action"] = "";
}

void jsonDoc::Matchmaking::setJoin()
{
	_rootObj["action"] = "matchmakingJoin";
}

void jsonDoc::Matchmaking::setLeave()
{
	_rootObj["action"] = "matchmakingLeave";
}

void jsonDoc::Matchmaking::setUpdateInfo()
{
	_rootObj["action"] = "matchmakingUpdate";
}

void jsonDoc::Matchmaking::setConnId(const QString& connId)
{
	JsonDoc::setDataObject("connId", connId);
}

QString jsonDoc::Matchmaking::getConnId()
{
	return JsonDoc::getDataObjectStr("connId");
}

void jsonDoc::Matchmaking::setPlayerName(const QString& username)
{
	JsonDoc::setDataObject("username", username);
}

QString jsonDoc::Matchmaking::getPlayerName()
{
	return JsonDoc::getDataObjectStr("username");
}

void jsonDoc::Matchmaking::setPlayersInQueueCount(const int& count)
{
	JsonDoc::setDataObject("count", count);
}

int jsonDoc::Matchmaking::getPlayersInQueueCount()
{
	return JsonDoc::getDataObjectStr("count").toInt();
}



//Game class
jsonDoc::Game::Game() : JsonDoc()
{
	_rootObj["action"] = "game";
}

void jsonDoc::Game::addPlayers(const QString& player1Name, const QString& player1Id, const QString& player2Name, const QString& player2Id)
{
	QJsonArray players = JsonDoc::getDataObjectArray("players");

	QJsonObject player1;
	player1["playerName"] = player1Name;
	player1["connId"] = player1Id;
	player1["pawn"] = "x";

	QJsonObject player2;
	player2["playerName"] = player2Name;
	player2["connId"] = player2Id;
	player2["pawn"] = "o";

	players.append(player1);
	players.append(player2);

	JsonDoc::setDataObject("players", players);
}

QJsonArray jsonDoc::Game::getGameField()
{
	return JsonDoc::getDataObjectArray("gameField");
}

void jsonDoc::Game::setGameField(const QJsonArray& gameField)
{
	JsonDoc::setDataObject("gameField", gameField);
}

QJsonArray jsonDoc::Game::getPlayers()
{
	return JsonDoc::getDataObjectArray("players");
}

QString jsonDoc::Game::getId()
{
	QJsonObject obj = JsonDoc::getDataObjectJsonObj("_id");

	return obj["$oid"].toString();
}

int jsonDoc::Game::getRoundCount()
{
	return JsonDoc::getDataObjectInt("round");
}

void jsonDoc::Game::incRoundCount()
{
	int round = JsonDoc::getDataObjectInt("round");
	round++;
	JsonDoc::setDataObject("round", round);
}

void jsonDoc::Game::changePlayerTurn()
{
	int turn = JsonDoc::getDataObjectInt("turn");
	int nextTurn;

	if (turn == 1)
	{
		nextTurn = 2;
	}
	else
	{
		nextTurn = 1;
	}

	JsonDoc::setDataObject("turn", nextTurn);
}

QJsonDocument jsonDoc::Game::sendToPlayer(const int& playerNumber)
{
	QJsonObject message, data;
	message["action"] = _rootObj["action"];

	data["gameId"] = getId();
	data["round"] = getRoundCount();
	data["gameField"] = getGameField();
	data["gameTurn"] = getPlayerTurn(playerNumber);

	if (playerNumber == 1)
	{
		data["pawn"] = "x";
	}
	else
	{
		data["pawn"] = "o";
	}

	message["data"] = data;

	QJsonDocument messageDoc(message);

	return messageDoc;
}

void jsonDoc::Game::updateFromPlayer(const QJsonDocument& jsonDoc)
{
	QJsonObject message = jsonDoc.object();

	QJsonObject data = message["data"].toObject();

	QJsonArray gameField = data["gameField"].toArray();
	JsonDoc::setDataObject("gameField", gameField);
}

void jsonDoc::Game::createNewGame()
{
	_rootObj["action"] = "gameInit";

	//set id
	JsonDoc::setDataObject("_id", JsonDoc::genId());

	//starting pawn
	JsonDoc::setDataObject("turn", randomGamePawn());

	//set round
	JsonDoc::setDataObject("round", 1);

	//round end time
	JsonDoc::setDataObject("roundEndTime", setRoundEndTime());

	//players empty array
	QJsonArray players;
	JsonDoc::setDataObject("players", players);

	//gameField
	JsonDoc::setDataObject("gameField", setEmptyGameField());
}

int jsonDoc::Game::randomGamePawn()
{
	int random = QRandomGenerator::global()->bounded(2) + 1;

	return random;
}

QString jsonDoc::Game::setRoundEndTime()
{
	QDateTime currentTime = QDateTime::currentDateTime();

	QDateTime endTime = currentTime.addSecs(16);

	return endTime.toString(Qt::RFC2822Date);
}

QJsonArray jsonDoc::Game::setEmptyGameField()
{
	QJsonArray gameField;

	for (int i = 0; i < 3; ++i)
	{
		QJsonArray row;

		for (int j = 0; j < 3; ++j)
		{
			row.append("n");
		}

		gameField.append(row);
	}

	return gameField;
}

bool jsonDoc::Game::getPlayerTurn(const int& playerNumber)
{
	int turn = JsonDoc::getDataObjectInt("turn");

	if (playerNumber == turn)
	{
		return true;
	}
	else
	{
		return false;
	}
}



//PlayerList class
jsonDoc::PlayerList::PlayerList(const QJsonArray& playerList)
{
	_playerList = playerList;
}

jsonDoc::PlayerList::PlayerList(const QJsonDocument& playerList)
{
	_playerList = playerList.array();
}

QString jsonDoc::PlayerList::getIdPlayer1()
{
	QJsonObject player1 = _playerList.at(0).toObject();
	return player1["connId"].toString();
}

QString jsonDoc::PlayerList::getUsernamePlayer1()
{
	QJsonObject player1 = _playerList.at(0).toObject();
	return player1["username"].toString();
}

QString jsonDoc::PlayerList::getIdPlayer2()
{
	QJsonObject player2 = _playerList.at(1).toObject();
	return player2["connId"].toString();
}

QString jsonDoc::PlayerList::getUsernamePlayer2()
{
	QJsonObject player2 = _playerList.at(1).toObject();
	return player2["username"].toString();
}