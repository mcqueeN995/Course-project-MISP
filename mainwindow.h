#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVector>
#include "gameengine.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    
private slots:
    void onGameStateChanged();
    void onGameOver(const QString& winner);
    void onNewGameClicked();
    void onEndTurnClicked();
    void onHandCardClicked();
    void onPlayerBoardCardClicked();
    void onOpponentBoardCardClicked();
    
private:
    void setupUI();
    void updateUI();
    void clearLayout(QLayout* layout);
    
    GameEngine* m_gameEngine;
    
    QWidget* m_centralWidget;
    QVBoxLayout* m_mainLayout;
    
    QLabel* m_statusLabel;
    QPushButton* m_newGameButton;
    QPushButton* m_endTurnButton;
    
    QLabel* m_opponentInfoLabel;
    QHBoxLayout* m_opponentBoardLayout;
    QWidget* m_opponentBoardWidget;
    
    QLabel* m_playerInfoLabel;
    QHBoxLayout* m_playerBoardLayout;
    QWidget* m_playerBoardWidget;
    
    QLabel* m_playerHandLabel;
    QHBoxLayout* m_playerHandLayout;
    QWidget* m_playerHandWidget;
    
    int m_selectedPlayerCardIndex;
    bool m_attackMode;
};

#endif // MAINWINDOW_H
