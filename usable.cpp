#include "main.hpp"

/******************************************************************************
 *                                     USABLE                                 *
 ******************************************************************************/
/**
 * Constructeur 
 */
Usable::Usable(Object* powner) :
	owner(powner)
{
}

/**
 * Utilisation de l'objet (powner) par le porteur (pwearer)
 */
//TODO Supprimer de l'inventaire et supprimer de la liste globale ?? dans le cas d'une arme ??
bool Usable::use(Object* pwearer)
{
	if (owner->usable) {
	}
	return true;
}

/**
 * L'objet est ramassé par le futur porteur si il a la place
 */
bool Usable::pick(Object* pwearer)
{
	if (pwearer->container && pwearer->container->add(owner)) {
		Engine::getInstance()->getObjects().remove(owner);
		return true;
	}
	return false;
}

/**
 * L'objet est laché au sol
 * Suppression de l'inventaire du joueur et ajout dans la liste des objets dans Engine
 * @param pwearer Le porteur de l'objet
 */
void Usable::drop(Object* pwearer)
{
	if (!pwearer->container)
		return;
		
	pwearer->container->getInventory().remove(owner);				//suppression dans l'inventaire du joueur
	Engine::getInstance()->getObjects().insertBefore(owner, 0);		//ajout dans la liste des objets
	owner->posX = pwearer->posX;									//mise à jour de la position de l'objet
	owner->posY = pwearer->posY;									//...
	Engine::getInstance()->getGui()->message(TCODColor::yellow, "Vous avez laché %s", owner->name);
}

/******************************************************************************
 *                                 HEALTHPOTION                               *
 ******************************************************************************/
/**
 * Constructeur
 * @param powner L'objet concerné
 * @param plife Vie restitué lors de l'utilisation
 */
HealthPotion::HealthPotion(Object* powner, const int& plife) :
	Usable(powner), life(plife)
{
}

/**
 * Utilisation de la potion de soin
 * @param pwearer Le porteur de la potion
 */
bool HealthPotion::use(Object* pwearer)
{
	if (pwearer->destructible) {
		//récupération du nombre de point réstauré
		float amountHealed = pwearer->destructible->heal(life);
		
		//si le nombre de point restauré est > 0 on consomme la potion
		if (amountHealed > 0) {
			Engine::getInstance()->getGui()->message(TCODColor::green, "%f points de vie restaurés", amountHealed);
			return Usable::use(pwearer);
		}
	}
	return false;
}

/******************************************************************************
 *                                   CROSSBOW                                 *
 ******************************************************************************/
/**
 * Constructeur
 * @param powner Objet concerné
 * @param prange Portée de l'arme
 * @param pdamage Dommage causé à la cible
 */
CrossBow::CrossBow(Object* powner, const float& prange, const float& pdamage) :
	Usable(powner), range(prange), damage(pdamage)
{
}

/**
 * Utilisation de l'arme, récupérartion du monstre le plus proche et attaque
 * @param pwearer Le porteur de l'objet
 */
bool CrossBow::use(Object* pwearer)
{
	//recherche du plus proche ennemi
	Object* target = Engine::getInstance()->getClosestObject(pwearer->posX, pwearer->posY, range);
	
	if (!target)
	{
		Engine::getInstance()->getGui()->message(TCODColor::yellow, "Pas d'ennemi en vue");
		return false;
	}
	
	Engine::getInstance()->getGui()->message(TCODColor::yellow, "Un carreau de %s touche %s", pwearer->name, target->name);
	target->destructible->takeDamage(damage);
	
	return true;
}
