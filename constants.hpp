/*******************************************************************************
 * RMUSH - RogueLike
 * 
 * Auteur 	: Alexandre JORDAN
 * Date		: 18/08/2015
 * 
 * Constantes de paramètres du jeu
 ******************************************************************************/
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

//window
static const int WINDOW_WIDTH 			= 120;
static const int WINDOW_HEIGHT 			= 90;

//infos
static const int INFOS_POS_X 			= 1;
static const int INFOS_POS_Y 			= 75;
static const int INFOS_WIDTH 			= 120;
static const int INFOS_HEIGHT 			= 15;
static const int MESSAGES_LINES			= 12;
static const int MESSAGES_POS_X			= 40;

//carte
static const int MAP_WIDTH 				= WINDOW_WIDTH;
static const int MAP_HEIGHT 			= WINDOW_HEIGHT - INFOS_HEIGHT - 1;

//pièces
static const int ROOM_MIN_SIZE 			= 6;
static const int ROOM_MAX_SIZE 			= 14;
static const TCODColor C_DARKWALL		= TCODColor(0, 0, 100);
static const TCODColor C_DARKGROUND		= TCODColor(50, 50, 150);
static const TCODColor C_LIGHTWALL		= TCODColor(130, 110, 50);
static const TCODColor C_LIGHTGROUND	= TCODColor(200, 180, 50);

//gameplay
static const int TRACKING_TURNS			= 3;
static const int MAX_ROOM_MONSTERS		= 3;
static const int MAX_ROOM_ITEMS			= 5;

//inventaire
static const int INVENTORY_WIDTH		= 70;
static const int INVENTORY_HEIGHT		= 32;

//equipement
static const int EQUIPMENT_WIDTH		= 70;
static const int EQUIPMENT_HEIGHT		= 35;

//console de dev
static const int DEV_CONSOLE_WIDTH		= 60;
static const int DEV_CONSOLE_HEIGHT		= 50;

//couleurs des messages
static const TCODColor C_MESS_INFO		= TCODColor::white;
static const TCODColor C_MESS_HIT		= TCODColor::green;
static const TCODColor C_MESS_DAMAGE	= TCODColor::red;
static const TCODColor C_CHAR_STAIR		= TCODColor::white;

//couleurs des entity
static const TCODColor C_PLAYER			= TCODColor::white;
static const TCODColor C_MONSTER		= TCODColor::orange;

#endif