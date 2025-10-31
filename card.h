#ifndef CARD_H
#define CARD_H

#include <QString>

class Card {
public:
    Card();
    Card(int health, int damage, const QString& name);
    
    int getHealth() const;
    int getDamage() const;
    QString getName() const;
    
    void setHealth(int health);
    void setDamage(int damage);
    void takeDamage(int damage);
    
    bool isDead() const;
    
private:
    int m_health;
    int m_damage;
    QString m_name;
};

#endif // CARD_H
