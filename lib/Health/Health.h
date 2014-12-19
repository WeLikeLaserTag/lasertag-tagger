class Health {
public:
    Health();
    int getHealth();
    void takeDamage(int fromPlayerId, int fromTeamId, int damage, int critical);
private:
    int health;
};