#include "include/GameManager.h"

GameManager::GameManager(QObject *parent): QObject(parent)
{
}

GameManager::~GameManager()
{
}

void GameManager::getRequest(const QString& connId, const QByteArray& data)
{
	QJsonDocument dataJsonDoc = jsonDoc::JsonDoc::toJson(data);
	QString requestAction = jsonDoc::JsonDoc::getAction(dataJsonDoc);

	DatabaseClient mongoDb;

	qDebug() << "load" << dataJsonDoc.toJson();

	if (requestAction == "login")
	{
		jsonDoc::Account requestAccountJson;
		requestAccountJson.setJson(dataJsonDoc);

		QString passFromClient = requestAccountJson.getPassword();
		QString usernameFromClient = requestAccountJson.getUsername();
		QString passFromDatabase = mongoDb.getUserPassByNick(usernameFromClient);

		bool status = jsonDoc::Account::verifyPassword(passFromClient, passFromDatabase);

		jsonDoc::Account responseAccount;
		responseAccount.setLogin();
		responseAccount.setUsername(usernameFromClient);
		responseAccount.setResult(status);

		QByteArray responseData = jsonDoc::JsonDoc::toBytes(responseAccount.getJsonDoc());

		sendResponse(connId, responseData);
	}
	if (requestAction == "newAccount")
	{
		jsonDoc::Account accountJson;
		accountJson.setJson(dataJsonDoc);
		accountJson.setEmptyStats();

		QJsonDocument newPlayerJson(accountJson.getData());

		bool status = mongoDb.createPlayer(newPlayerJson);

		jsonDoc::Account responseAccount;
		responseAccount.setNewaccount();
		responseAccount.setUsername(accountJson.getUsername());
		responseAccount.setResult(status);

		QByteArray responseData = jsonDoc::JsonDoc::toBytes(responseAccount.getJsonDoc());

		sendResponse(connId, responseData);
	}
	if (requestAction == "rankingUpdate")
	{
		QJsonDocument rankingJsonDoc = mongoDb.getPlayerRanking();
		jsonDoc::Ranking playerRanking;
		playerRanking.setRankning(rankingJsonDoc);

		QByteArray responseData = jsonDoc::JsonDoc::toBytes(playerRanking.getJsonDoc());
		
		sendResponse(connId, responseData);
	}
	if (requestAction == "matchmakingJoin")
	{
		jsonDoc::Matchmaking matchmakingJsonDoc;
		matchmakingJsonDoc.setJson(dataJsonDoc);
		QJsonObject data =  matchmakingJsonDoc.getData();

		mongoDb.addPlayerToQueue(data);

		int playersCount = mongoDb.countPlayersInQueue();
		jsonDoc::Matchmaking matchmakingInfo;
		matchmakingInfo.setUpdateInfo();
		matchmakingInfo.setPlayersInQueueCount(playersCount);

		QByteArray responseData = jsonDoc::JsonDoc::toBytes(matchmakingInfo.getJsonDoc());

		sendResponse(connId, responseData);
	}
	if (requestAction == "matchmakingLeave")
	{
		jsonDoc::Matchmaking matchmakingJsonDoc;
		matchmakingJsonDoc.setJson(dataJsonDoc);
		QJsonObject data = matchmakingJsonDoc.getData();

		mongoDb.removePlayerFromQueue(data);
	}
	if (requestAction == "game")
	{
		QJsonObject message = dataJsonDoc.object();
		QJsonObject dataFromUser = message["data"].toObject();

		QString gameId = dataFromUser["gameId"].toString();

		QJsonObject data = mongoDb.getGame(gameId);
		jsonDoc::Game gameJson;

		gameJson.setData(data);
		gameJson.setGameField(dataFromUser["gameField"].toArray());
		gameJson.changePlayerTurn();
		gameJson.incRoundCount();

		mongoDb.updateGame(gameId, gameJson.getData());

		jsonDoc::PlayerList players(gameJson.getPlayers());

		QString player1 = players.getIdPlayer1();
		QString player2 = players.getIdPlayer2();

		QByteArray responseData1 = jsonDoc::JsonDoc::toBytes(gameJson.sendToPlayer(1));
		QByteArray responseData2 = jsonDoc::JsonDoc::toBytes(gameJson.sendToPlayer(2));

		qDebug() << responseData1;
		qDebug() << responseData2;

		sendResponse(player1, responseData1);
		sendResponse(player2, responseData2);
	}
}
