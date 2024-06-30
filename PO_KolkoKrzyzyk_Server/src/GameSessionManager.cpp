#include "include/GameSessionManager.h"

GameSessionManager::GameSessionManager(QObject *parent): QObject(parent)
{
	_matchmakingMatcher = new QTimer(this);
	QObject::connect(_matchmakingMatcher, &QTimer::timeout, this, &GameSessionManager::checkQueue);
	_matchmakingMatcher->start(5000);
}

GameSessionManager::~GameSessionManager()
{
}

void GameSessionManager::receiveData(const QString& connId, const QByteArray& data)
{

}

void GameSessionManager::checkQueue()
{
	DatabaseClient mongoDb;

	int playersCount = mongoDb.countPlayersInQueue();

	jsonDoc::Matchmaking matchmakingInfo;
	matchmakingInfo.setUpdateInfo();
	matchmakingInfo.setPlayersInQueueCount(playersCount);

	QByteArray matchmakingInfoByte = jsonDoc::JsonDoc::toBytes(matchmakingInfo.getJsonDoc());
	sendToAllInQueue(matchmakingInfoByte);

    if (playersCount >= 2)
    {
        createNewGame();
    }
}

void GameSessionManager::sendToAllInQueue(const QByteArray& data)
{
	DatabaseClient mongoDb;
	QJsonDocument jsonDoc = mongoDb.getPlayersInQueue();
	QJsonArray players = jsonDoc.array();

	for (const QJsonValue& player : players)
	{
		QJsonObject playerObj = player.toObject();
		QString connId = playerObj["connId"].toString();

		emit sendData(connId, data);
	}
}

QString GameSessionManager::checkWinner(const QJsonArray& gameField)
{
    QString playerX = "x";
    QString playerO = "o";

    if (checkIfWin(gameField, playerO))
    {
        return playerO;
    }
    if (checkIfWin(gameField, playerX))
    {
        return playerO;
    }
    return nullptr;
}

bool GameSessionManager::checkIfWin(const QJsonArray& gameField, const QString& player)
{
    // rows
    for (int i = 0; i < 3; ++i)
    {
        if (gameField[i].toArray()[0].toString() == player &&
            gameField[i].toArray()[1].toString() == player &&
            gameField[i].toArray()[2].toString() == player)
        {
            return true;
        }
    }

    // columns
    for (int j = 0; j < 3; ++j)
    {
        if (gameField[0].toArray()[j].toString() == player &&
            gameField[1].toArray()[j].toString() == player &&
            gameField[2].toArray()[j].toString() == player)
        {
            return true;
        }
    }

    // diagonals
    if (gameField[0].toArray()[0].toString() == player &&
        gameField[1].toArray()[1].toString() == player &&
        gameField[2].toArray()[2].toString() == player)
    {
        return true;
    }
    if (gameField[0].toArray()[2].toString() == player &&
        gameField[1].toArray()[1].toString() == player &&
        gameField[2].toArray()[0].toString() == player)
    {
        return true;
    }

    return false;
}

void GameSessionManager::createNewGame()
{
    DatabaseClient mongoDb;

	QJsonDocument players = mongoDb.getPlayersInQueue();
    jsonDoc::PlayerList playersList(players);

    jsonDoc::Game gameJson;
    gameJson.createNewGame();
    gameJson.addPlayers(playersList.getUsernamePlayer1(), playersList.getIdPlayer1(), playersList.getUsernamePlayer2(), playersList.getIdPlayer2());

    mongoDb.addGame(gameJson.getData());

    mongoDb.removePlayerFromQueue(playersList.getIdPlayer1());
    mongoDb.removePlayerFromQueue(playersList.getIdPlayer2());

    QByteArray data1 = jsonDoc::JsonDoc::toBytes(gameJson.sendToPlayer(1));
    QByteArray data2 = jsonDoc::JsonDoc::toBytes(gameJson.sendToPlayer(2));

    emit sendData(playersList.getIdPlayer1(), data1);
    emit sendData(playersList.getIdPlayer2(), data2);
}
