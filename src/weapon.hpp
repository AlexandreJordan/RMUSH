#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "entity.hpp"

#include "pugixml.hpp"
#include <string>
#include <sstream>

class EntityItem;

class Weapon : public EntityItem
{
public:
    Weapon();
    ~Weapon();

    void render();
    void update();
    void rtupdate();

    void initNew(const int& px, const int& py, const int& pid, const int& plevelId, const std::string& pdata = "");
    void initLoad(const pugi::xml_node& pnode);
    std::string getDataXml();

    void use(EntityPnj* powner);

    int minDamage;
    int maxDamage;
};

#endif
