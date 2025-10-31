#include "gameenige.h"
#include <QDebug>

GameEngine::GameEngine(QObject* parent)
    : QObject(parent), m_player(nullptr), m_opponent(nullptr),
      m_isPlayerTurn(true), m_gameOver(false), m_turnCount(0) {
    
    m_cardNames = {"Воин", "Маг", "Лучник", "Рыцарь", "Дракон", 
                   "Гоблин", "Орк", "Эльф", "Гном", "Тролль",
                   "Паладин", "Некромант", "Жрец", "Разбойник", "Шаман"};
    
    m_player = new Player("Игрок", false);
    m_opponent = new Player("Бот", true);
}

void GameEngine::startNewGame() {
    delete m_player;
    delete m_opponent;
    
    m_player = new Player("Игрок", false);
    m_opponent = new Player("Бот", true);
    
    m_gameOver = false;
    m_turnCount = 0;
    
    // Случайно определяем, кто ходит первым
    m_isPlayerTurn = QRandomGenerator::global()->bounded(2) == 0;
    
    // Раздаем начальные карты (по 3 карты каждому)
    for (int i = 0; i < 3; ++i) {
        drawCard(m_player);
        drawCard(m_opponent);
    }
    
    emit gameStateChanged();
    
    // Если первым ходит бот
    if (!m_isPlayerTurn) {
        botTurn();
    }
}

void GameEngine::playerPlayCard(int handIndex) {
    if (m_gameOver || !m_isPlayerTurn) return;
    
    if (handIndex >= 0 && handIndex < m_player->getHandSize()) {
        m_player->playCard(handIndex);
        emit gameStateChanged();
    }
}

void GameEngine::playerAttackCard(int playerCardIndex, int opponentCardIndex) {
    if (m_gameOver || !m_isPlayerTurn) return;
    
    QVector<Card>& playerBoard = m_player->getBoard();
    QVector<Card>& opponentBoard = m_opponent->getBoard();
    
    if (playerCardIndex >= 0 && playerCardIndex < playerBoard.size() &&
        opponentCardIndex >= 0 && opponentCardIndex < opponentBoard.size()) {
        
        Card& attackingCard = playerBoard[playerCardIndex];
        Card& defendingCard = opponentBoard[opponentCardIndex];
        
        int attackDamage = attackingCard.getDamage();
        int counterDamage = defendingCard.getDamage();
        
        attackingCard.takeDamage(counterDamage);
        defendingCard.takeDamage(attackDamage);
        
        m_player->removeDeadCardsFromBoard();
        m_opponent->removeDeadCardsFromBoard();
        
        emit gameStateChanged();
    }
}

void GameEngine::endTurn() {
    if (m_gameOver) return;
    
    m_isPlayerTurn = !m_isPlayerTurn;
    m_turnCount++;
    
    // Добираем карту в начале хода
    if (m_isPlayerTurn) {
        drawCard(m_player);
    } else {
        drawCard(m_opponent);
        emit gameStateChanged();
        botTurn();
    }
    
    checkGameOver();
    emit gameStateChanged();
}

void GameEngine::botTurn() {
    if (m_gameOver) return;
    
    // Простая стратегия бота:
    // 1. Выложить все возможные карты из руки
    while (m_opponent->getHandSize() > 0 && m_opponent->canAddToBoard()) {
        m_opponent->playCard(0);
    }
    
    // 2. Атаковать случайными картами
    QVector<Card>& botBoard = m_opponent->getBoard();
    QVector<Card>& playerBoard = m_player->getBoard();
    
    if (playerBoard.size() > 0 && botBoard.size() > 0) {
        for (int i = 0; i < botBoard.size(); ++i) {
            int targetIndex = QRandomGenerator::global()->bounded(playerBoard.size());
            
            int attackDamage = botBoard[i].getDamage();
            int counterDamage = playerBoard[targetIndex].getDamage();
            
            botBoard[i].takeDamage(counterDamage);
            playerBoard[targetIndex].takeDamage(attackDamage);
        }
        
        m_opponent->removeDeadCardsFromBoard();
        m_player->removeDeadCardsFromBoard();
    }
    
    checkGameOver();
}

void GameEngine::drawCard(Player* player) {
    if (player->canAddToHand()) {
        player->addCardToHand(generateRandomCard());
    }
}

Card GameEngine::generateRandomCard() {
    int health = QRandomGenerator::global()->bounded(1, 8);
    int damage = QRandomGenerator::global()->bounded(1, 6);
    QString name = m_cardNames[QRandomGenerator::global()->bounded(m_cardNames.size())];
    
    return Card(health, damage, name);
}

void GameEngine::checkGameOver() {
    if (m_player->getBoardSize() == 0 && m_player->getHandSize() == 0) {
        m_gameOver = true;
        emit gameOverSignal("Бот победил!");
    } else if (m_opponent->getBoardSize() == 0 && m_opponent->getHandSize() == 0) {
        m_gameOver = true;
        emit gameOverSignal("Вы победили!");
    }
}

Player* GameEngine::getPlayer() {
    return m_player;
}

Player* GameEngine::getOpponent() {
    return m_opponent;
}

bool GameEngine::isPlayerTurn() const {
    return m_isPlayerTurn;
}

QString GameEngine::getGameStatus() const {
    if (m_gameOver) {
        return "Игра окончена";
    }
    return m_isPlayerTurn ? "Ваш ход" : "Ход бота";
}

bool GameEngine::isGameOver() const {
    return m_gameOver;
}
