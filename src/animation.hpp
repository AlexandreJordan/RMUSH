#ifndef ANIMATION_HPP
#define ANIMATION_HPP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                             ///
///                                            ANIMATION                                                        ///
///                                                                                                             ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Animation
{
public:
    Animation(const int& poriginX, const int& poriginY) : originX(poriginX), originY(poriginY) {}
    virtual ~Animation() {}

    virtual void update() = 0;
    virtual void render() = 0;

private:
    int originX;
    int originY;
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                             ///
///                                         ANIMATIONTHROWING                                                   ///
///                                                                                                             ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AnimationThrowing : public Animation
{
public:
    AnimationThrowing(const int& poriginX, const int& poriginY, const int& ptargetX, const int& ptargetY, const char& pchr) 
        : Animation(poriginX, poriginY), targetX(ptargetX), targetY(ptargetY), chr(pchr)  {}
    ~AnimationThrowing() {}

    void update();
    void render();

private:
    int targetX;
    int targetY;
    char chr;
};

#endif //ANIMATION_HPP