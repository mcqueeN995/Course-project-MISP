#include "card.h"

Card::Card() : m_health(0), m_damage(0), m_name("") {}

Card::Card(int health, int damage, const QString& name)
    : m_health(health), m_damage(damage), m_name(name) {}

int Card::getHealth() const {
    return m_health;
}

int Card::getDamage() const {
    return m_damage;
}

QString Card::getName() const {
    return m_name;
}

void Card::setHealth(int health) {
    m_health = health;
}

void Card::setDamage(int damage) {
    m_damage = damage;
}

void Card::takeDamage(int damage) {
    m_health -= damage;
    if (m_health < 0) {
        m_health = 0;
    }
}

bool Card::isDead() const {
    return m_health <= 0;
}
