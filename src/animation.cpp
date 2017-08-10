#include "animation.hpp"
#include "constants.hpp"
#include <iostream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
///                                                                          ///
///                         ANIMATIONTHROWING                                ///
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////

void AnimationThrowing::update()
{
    std::cout << "animationthrowing update" << std::endl;
    std::cout << "index : " << index_ << std::endl;
    std::cout << "taille : " << points_.size() << std::endl;

    currentX = points_.at(index_).x;
    currentY = points_.at(index_).y;

    std::cout << "animation update current : " << currentX << ", " << currentY << std::endl;

    index_++;

    //condition de fin de l'animation
    Point &p = points_.at(points_.size() - 1);
    IsFinished = (currentX == p.x && currentY == p.y);
}

void AnimationThrowing::render()
{
    std::cout << "animationthrowing render" << std::endl;
    std::cout << "current : " << currentX << ", " << currentY << std::endl;

    TCODConsole::root->setChar(currentX, currentY, chr);
    TCODConsole::root->setCharForeground(currentX, currentY, TCODColor::red);

    std::cout << "fin render anim" << std::endl;
}