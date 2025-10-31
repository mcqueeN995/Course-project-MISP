#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "player.h"
#include <QObject>
#include <QVector>
#include <QString>
#include <QRandomGenerator>

class GameEngine : public QObject {
    Q_OBJECT

public:
    explicit GameEngine(QObject* parent = nullptr);

    void startNewGame();
    void playerPlayCard(int handIndex);
    void playerAttackCard(int playerCardIndex, int opponentCardIndex);
    void endTurn();

    Player* getPlayer();
    Player* getOpponent();

    bool isPlayerTurn() const;
    QString getGameStatus() const;
    bool isGameOver() const;

    signals:
        void gameStateChanged();
    void gameOverSignal(const QString& winner);

private:
    void botTurn();
    void drawCard(Player* player);
    Card generateRandomCard();
    void checkGameOver();

    Player* m_player;
    Player* m_opponent;
    bool m_isPlayerTurn;
    bool m_gameOver;
    int m_turnCount;

    QVector<QString> m_cardNames;
};

#endif // GAMEENGINE_H
