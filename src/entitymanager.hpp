#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <map>
#include <vector>
#include <string>

#include "entity.hpp"
#include "entitypnj.hpp"

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    void initLoad(const pugi::xml_node& ppnjsNode,
                  const pugi::xml_node& pitemsNode,
                  const pugi::xml_node& pfixedItemsNode);

    EntityPnj* getPnj(const int& pid);                                                                  //retourne l'objet suivant son ID
    EntityItem* getItem(const int& pid);                                                                //--
    EntityFixedItem* getFixedItem(const int& pid);                                                      //--

    EntityPnj* getPnj(const int& px, const int& py, const int& plevelId);                               //retourne l'objet fixe suivant sa position
    EntityFixedItem* getFixedItem(const int& px, const int& py, const int& plevelId);                   //--

    std::vector<EntityItem*> getItems(const int& px, const int& py, const int& plevelId);               //retourne la liste des objets suivant leur px, py et le level ID
    std::vector<EntityItem*> getItems(const std::vector<int>& plistId);                                 //--
    std::vector<int> getItemsId(const int& px, const int& py, const int& plevelId);

    std::vector<EntityPnj*> getPnjs(const int& plevelId);                                               //retourne la liste des entités pour un level donné
    std::vector<EntityItem*> getItems(const int& plevelId);                                             //--
    std::vector<EntityFixedItem*> getFixedItems(const int& plevelId);                                   //--
    std::vector<EntityFixedItem*> getFixedItems(const std::vector<int>& plistId);                       //--

    int createPnj(const int& pentityType, const int& px, const int& py, const int& plevelId);           //crée une entité, le stock et retourne son ID
    int createItem(const int& pentityType, const int& px, const int& py, const int& plevelId);          //--
    int createFixedItem(const int& pentityType, const int& px, const int& py, const int& plevelId);     //--

    void loadPnj(const pugi::xml_node& pnode);
    void loadItem(const pugi::xml_node& pnode);
    void loadFixedItem(const pugi::xml_node& pnode);

    std::string getDataXml();                                                                           //retourne le xml complet du gestionnaire d'entité

private:
    int lastId_;                                                                                        //dernier ID donné (lastId_ est déjà utilisé)

    std::map<int, EntityPnj*> pnjs_;                                                                    //les PNJS <ID / objet>
    std::map<int, EntityItem*> items_;                                                                  //les items  <ID / objet>
    std::map<int, EntityFixedItem*> fixedItems_;                                                        //les items fixes <ID / objet>

    int getNewId();                                                                                     //retourne un nouvel ID unique

    std::string getDataXmlPnjs();                                                                       //retourne le xml de l'ensemble de la liste
    std::string getDataXmlItems();                                                                      //--
    std::string getDataXmlFixedItems();                                                                 //--
};

#endif
