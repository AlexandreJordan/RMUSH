#ifndef MAIN_HPP
#define MAIN_HPP

class Entity;

#include "libtcod.hpp"
#include "engine.hpp"

#endif

///////////////////////////////////////////////////////////////////////////////
// TODO LIST
//
//---> AMELIORATION
//TODO retour à la ligne automatique dans les messages
//TODO paufiner la gestion du temps pour les actions.
//TODO séparer le déplacement et l'action : augmenter la base pour la gestion du temps du joueur
//TODO gestion des portes
//TODO Dégat lors de la fermeture d'une porte avec le joueur ou pnj sur la case
//TODO Gestion des accents (problème d'encodage)


//---> ENGINE


//---> PNJS
//TODO un monstre par case


//---> ITEMS
//TODO Pouvoir lié un EntityItem au sol sur la même case que le joueur (pas obligé de le ramasser)


//---> DESIGN / RENDU
//TODO animation des cases avec les couleurs
//TODO rougir la zone ou un monstre est mort ou toucher
//TODO indiquer l'état de santé des pnjs avec la couleur de leur caractère


//---> COMBATS
//TODO calcul des dégats


//---> COMPORTEMENT
//TODO gestion de la furtivité
//TODO gestion du feu avec propagation
//TODO déplacement automatique : choisir une case et afficher le tracé du meilleur parcours
//TODO gestion et fenêtre des mutations


//---> MAP


//---> BUGS
//BUG DeleteItems / FixedItems : supprimer les liens avec les items liés


///////////////////////////////////////////////////////////////////////////////
// MUTATIONS
//
//  Mutation :
//      Nom
//      Liste de propriétés appliquées sur le joueur
//      
//  ProcessModificationPnj(EntityMobile*, )
//
// Mutation ++
//  Augmente la vitesse de déplacement
//  Augmente la vitesse d'action
//  Augmente la force
//  Augmente la dextérité
//  Augmente la CA
//
// Mutation --
//  Réduit la vitesse de déplacement
//  Réduit la vitesse d'action
//  Réduit la force
//  Réduit la dextérité
//  Réduit la CA

///////////////////////////////////////////////////////////////////////////////
// ANIMATION TEMP REEL
//
//  ThrowingAnimation
//  ExplosionAnimation


///////////////////////////////////////////////////////////////////////////////
// COMBAT
//
// Attaquant                Defenseur
//  - Force                 - Armure
//  - Arme                  - Dexterité
//  - Dexterité


///////////////////////////////////////////////////////////////////////////////
// JOUEUR
//
//Vie           : Vie du joueur
//Dextérité     :
//Défense       : Défense du joueur
//                Les pièces d'armures fournissent des points de défenses supplémentaires
//Furtivité     : Détermine la distance à laquelle les ennemis voit le joueur


///////////////////////////////////////////////////////////////////////////////
// BESTIAIRE
//
// Homme mutant
// Chien


///////////////////////////////////////////////////////////////////////////////
// INVENTAIRE
//
//Liste des items portés par le joueur (au nombre de 10)
//Description de l'item sélectionné
//Liste des actions possibles sur l'item sélectionné
//Affichage de l'équipement du joueur


///////////////////////////////////////////////////////////////////////////////
// TELECOMMANDE UNIVERSELLE
//
//Liste des déclencheurs d'items (au nombre de 4)
//Possibilité d'affecter un déclencheur à un item de l'inventaire
//Liste des déclencheurs d'items fixes (au nombre de 2)
//Possibilité d'affecter un déclencheur à un item fixe proche du joueur


///////////////////////////////////////////////////////////////////////////////
// BACKGROUND
//
//But du jeu    :   Descendre et détruire la source des mutations. Celle-ci sont générées par le gaz
//                  produit par les champignons des caves.
//
//Mutations     :   Elles affectent les PNJs ainsi que le joueur en modifiant ses caractéristiques
//                  physiques ou psychiques. Elles peuvent être bénéfiques ou bien engendrer un malus.


///////////////////////////////////////////////////////////////////////////////
// GUI
//
// Vie : x / m        |#####---|
// Bruit              |##------|
// Mutations : x      For. : x  
// Arme      : x      Dex. : x
// Etage  : x      Déf. : x
