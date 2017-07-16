#ifndef EntityPlayer_HPP
#define EntityPlayer_HPP

#include "entity.hpp"
#include "pugixml.hpp"

#include <string>
#include <sstream>
#include <map>
#include <vector>



class EntityPlayer : public EntityPnj
{
public:
    EntityPlayer();
    ~EntityPlayer();

    void update();
    void rtupdate();
    void render();

    void initNew(const int&, const int&, const int&, const int&, const std::string&);
    void initLoad(const pugi::xml_node& pnode);

    void takeDamage(const float& pdamage);                              //calcul et application des dommages sur l'entité

    std::vector<int>& getInventory() {
        return inventory_;
    }

    float lightX;                                                       //position de la lumière de l'entité
    float lightY;                                                       //...
    float lightIntensity;                                               //intensité de la lumière
    bool lightOn;                                                       //lumière allumé ou non, si oui, diminution de la furtivité

    int weapon;                                                         //arme utilisé par l'entité

    int aHeadPart;                                                      //pièce d'armure porté par le joueur
    int aBodyPart;                                                      //...
    int aHandsPart;                                                     //...
    int aLegsPart;                                                      //...
    int aFootsPart;                                                     //...

    std::map<int, int> itemLink;                                        //items liés à la TU
    std::map<int, int> itemFixedLink;                                   //items fixes liés à la TU

    float mutationTreshold;                                             //seuil de mutation >1 une mutation est ajouté au joueur
    bool isFire;                                                        //VRAI si le joueur est en jeu, il perd de PV à chaque tour
    int fireCpt;                                                        //compteur de tour pour la gestion du feu

    std::string getDataXml();

private:
    bool processKey(const int& key);                                    //gestion de la touche appuyées

    bool action(const int& ptargetX, const int& ptargetY);
    bool move(const int& ptargetX, const int& ptargetY);
    bool attack(const int& ptargetX, const int& ptargetY);

    bool addToInventory(const int& pItemId);                            //ajoute l'item dans l'inventaire
    void dropToGround(const int& pitemId);                              //dépose un objet au sol
    void equipItem(const int& pitemId);                                 //tente d'équiper l'item, retourne vrai si réussi
                                                                        //des coordonnées par paramètre
    void showInventoryPanel();                                          //affiche l'inventaire
    void showRemotePanel();                                             //affiche la fenêtre de la télécommande universel

    void fillInventoryDev();

    std::vector<int> inventory_;                                        //liste des items portés par le joueur

    float torchX;
    TCODNoise* noise;
};

#endif
