#ifndef WORLD_H
#define WORLD_H

#include "Physics/BodyProxy.h"
#include "Vector2.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include <vector>
class World {
public:
    World();

    void executeWorldStep(float step, int subStep);

    void reset();

    int createWorld(Vector2 aGravity);
    int createBody(BodyProxy& aBodyProxy);
    void updateBody(int aBodyID, BodyProxy& aBodyProxy);
    void applyForce(int aBodyID, Vector2 aForce);

    void setPosition(int BodyID, Vector2 aPosition);
    Vector2 getPosition(int aBodyID);

    void setGravity(Vector2 aGravity);
    Vector2 getGravity();

    std::vector<int> getContactEvents();

private:
    b2WorldId mWorldID;
    Vector2 mGravity;
};

#endif // WORLD_H
