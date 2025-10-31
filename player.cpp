#include "player.h"

Player::Player(const QString& name, bool isBot)
    : m_name(name), m_isBot(isBot) {}

QString Player::getName() const {
    return m_name;
}

bool Player::isBot() const {
    return m_isBot;
}

QVector<Card>& Player::getHand() {
    return m_hand;
}

QVector<Card>& Player::getBoard() {
    return m_board;
}

bool Player::canAddToHand() const {
    return m_hand.size() < MAX_HAND_SIZE;
}

bool Player::canAddToBoard() const {
    return m_board.size() < MAX_BOARD_SIZE;
}

void Player::addCardToHand(const Card& card) {
    if (canAddToHand()) {
        m_hand.append(card);
    }
}

void Player::playCard(int handIndex) {
    if (handIndex >= 0 && handIndex < m_hand.size() && canAddToBoard()) {
        m_board.append(m_hand[handIndex]);
        m_hand.remove(handIndex);
    }
}

void Player::removeDeadCardsFromBoard() {
    for (int i = m_board.size() - 1; i >= 0; --i) {
        if (m_board[i].isDead()) {
            m_board.remove(i);
        }
    }
}

int Player::getHandSize() const {
    return m_hand.size();
}

int Player::getBoardSize() const {
    return m_board.size();
}
