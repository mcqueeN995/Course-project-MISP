#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <QVector>
#include <QString>

class Player {
public:
    Player(const QString& name, bool isBot = false);
    
    QString getName() const;
    bool isBot() const;
    
    QVector<Card>& getHand();
    QVector<Card>& getBoard();
    
    bool canAddToHand() const;
    bool canAddToBoard() const;
    
    void addCardToHand(const Card& card);
    void playCard(int handIndex);
    void removeDeadCardsFromBoard();
    
    int getHandSize() const;
    int getBoardSize() const;
    
private:
    QString m_name;
    bool m_isBot;
    QVector<Card> m_hand;
    QVector<Card> m_board;
    
    static const int MAX_HAND_SIZE = 10;
    static const int MAX_BOARD_SIZE = 7;
};

#endif // PLAYER_H
