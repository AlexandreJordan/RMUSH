#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include "types.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                             ///
///                                            ANIMATION                                                        ///
///                                                                                                             ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Animation
{
public:
    Animation(const int& poriginX, const int& poriginY) : originX(poriginX), originY(poriginY), IsFinished(false) {}
    virtual ~Animation() {}

    virtual void update() = 0;
    virtual void render() = 0;

    bool IsFinished;
private:
    int originX;
    int originY;
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                             ///
///                                         ANIMATIONTHROWING                                                   ///
///                                                                                                             ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//class Point;

class AnimationThrowing : public Animation
{
public:
    AnimationThrowing(const int& poriginX, const int& poriginY, const std::vector<Point>& ppoints, const char& pchr) 
        : Animation(poriginX, poriginY), points_(ppoints), currentX(poriginY), currentY(poriginY), index_(0), chr(pchr)  {}
    virtual ~AnimationThrowing() {}

    virtual void update();
    virtual void render();

private:
    std::vector<Point> points_;
    
    int currentX;
    int currentY;

    int index_;

    char chr;
};

#endif //ANIMATION_HPP