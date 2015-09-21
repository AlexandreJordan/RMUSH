#include "libtcod.hpp"

class Object;

#include "destructible.hpp"
#include "attacker.hpp"
#include "ai.hpp"
#include "container.hpp"
#include "usable.hpp"
#include "object.hpp"
#include "map.hpp"
#include "engine.hpp"
#include "gui.hpp"

//---> PRIORITE
//TODO gérer les accents

//---> CODE
//TODO utiliser std::string
//TODO retour à la ligne automatique dans les messages
//TODO amélioration du singleton
//TODO amélioration des headers
//TODO définir des couleurs pour chaque type de messages (inventaire, attaque, défense, infos de vue)
//TODO destruction (delete) dans le container.cpp / remove(); ???

//---> GAMEPLAY
//TODO couloir entre les pièces
//TODO gestion de la défense dans destructible
//TODO chargement d'une carte définit dans une fichier texte (level fixe)
//TODO position des escaliers montant et descendant
//TODO changement de niveau
//TODO information sur les items dans l'inventaire
//TODO combat avec arme de jets
//TODO Piège et nuage
//TODO gestion de la furtivité
//TODO sauvegarde / chargement / menu de jeu
//TODO fenêtre d'aide
//TODO FOV RESTRICTIVE
//TODO Torche on
