#ifndef stairs_HPP
#define stairs_HPP

#include "entity.hpp"
#include "pugixml.hpp"
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                     ///
///                     STAIRUP                             ///
///                                                     ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class StairUp : public EntityFixedItem
{
public:
    StairUp();
    ~StairUp();

    void render();
    void update();
    void rtupdate();

    void initNew(const int& px, const int& py, const int& pid, const int& plevelId, Entity* pitemLink = nullptr);
    void initLoad(const pugi::xml_node& pnode);
    std::string getDataXml();

    void use(EntityPnj* powner);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                     ///
///                        STAIRDOWN                            ///
///                                                     ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class StairDown : public EntityFixedItem
{
public:
    StairDown();
    ~StairDown();

    void render();
    void update();
    void rtupdate();

    void initNew(const int& px, const int& py, const int& pid, const int& plevelId, Entity* pitemLink = nullptr);
    void initLoad(const pugi::xml_node& pnode);
    std::string getDataXml();

    void use(EntityPnj* powner);
};

#endif
