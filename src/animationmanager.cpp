#include "animationmanager.hpp"
#include <algorithm>

using namespace std;

void AnimationManager::update()
{
    for (Animation* animation : animations_)
    {
        if (animation->IsFinished)
            removeAnimation(animation);
        else
            animation->update();
    }
}

void AnimationManager::render()
{
    for (Animation* animation : animations_)
        animation->render();

    std::cout << "fin manager render anim" << std::endl;
}

void AnimationManager::registerAnimation(Animation* panimation)
{
    std::cout << "registerAnimation" << std::endl;
    animations_.push_back(panimation);
}

void AnimationManager::removeAnimation(Animation* panimation)
{
    vector<Animation*>::iterator it;

    it = find(animations_.begin(), animations_.end(), panimation);
    if (it != animations_.end()) {
        animations_.erase(it);
        delete *it;
    }
}
