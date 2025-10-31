#include "mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_selectedPlayerCardIndex(-1), m_attackMode(false) {
    
    m_gameEngine = new GameEngine(this);
    
    connect(m_gameEngine, &GameEngine::gameStateChanged, 
            this, &MainWindow::onGameStateChanged);
    connect(m_gameEngine, &GameEngine::gameOverSignal, 
            this, &MainWindow::onGameOver);
    
    setupUI();
    m_gameEngine->startNewGame();
}

MainWindow::~MainWindow() {
    delete m_gameEngine;
}

void MainWindow::setupUI() {
    setWindowTitle("Карточная игра - Курсовая работа");
    setMinimumSize(800, 600);
    
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    
    // Статус игры и кнопки управления
    QHBoxLayout* controlLayout = new QHBoxLayout();
    m_statusLabel = new QLabel("Статус: Ожидание", this);
    m_statusLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    
    m_newGameButton = new QPushButton("Новая игра", this);
    m_endTurnButton = new QPushButton("Закончить ход", this);
    
    connect(m_newGameButton, &QPushButton::clicked, this, &MainWindow::onNewGameClicked);
    connect(m_endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
    
    controlLayout->addWidget(m_statusLabel);
    controlLayout->addStretch();
    controlLayout->addWidget(m_newGameButton);
    controlLayout->addWidget(m_endTurnButton);
    
    m_mainLayout->addLayout(controlLayout);
    
    // Информация о противнике
    m_opponentInfoLabel = new QLabel("Противник (Бот)", this);
    m_opponentInfoLabel->setStyleSheet("font-size: 14px; color: red;");
    m_mainLayout->addWidget(m_opponentInfoLabel);
    
    // Поле противника
    m_opponentBoardWidget = new QWidget(this);
    m_opponentBoardLayout = new QHBoxLayout(m_opponentBoardWidget);
    m_opponentBoardWidget->setStyleSheet("background-color: #ffcccc; border: 2px solid red;");
    m_opponentBoardWidget->setMinimumHeight(120);
    m_mainLayout->addWidget(m_opponentBoardWidget);
    
    m_mainLayout->addSpacing(20);
    
    // Информация о игроке
    m_playerInfoLabel = new QLabel("Игрок (Вы)", this);
    m_playerInfoLabel->setStyleSheet("font-size: 14px; color: blue;");
    m_mainLayout->addWidget(m_playerInfoLabel);
    
    // Поле игрока
    m_playerBoardWidget = new QWidget(this);
    m_playerBoardLayout = new QHBoxLayout(m_playerBoardWidget);
    m_playerBoardWidget->setStyleSheet("background-color: #ccccff; border: 2px solid blue;");
    m_playerBoardWidget->setMinimumHeight(120);
    m_mainLayout->addWidget(m_playerBoardWidget);
    
    m_mainLayout->addSpacing(20);
    
    // Рука игрока
    m_playerHandLabel = new QLabel("Ваша рука:", this);
    m_playerHandLabel->setStyleSheet("font-size: 14px;");
    m_mainLayout->addWidget(m_playerHandLabel);
    
    m_playerHandWidget = new QWidget(this);
    m_playerHandLayout = new QHBoxLayout(m_playerHandWidget);
    m_playerHandWidget->setStyleSheet("background-color: #f0f0f0; border: 2px solid gray;");
    m_playerHandWidget->setMinimumHeight(100);
    m_mainLayout->addWidget(m_playerHandWidget);
}

void MainWindow::updateUI() {
    // Обновляем статус
    m_statusLabel->setText("Статус: " + m_gameEngine->getGameStatus());
    
    Player* player = m_gameEngine->getPlayer();
    Player* opponent = m_gameEngine->getOpponent();
    
    // Обновляем информацию о игроках
    m_playerInfoLabel->setText(QString("Игрок - Рука: %1/10, Стол: %2/7")
                                .arg(player->getHandSize())
                                .arg(player->getBoardSize()));
    
    m_opponentInfoLabel->setText(QString("Бот - Рука: %1/10, Стол: %2/7")
                                 .arg(opponent->getHandSize())
                                 .arg(opponent->getBoardSize()));
    
    // Очищаем старые виджеты
    clearLayout(m_playerHandLayout);
    clearLayout(m_playerBoardLayout);
    clearLayout(m_opponentBoardLayout);
    
    // Отображаем руку игрока
    for (int i = 0; i < player->getHand().size(); ++i) {
        const Card& card = player->getHand()[i];
        QPushButton* cardButton = new QPushButton(
            QString("%1\nHP: %2\nDMG: %3")
            .arg(card.getName())
            .arg(card.getHealth())
            .arg(card.getDamage()),
            this
        );
        cardButton->setProperty("cardIndex", i);
        cardButton->setMinimumSize(80, 80);
        cardButton->setStyleSheet("background-color: #ffffff; border: 1px solid black;");
        connect(cardButton, &QPushButton::clicked, this, &MainWindow::onHandCardClicked);
        m_playerHandLayout->addWidget(cardButton);
    }
    m_playerHandLayout->addStretch();
    
    // Отображаем поле игрока
    for (int i = 0; i < player->getBoard().size(); ++i) {
        const Card& card = player->getBoard()[i];
        QPushButton* cardButton = new QPushButton(
            QString("%1\nHP: %2\nDMG: %3")
            .arg(card.getName())
            .arg(card.getHealth())
            .arg(card.getDamage()),
            this
        );
        cardButton->setProperty("cardIndex", i);
        cardButton->setMinimumSize(80, 100);
        cardButton->setStyleSheet("background-color: #aaaaff; border: 2px solid blue;");
        connect(cardButton, &QPushButton::clicked, this, &MainWindow::onPlayerBoardCardClicked);
        m_playerBoardLayout->addWidget(cardButton);
    }
    m_playerBoardLayout->addStretch();
    
    // Отображаем поле противника
    for (int i = 0; i < opponent->getBoard().size(); ++i) {
        const Card& card = opponent->getBoard()[i];
        QPushButton* cardButton = new QPushButton(
            QString("%1\nHP: %2\nDMG: %3")
            .arg(card.getName())
            .arg(card.getHealth())
            .arg(card.getDamage()),
            this
        );
        cardButton->setProperty("cardIndex", i);
        cardButton->setMinimumSize(80, 100);
        cardButton->setStyleSheet("background-color: #ffaaaa; border: 2px solid red;");
        connect(cardButton, &QPushButton::clicked, this, &MainWindow::onOpponentBoardCardClicked);
        m_opponentBoardLayout->addWidget(cardButton);
    }
    m_opponentBoardLayout->addStretch();
}

void MainWindow::clearLayout(QLayout* layout) {
    if (!layout) return;
    
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void MainWindow::onGameStateChanged() {
    updateUI();
}

void MainWindow::onGameOver(const QString& winner) {
    QMessageBox::information(this, "Игра окончена", winner);
    updateUI();
}

void MainWindow::onNewGameClicked() {
    m_gameEngine->startNewGame();
    m_selectedPlayerCardIndex = -1;
    m_attackMode = false;
    updateUI();
}

void MainWindow::onEndTurnClicked() {
    if (!m_gameEngine->isGameOver() && m_gameEngine->isPlayerTurn()) {
        m_selectedPlayerCardIndex = -1;
        m_attackMode = false;
        m_gameEngine->endTurn();
    }
}

void MainWindow::onHandCardClicked() {
    if (m_gameEngine->isGameOver() || !m_gameEngine->isPlayerTurn()) return;
    
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    int cardIndex = button->property("cardIndex").toInt();
    m_gameEngine->playerPlayCard(cardIndex);
    m_selectedPlayerCardIndex = -1;
    m_attackMode = false;
}

void MainWindow::onPlayerBoardCardClicked() {
    if (m_gameEngine->isGameOver() || !m_gameEngine->isPlayerTurn()) return;
    
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    int cardIndex = button->property("cardIndex").toInt();
    m_selectedPlayerCardIndex = cardIndex;
    m_attackMode = true;
}

void MainWindow::onOpponentBoardCardClicked() {
    if (m_gameEngine->isGameOver() || !m_gameEngine->isPlayerTurn()) return;
    if (!m_attackMode || m_selectedPlayerCardIndex < 0) return;
    
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    int opponentCardIndex = button->property("cardIndex").toInt();
    m_gameEngine->playerAttackCard(m_selectedPlayerCardIndex, opponentCardIndex);
    
    m_selectedPlayerCardIndex = -1;
    m_attackMode = false;
}
