#include "Physics/World.h"
#include "CircleCollider.h"
#include "Collider.h"
#include "PhysicsEngine.h"
#include "RigidBody.h"
#include "Vector2.h"
#include "box2d/box2d.h"
#include "box2d/collision.h"

World::World() {}

World::World(Vector2 aGravity) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    mGravity = aGravity;
    worldDef.gravity = (b2Vec2){mGravity.x, mGravity.y};
    mWorldID = b2CreateWorld(&worldDef);
}

int World::createWorld(Vector2 aGravity) { return 0; }

World::~World() {

    //   b2DestroyWorld(mWorldID);
}

void World::executeWorldStep(float aStep, int aSubStep) { b2World_Step(mWorldID, aStep, aSubStep); }

void World::resetWorld() {
    b2DestroyWorld(mWorldID);
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){mGravity.x, mGravity.y};
    mWorldID = b2CreateWorld(&worldDef);
}

BodyID World::createBody(BodyProxy& aBodyProxy) {

    b2BodyDef bodyDef = b2DefaultBodyDef();

    switch (aBodyProxy.getBodyType()) {
    case BodyType::STATIC:
        std::cout << "made static" << std::endl;
        bodyDef.type = b2_staticBody;
        break;
    case BodyType::DYNAMIC:
        bodyDef.type = b2_dynamicBody;
        break;
    }

    bodyDef.position = (b2Vec2){aBodyProxy.getPosition().x, aBodyProxy.getPosition().y};
    bodyDef.gravityScale = aBodyProxy.getGravityScale();
    bodyDef.fixedRotation = !aBodyProxy.getCanRotate();
    bodyDef.linearDamping = aBodyProxy.getLinearDamping();
    bodyDef.angularDamping = aBodyProxy.getAngularDamping();
    b2BodyId bodyID = b2CreateBody(mWorldID, &bodyDef);
    std::cout << "BodyID: " << bodyID.world0 << bodyID.index1 << bodyID.revision << std::endl;
    createShape(aBodyProxy, {bodyID.index1, bodyID.revision, bodyID.world0});

    BodyID convertedBodyID = {bodyID.index1, bodyID.revision, bodyID.world0};
    return convertedBodyID;
}

void World::createShape(BodyProxy& aBodyProxy, BodyID aBodyID) {

    b2BodyId bodyID = convertToB2BodyID(aBodyID);

    for (BoxCollider* boxCollider : aBodyProxy.getBoxColliders()) {
        b2Polygon polygon =
            b2MakeOffsetBox(boxCollider->getWidth(), boxCollider->getHeight(),
                            {boxCollider->getTransform().position.x, boxCollider->getTransform().position.y}, 0.0f);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = aBodyProxy.getDensity();
        shapeDef.friction = aBodyProxy.getFriction();
        shapeDef.restitution = aBodyProxy.getRestitution();
        shapeDef.isSensor = boxCollider->isTrigger();

        shapeDef.filter.groupIndex = boxCollider->getFilterCategory();

        for (filterCategory category : boxCollider->getCollideWith()) {
            shapeDef.filter.categoryBits |= category;
        }
        std::cout << "setting filter category: " << boxCollider->getFilterCategory() << std::endl;
        b2CreatePolygonShape(bodyID, &shapeDef, &polygon);
    }

    for (CircleCollider* circleCollider : aBodyProxy.getCircleColliders()) {
        b2Circle circleShape;

        circleShape.radius = circleCollider->getRadius();
        circleShape.center = {circleCollider->getTransform().position.x, circleCollider->getTransform().position.y};
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = aBodyProxy.getDensity();
        shapeDef.friction = aBodyProxy.getFriction();
        shapeDef.restitution = aBodyProxy.getRestitution();

        // shapeDef.filter.groupIndex = circleCollider->getFilterCategory();
        //
        // for (filterCategory category : circleCollider->getCollideWith()) {
        //     shapeDef.filter.categoryBits |= category;
        // }

        //    shapeDef.isSensor = boxCollider->isTrigger();
        b2CreateCircleShape(bodyID, &shapeDef, &circleShape);
    }
}

void World::deleteBody(BodyID aBodyID) {
    b2BodyId bodyID = convertToB2BodyID(aBodyID);
    b2DestroyBody(bodyID);
}

void World::applyLinearForce(std::vector<Vector2> aForce, BodyID aBodyID) {
    b2BodyId bodyID = convertToB2BodyID(aBodyID);
    for (int i = 0; i < aForce.size(); i++) {
        b2Vec2 force = {aForce[i].x, aForce[i].y};
        b2Body_ApplyLinearImpulseToCenter(bodyID, force, true);
    }
}

