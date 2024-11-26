#include "Physics/BodyProxy.h"
#include "BoxCollider.h"
#include <functional>

BodyProxy::BodyProxy(const std::reference_wrapper<GameObject>& aGameObject) {
    if (aGameObject.get().hasComponent<RigidBody>()) {
        RigidBody* rigidBody = aGameObject.get().getComponents<RigidBody>().at(0);

        if (aGameObject.get().hasComponent<BoxCollider>()) {
            mBoxColliders = aGameObject.get().getComponents<BoxCollider>();
        }
        // if (aGameObject->hasComponent<CircleCollider>()) {
        //     mCircleColliders = aGameObject->getComponents<CircleCollider>();
        // }

        mPosition = aGameObject.get().getTransform().position;
        mHasGravity = rigidBody->getHasGravity();
        mIsMoveableByForce = rigidBody->getIsMoveableByForce();
        mCanRotate = rigidBody->getCanRotate();
        mDensity = rigidBody->getDensity();
        mFriction = rigidBody->getFriction();
        mRestitution = rigidBody->getRestitution();
        mMass = rigidBody->getMass();
        mGravityScale = rigidBody->getGravityScale();
        mLinearDamping = rigidBody->getLinearDamping();
        mAngularDamping = rigidBody->getAngularDamping();

        processBodyType();

    } else {
        return;
    }
}

BodyProxy::~BodyProxy() {}

void BodyProxy::processBodyType() {
    if (!mHasGravity && !mIsMoveableByForce && !mCanRotate && mDensity == 0 && mFriction == 0 && mRestitution == 0 &&
        mMass == 0 && mGravityScale == 0) {
        mBodyType = BodyType::STATIC;
    } else {
        mBodyType = BodyType::DYNAMIC;
    }
}

bool BodyProxy::getHasGravity() const { return mHasGravity; }
bool BodyProxy::getIsMoveableByForce() const { return mIsMoveableByForce; }
bool BodyProxy::getCanRotate() const { return mCanRotate; }
BodyType BodyProxy::getBodyType() const { return mBodyType; }

float BodyProxy::getLinearDamping() const { return mLinearDamping; }
float BodyProxy::getAngularDamping() const { return mAngularDamping; }

float BodyProxy::getDensity() const { return mDensity; }
float BodyProxy::getFriction() const { return mFriction; }
float BodyProxy::getRestitution() const { return mRestitution; }

float BodyProxy::getMass() const { return mMass; }
float BodyProxy::getGravityScale() const { return mGravityScale; }

Vector2 BodyProxy::getPosition() const { return mPosition; }
std::vector<BoxCollider*> BodyProxy::getBoxColliders() const { return mBoxColliders; }
std::vector<CircleCollider*> BodyProxy::getCircleColliders() const { return mCircleColliders; }
