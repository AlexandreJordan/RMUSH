#ifndef EntityMonster_HPP
#define EntityMonster_HPP

#include "entity.hpp"

class EntityPnj : public EntityMobile
{
public:
    EntityPnj();
    virtual ~EntityPnj();

    virtual void update();
    virtual void rtupdate();
    virtual void render();

    void initNew(const int& px, const int& py, const int& pid, const int& plevelId, const std::string&);
    void initLoad();
    std::string getDataXml();

    void takeDamage(const float& pdamage);

    bool showFov;                                       //affiche ou non le champ de vision

private:
    void moveOrAttack(const int& ptargetX, const int& ptargetY);
};

#endif