void World::applyTorque(std::vector<float> aTorque, BodyID aBodyID) {
    b2BodyId bodyID = convertToB2BodyID(aBodyID);
    for (int torqueBufferIndex = 0; torqueBufferIndex < aTorque.size(); torqueBufferIndex++) {
        b2Body_ApplyTorque(bodyID, aTorque[torqueBufferIndex], true);
    }
}
void World::setPosition(Vector2 aPosition, BodyID aBodyID) {
    b2BodyId bodyid = convertToB2BodyID(aBodyID);
    if (bodyid.index1 == 1) {
        std::cout << "box2d pos is: (" << b2Body_GetPosition(bodyid).x << ", " << b2Body_GetPosition(bodyid).y << ")"
                  << std::endl;
    }

    // std::cout << "Setting pos at id: " << bodyid.world0 << bodyid.index1 << bodyid.revision << std::endl;
    b2Body_SetTransform(bodyid, {aPosition.x, aPosition.y}, b2Body_GetRotation(bodyid));
}

Vector2 World::getPosition(BodyID aBodyID) {
    b2BodyId bodyID = convertToB2BodyID(aBodyID);
    Vector2 position = {b2Body_GetPosition(bodyID).x, b2Body_GetPosition(bodyID).y};
    return position;
}

void World::setGravity(Vector2 aGravity) {
    mGravity = aGravity;
    b2World_SetGravity(mWorldID, {aGravity.x, aGravity.y});
}

Vector2 World::getGravity() { return mGravity; }

std::vector<std::pair<int, int>> World::getContactEvents() {
    std::vector<std::pair<int, int>> collisionList;
    b2ContactEvents contactlist = b2World_GetContactEvents(mWorldID);

    for (int i = 0; i < contactlist.beginCount; i++) {
        collisionList.push_back({b2Shape_GetBody(contactlist.beginEvents[i].shapeIdA).index1,
                                 b2Shape_GetBody(contactlist.beginEvents[i].shapeIdB).index1});
    }
    return collisionList;
}

void World::setBodyActivity(bool aState, BodyID aBodyID) {
    b2BodyId bodyID = convertToB2BodyID(aBodyID);
    if (b2Body_IsValid(bodyID)) {
        if (aState) {
            b2Body_Enable(bodyID);
        } else {
            b2Body_Disable(bodyID);
        }
    }
}

void World::updateBodyProperties(BodyProxy& aBodyProxy, BodyID aBodyID) {
    b2BodyId bodyID = convertToB2BodyID(aBodyID);

    b2Body_SetFixedRotation(bodyID, !aBodyProxy.getCanRotate());
    b2Body_SetGravityScale(bodyID, aBodyProxy.getGravityScale());
    b2Body_SetLinearDamping(bodyID, aBodyProxy.getLinearDamping());
    b2Body_SetAngularDamping(bodyID, aBodyProxy.getAngularDamping());
}

void World::updateShapeProperties(BodyProxy& aBodyProxy, BodyID aBodyID) {
    b2BodyId bodyID = convertToB2BodyID(aBodyID);

    b2ShapeId shapeArray[aBodyProxy.getBoxColliders().size()];

    b2Body_GetShapes(bodyID, shapeArray, aBodyProxy.getBoxColliders().size());

    for (int i = 0; i < aBodyProxy.getBoxColliders().size(); i++) {
        b2Shape_SetDensity(shapeArray[i], aBodyProxy.getDensity());
        b2Shape_SetFriction(shapeArray[i], aBodyProxy.getFriction());
        b2Shape_SetRestitution(shapeArray[i], aBodyProxy.getRestitution());

        if (b2Shape_IsSensor(shapeArray[i]) != aBodyProxy.getBoxColliders().at(i)->isTrigger()) {
            b2DestroyShape(shapeArray[i]);
            createShape(aBodyProxy, aBodyID);
        }
        // b2Shape_SetFilter(shapeArray[i], filterCategory::MONSTER);
    }
}

std::vector<std::pair<int, int>> World::getSensorEvents() {
    std::vector<std::pair<int, int>> sensorList;
    b2SensorEvents sensorEvents = b2World_GetSensorEvents(mWorldID);

    for (int i = 0; i < sensorEvents.beginCount; i++) {
        sensorList.push_back({b2Shape_GetBody(sensorEvents.beginEvents[i].sensorShapeId).index1,
                              b2Shape_GetBody(sensorEvents.beginEvents[i].visitorShapeId).index1});
    }

    return sensorList;
}

b2BodyId World::convertToB2BodyID(BodyID aBodyID) {
    b2BodyId bodyID;
    bodyID.index1 = aBodyID.bodyID;
    bodyID.revision = aBodyID.revision;
    bodyID.world0 = aBodyID.world0;
    return bodyID;
}
