#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP

#include "animation.hpp"
#include <vector>
#include <iostream>

class AnimationManager
{
public:
    AnimationManager() {}
    ~AnimationManager() {}

    void update();
    void render();

    void registerAnimation(Animation* animation);
    void removeAnimation(Animation* animation);

    bool isEmpty() { 
        //std::cout << "taille animation : " << animations_.size() << std::endl;
        return (animations_.size() <= 0); }

private:
    std::vector<Animation*> animations_;
};

#endif //ANIMATIONMANAGER_HPP