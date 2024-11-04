#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>

#include "Sprite.h"

class Animation : public Component {
public:
    Animation(std::vector<Sprite*> aAnimationFrames, int aTimeBetweenFrames, bool aIsLooping = false);
    ~Animation();

    Transform getTransform();
    void setTransform(Transform aNewTransform);

    Sprite* getFrame(int aFrameIndex);
    Sprite* getFrameAtTime(long aTime);

    int getTimeBetweenFrames();
    void setTimeBetweenFrames(int aTimeBetweenFrames);

    void setFlipX(bool state);
    bool getFlipX();

    void setFlipY(bool state);
    bool getFlipY();

    bool isLooping();
    void setIsLooping(bool aIsLooping);

    int getFrameCount();

private:
    Transform mTransform;

    bool mFlipX;
    bool mFlipY;

    std::vector<Sprite*> mAnimationFrames;
    int mTimeBetweenFrames;
    bool mIsLooping;
};

#endif
