#pragma once

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "include/DatabaseClient.h"
#include "include/JsonDoc.h"

class GameSessionManager : public QObject
{
	Q_OBJECT

public:
	GameSessionManager(QObject *parent = nullptr);
	~GameSessionManager();

public slots:
	void receiveData(const QString& connId, const QByteArray& data);

 private slots:
	void checkQueue();

signals:
	void sendData(const QString& connId, const QByteArray& data);

protected:
	void sendToAllInQueue(const QByteArray& data);
	QString checkWinner(const QJsonArray& gameField);
	bool checkIfWin(const QJsonArray& gameField, const QString& player);
	void createNewGame();

private:
	QTimer* _matchmakingMatcher;
};
