#include <Health.h>

Health::Health() {
    health = 200;
}

int Health::getHealth() {
    return health;
}

void Health::takeDamage(int fromPlayerId, int fromTeamId, int damage, int critical) {
    if(health == 0) {
        return;
    }

    // @TODO: implement friendly fire
    // @TODO: implement critical

    if(health - damage < 0) {
        health = 0;
        return;
    }

    health -= damage;
}