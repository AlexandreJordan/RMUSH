#include "entitypnj.hpp"

#include <iostream>
#include "engine.hpp"
#include "constants.hpp"
#include "level.hpp"
#include "tools.hpp"

EntityPnj::EntityPnj() :
    EntityMobile(), showFov(false)
{
}

EntityPnj::~EntityPnj()
{
}

void EntityPnj::initNew(const int& px, const int& py, const int& pid, const int& plevelId, const std::string& pdata)
{

}


void EntityPnj::initLoad()
{

}

//
// Dessin du PNJ et de son FOV si showFov = true
//
void EntityPnj::render()
{
    TCODConsole::root->setChar( x, y, chr );
    TCODConsole::root->setCharForeground( x, y, color );

    if (showFov)
    {
        for (int nx = x - (int)fov; nx <= x + (int)fov; nx++ )
            for ( int ny = y - (int)fov; ny <= y + (int)fov; ny++ )
            {
                if (Tools::getDistance(x, y, nx, ny) <= fov && !Engine::getInstance()->getMap().getCurrentLevel().isWall(nx, ny))
                {
                    TCODColor col = TCODConsole::root->getCharBackground(nx, ny);
                    col = col * 0.5f;
                    TCODConsole::root->setCharBackground(nx, ny, col);
                }
            }
    }
}

//
// Mise à jour du PNJ
// Déplacement du pnj vers le joueur
//
void EntityPnj::update()
{
    if (isDead)
        return;


    if (Engine::getInstance()->getMap().getCurrentLevel().isInFov(x, y))
    {
        //le monstre est dans le FOV du joueur
        //le monstre VOIT le joueur
        //si le monstre perd de vue le joueur, il a "TRACKING_TURNS" de tours pour
        //avancer vers le dernier point vu par le joueur
        moveSpeed = TRACKING_TURNS;
    }
    else
    {
        //le monstre NE VOIT pas le joueur
        //On décrémente son nombre de possibilité de retrouver le joueur
        moveSpeed--;
    }

    //déplacement du monstre vers le joueur si il reste des tours possibles pour le monstre
    if (moveSpeed > 0)
    {
        if (energy >= 100)
        {
            while (energy >= 100)
            {
                //déroulement de l'action
                moveOrAttack(Engine::getInstance()->getPlayer().x, Engine::getInstance()->getPlayer().y);

                //on soustrait 100
                energy -= 100;
            }
        }
        else
        {
            //énergie < 100 : rien ne se passe
            energy += speed;
        }
    }
}

void EntityPnj::rtupdate()
{
}

//
// Déplacement ou attaque du monstre
// @param powner Pointeur vers le monstre
// @param ptargetX Position de la cible
// @param ptargetY Position de la cible
//
void EntityPnj::moveOrAttack(const int& ptargetX, const int& ptargetY)
{
    //delta entre le joueur et le monstre
    int deltaX      = ptargetX - x;
    int deltaY      = ptargetY - y;

    //déplacement positif ou négatif ?
    int stepX       = (deltaX > 0) ? 1 : -1;
    int stepY       = (deltaY > 0) ? 1 : -1;

    //distance entre le joueur et le monstre
    float distance = Tools::getDistance(ptargetX, ptargetY, x, y);

    if (distance >= 2)
    {
        //on normalise le delta pour avoir l'orientation du déplacement en 1 case
        deltaX = (int)round( deltaX / distance );
        deltaY = (int)round( deltaY / distance );

        //
        //déplacement du monstre en direction du joueur
        //

        //si la case la plus directe est accessible, déplacement
        if (Engine::getInstance()->getMap().getCurrentLevel().canWalk(x + deltaX, y + deltaY))
        {
            x = x + deltaX;
            y = y + deltaY;
        }
        //sinon on tente de contourner par les X
        else
            if (Engine::getInstance()->getMap().getCurrentLevel().canWalk(x + deltaX, y))
            {
                x = x + stepX;
            }
            //sinon on tente de contourner par les Y
            else
                if (Engine::getInstance()->getMap().getCurrentLevel().canWalk(x, y + deltaY))
                {
                    y = y + stepY;
                }
    }

    /*else
    {
        //le monstre attaque le joueur
        Engine::getInstance()->getGui().message(TCODColor::red, "%s vous attaque", name.c_str());

        //récupération des dommages de l'arme du monstre
        //random entre min et max de l'arme
        //sinon utilisation d'une valeur pour combat à main nue
        int minDamageWeapon = 0;
        int maxDamageWeapon = 0;

        if (weapon)
        {
            minDamageWeapon = dynamic_cast<Weapon*>(weapon)->minDamage;
            maxDamageWeapon = dynamic_cast<Weapon*>(weapon)->maxDamage;
        }

    //calcul des dégats
    player.takeDamage(Tools::getDamage(strength, dexterity, minDamageWeapon, maxDamageWeapon, player.dexterity, player.defense));
    }*/
}

//
// Calcul et applique les dommages au monstre
//
void EntityPnj::takeDamage(const float& pdamage)
{
    if (pdamage >= life)
    {
        life    = 0;
        isDead  = true;
        chr = '%';
    }

    life = static_cast<int>(life - pdamage);
}

//
// Retourne les données de l'entité sous forme de XML
//
std::string EntityPnj::getDataXml()
{
    std::stringstream resXml;

    resXml << Entity::getDataXml();
    /*life;                             //niveau de vie actuel
    int maxLife;                            //vie maximale
    int defense;                            //point de défense
    int strength;                           //point d'attaque
    int dexterity;                          //point de dextérité
    int stealth;                            //furtivité du joueur

    bool isDead;                            //l'entité est morte
    int moveSpeed;                          //vitesse de déplacement

    float fov;*/
    resXml << "<life>" << life << "</life>" << std::endl;

    return resXml.str();
}