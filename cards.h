#ifndef CARDS_H
#define CARDS_H
#include <string>


class card {
    protected:
        std::string name;
        std::string type;
        int health;
        int damage;
        int mana;
    public:
        card() : name(""), type(""), health(0), damage(0), mana(0) {}
        virtual  attack(const card& enemy) = 0;
        virtual ~card() {}
};

class skeleton : public card {
    public:
    skeleton();
    int attack(const card& enemy) override;
};



#endif //CARDS_H
