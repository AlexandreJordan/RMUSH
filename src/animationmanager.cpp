#include "animationmanager.hpp"

AnimationManager()
{
}

~AnimationManager()
{
}

void  AnimationManager::update()
{
    for (Animation* animation : animations_)
    {
        if (animation->IsFinished)
            removeAnimation(animation);
        else
            animation->update();
    }
}

void  AnimationManager::render()
{
    for (Animation* animation : animations_)
        animation->render();
}

void AnimationManager::registerAnimation(Animation* animation)
{
    animations_.push_back(animations_);
}

void  AnimationManager::removeAnimation(Animation* animation)
{
    animations_.remove(animation);
}
