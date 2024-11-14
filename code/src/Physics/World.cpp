#include "Physics/World.h"
#include "SDL.h"
#include "Vector2.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include <utility>
#include <vector>

World::World() {}

int World::createWorld(Vector2 aGravity) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){aGravity.x, aGravity.y};
    mWorldID = b2CreateWorld(&worldDef);
    return 0;
}

void World::executeWorldStep(float aStep, int aSubStep) { b2World_Step(mWorldID, aStep, aSubStep); }

void World::resetWorld() { b2DestroyWorld(mWorldID); }

int World::createBody(BodyProxy& aBodyProxy) {
    std::cout << "World::createBody()" << std::endl;

    b2BodyDef bodyDef = b2DefaultBodyDef();

    switch (aBodyProxy.getBodyType()) {
    case BodyType::STATIC:

        bodyDef.type = b2_staticBody;

        break;
    case BodyType::DYNAMIC:
        std::cout << "create dynamicbody" << std::endl;
        bodyDef.type = b2_dynamicBody;
        bodyDef.linearDamping = 0.8f;
        bodyDef.angularDamping = 0.8f;
        break;
    }

    bodyDef.position = (b2Vec2){aBodyProxy.getPosition().x, aBodyProxy.getPosition().y};
    bodyDef.gravityScale = aBodyProxy.getGravityScale();
    bodyDef.fixedRotation = !aBodyProxy.getCanRotate();
    bodyDef.linearDamping = aBodyProxy.getLinearDamping();
    bodyDef.angularDamping = aBodyProxy.getAngularDamping();
    b2BodyId bodyID = b2CreateBody(mWorldID, &bodyDef);

    std::cout << "BodyID index: " << bodyID.index1 << std::endl;
    std::cout << "BodyID revision: " << bodyID.revision << std::endl;
    std::cout << "BodyID world0: " << bodyID.world0 << std::endl;
    std::cout << "creating body at: (" << aBodyProxy.getPosition().x << ", " << aBodyProxy.getPosition().y << ")"
              << std::endl;

    for (BoxCollider* boxCollider : aBodyProxy.getBoxColliders()) {
        b2Polygon polygon = b2MakeBox(boxCollider->getWidth(), boxCollider->getHeight());

        std::cout << "BoxCollider width: " << boxCollider->getWidth() << std::endl;
        std::cout << "BoxCollider height: " << boxCollider->getHeight() << std::endl;

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = aBodyProxy.getDensity();
        shapeDef.friction = aBodyProxy.getFriction();
        shapeDef.restitution = aBodyProxy.getRestitution();

        b2CreatePolygonShape(bodyID, &shapeDef, &polygon);
        b2Body_EnableSleep(bodyID, false);

        std::cout << "creating box collider at: (" << boxCollider->getTransform().position.x << ", "
                  << boxCollider->getTransform().position.y << ")" << std::endl;
    }

    // b2BodyDef bodyDef;
    // b2BodyId bodyId;
    // b2Polygon dynamicBox;
    // b2ShapeDef shapeDef;
    //
    // bodyDef = b2DefaultBodyDef();
    // bodyDef.type = b2_dynamicBody;
    // bodyDef.position = (b2Vec2){aBodyProxy.getPosition().x, aBodyProxy.getPosition().y}; // Initial position above
    //                                                                                      // ground
    // bodyDef.fixedRotation = true;
    // bodyDef.linearDamping = 0.1f;
    // bodyDef.angularDamping = 0.1f;
    //
    // bodyId = b2CreateBody(mWorldID, &bodyDef);
    //
    // BoxCollider* boxCollider = aBodyProxy.getBoxColliders().at(0);
    //
    // // Create a dynamic box shape (1 unit x 1 unit)
    // dynamicBox = b2MakeBox(boxCollider->getWidth(), boxCollider->getHeight());
    //
    // shapeDef = b2DefaultShapeDef();
    // shapeDef.density = 1.0f;
    // shapeDef.friction = 0.0f;
    // shapeDef.restitution = 0.0f;
    // b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
    // b2Body_EnableSleep(bodyId, false);
    return bodyID.index1;
}

void World::updateBody(int aBodyID, BodyProxy& aBodyProxy) {
    b2BodyId test = {aBodyID, 0, 1};
    // b2Body_SetTransform(test, {aBodyProxy.getPosition().x, aBodyProxy.getPosition().y}, b2Body_GetRotation(test));
}

void World::applyLinearForce(int aBodyID, std::vector<Vector2> aForce) {
    b2BodyId test = {aBodyID, 0, 1};
    for (int i = 0; i < aForce.size(); i++) {
        b2Vec2 force = {aForce[i].x, aForce[i].y};
        b2Body_ApplyLinearImpulseToCenter(test, force, true);
    }
}

void World::applyTorque(int aBodyID, std::vector<float> aTorque) {
    b2BodyId test = {aBodyID, 0, 1};
    for (int torqueBufferIndex = 0; torqueBufferIndex < aTorque.size(); torqueBufferIndex++) {
        b2Body_ApplyTorque(test, aTorque[torqueBufferIndex], true);
    }
}
void World::setPosition(int aBodyID, Vector2 aPosition) {
    b2BodyId bodyid = {aBodyID, 0, 1};

    b2Body_SetTransform(bodyid, {aPosition.x, aPosition.y}, {cos(0.0f), 0.0f});
}

Vector2 World::getPosition(int aBodyID) {
    b2BodyId test = {aBodyID, 0, 1};
    Vector2 position = {b2Body_GetPosition(test).x, b2Body_GetPosition(test).y};
    return position;
}

void World::setGravity(Vector2 aGravity) {}

Vector2 World::getGravity() { return mGravity; }

std::vector<std::pair<int, int>> World::getContactEvents() {
    std::vector<std::pair<int, int>> collisionList;
    b2ContactEvents contactlist = b2World_GetContactEvents(mWorldID);

    for (int i = 0; i < contactlist.beginCount; i++) {
        collisionList.push_back(
            {contactlist.beginEvents[i].shapeIdA.index1, contactlist.beginEvents[i].shapeIdB.index1});
        std::cout << "begincount is: " << contactlist.beginCount << std::endl;
        std::cout << "contact found: " << std::endl;
        std::cout << "A: " << contactlist.beginEvents[i].shapeIdA.index1 << std::endl;
        std::cout << "B: " << contactlist.beginEvents[i].shapeIdB.index1 << std::endl;
    }
    return collisionList;
}
