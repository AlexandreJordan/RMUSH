#include "main.hpp"

/**
 * Constructeur
 */
Container::Container(const int& psizeMax) :
	sizeMax(psizeMax)
{
}

/**
 * Destructeur 
 */
Container::~Container()
{
	inventory.clearAndDelete();
}

/**
 * Supprime l'objet de la liste et le détruit
 * @param object Objet à supprimer
 */
void Container::remove(Object* pobject)
{
	//std::cout << "Container::remove" << std::endl;
	//std::cout << "taille inv : " << inventory.size() << std::endl;
	
	inventory.remove(pobject);
	
	//std::cout << "taille inv : " << inventory.size() << std::endl;
	//std::cout << "test d'acces : " << pobject->name << std::endl;
}

/**
 * Ajoute l'objet à la liste
 * @param object Objet à ajouter
 */
bool Container::add(Object* pobject)
{
	if (inventory.size() < 26) {
		inventory.push(pobject);
		return true;
	}
	
	return false;
}
