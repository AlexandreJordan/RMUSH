#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP

#include "animation.hpp"
#include <vector>

class AnimationManager
{
public:
    AnimationManager();
    ~AnimationManager();

    void update();
    void render();

    void registerAnimation(Animation* animation);
    void removeAnimation(Animation* animation);

private:
    std::vector<Animation*> animations_;
};

#endif //ANIMATIONMANAGER_HPP