#include "PlayerBehaviourScript.h"

#include <iostream>

#include "Animation.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "Input.h"
#include "InputStructs.h"
#include "Network/NetworkObject.h"
#include "ParticleEmitter.h"
#include "Sprite.h"
#include "SpriteDef.h"
#include "SpriteDefUtil.h"

void PlayerBehaviourScript::setFlipX(bool aState) {
    if (mGameObject->hasComponent<Animation>()) {
        for (auto animation : mGameObject->getComponents<Animation>()) {
            animation->setFlipX(aState);
        }
    } else if (mGameObject->hasComponent<Sprite>()) {
        for (auto sprite : mGameObject->getComponents<Sprite>()) {
            sprite->setFlipX(aState);
        }
    }
}
void PlayerBehaviourScript::setFlipY(bool aState) {
    if (mGameObject->hasComponent<Animation>()) {
        for (auto animation : mGameObject->getComponents<Animation>()) {
            animation->setFlipY(aState);
        }
    } else if (mGameObject->hasComponent<Sprite>()) {
        for (auto sprite : mGameObject->getComponents<Sprite>()) {
            sprite->setFlipY(aState);
        }
    }
}

ParticleEmitter* emitter = nullptr;

void PlayerBehaviourScript::deactivateAllAnimations() {
    for (auto animation : mGameObject->getComponents<Animation>()) {
        animation->setActive(false);
    }
}

void PlayerBehaviourScript::setAnimationActive(std::string aAnimationTag, bool aState) {
    for (auto animation : mGameObject->getComponents<Animation>()) {
        if (animation->getTag() == aAnimationTag) {
            animation->setActive(aState);
        }
    }
}

void PlayerBehaviourScript::onStart() {}

void PlayerBehaviourScript::handleAnimations() {
    NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
    if (!networkObject) {
        return;
    }
    if (!networkObject->isOwner()) {
        return;
    }
    static Transform previousTransform = this->mGameObject->getTransform();

    // deactivateAllAnimations();
    // setAnimationActive(playerIdleFrontAnimation, true);
    // if (previousTransform.position.x < this->mGameObject->getTransform().position.x) {
    //     deactivateAllAnimations();
    //     setAnimationActive(playerIdleSideAnimation, true);
    //     setFlipX(false);
    // } else if (previousTransform.position.x > this->mGameObject->getTransform().position.x) {
    //     deactivateAllAnimations();
    //     setAnimationActive(playerIdleSideAnimation, true);
    //     setFlipX(true);
    // } else if (previousTransform.position.y < this->mGameObject->getTransform().position.y) {
    //     deactivateAllAnimations();
    //     setAnimationActive(playerIdleBackAnimation, true);
    //     setFlipX(false);
    // } else if (previousTransform.position.y > this->mGameObject->getTransform().position.y) {
    //     deactivateAllAnimations();
    //     setAnimationActive(playerIdleFrontAnimation, true);
    //     setFlipX(false);
    // }

    previousTransform = this->mGameObject->getTransform();
}

void PlayerBehaviourScript::handleMovement() {
    static const float movementSpeed = 50.0f;

    NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
    if (!networkObject) {
        return;
    }
    if (!networkObject->isOwner()) {
        return;
    }

    Input& input = Input::getInstance();

    Transform parentTransform = this->mGameObject->getTransform();

    if (input.GetKey(Key::Key_E)) {
        deactivateAllAnimations();
    }

    if (input.GetKey(Key::Key_R)) {
        parentTransform.rotate(1.0f);
    }
    if (input.GetKey(Key::Key_T)) {
        parentTransform.rotate(-1.0f);
    }

    if (input.GetKey(Key::Key_W)) {
        deactivateAllAnimations();
        setAnimationActive("playerIdleBack", true);
        setFlipX(false);
        parentTransform.position.y -= (movementSpeed * Time::deltaTime);
    }
    if (input.GetKey(Key::Key_A)) {
        deactivateAllAnimations();
        setAnimationActive("playerIdleSide", true);
        setFlipX(true);
        parentTransform.position.x -= (movementSpeed * Time::deltaTime);
    }
    if (input.GetKey(Key::Key_S)) {
        deactivateAllAnimations();
        setAnimationActive("playerIdleFront", true);
        setFlipX(false);
        parentTransform.position.y += (movementSpeed * Time::deltaTime);
    }
    if (input.GetKey(Key::Key_D)) {
        deactivateAllAnimations();
        setAnimationActive("playerIdleSide", true);
        setFlipX(false);
        parentTransform.position.x += (movementSpeed * Time::deltaTime);
    }
    this->mGameObject->setTransform(parentTransform);
}

void PlayerBehaviourScript::hanldeCameraMovement() {
    NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
    if (!networkObject) {
        return;
    }
    if (!networkObject->isOwner()) {
        return;
    }
    Camera& currentCam = EngineBravo::getInstance().getSceneManager().getCurrentScene()->getActiveCamera();

    Transform playerTransform = this->mGameObject->getTransform();

    currentCam.setTransform(playerTransform);
}

void PlayerBehaviourScript::onUpdate() {
    Input& input = Input::getInstance();

    handleMovement();
    handleAnimations();

    hanldeCameraMovement();

    // Camera& currentCam = EngineBravo::getInstance().getSceneManager().getCurrentScene()->getActiveCamera();
    //
    // static bool direction = true;
    //
    // if (direction) {
    //     Transform pos = currentCam.getTransform();
    //
    //     pos.position.x += 0.01f * Time::deltaTime;
    //
    //     currentCam.setTransform(pos);
    // } else {
    //     Transform pos = currentCam.getTransform();
    //
    //     pos.position.x -= 0.01f * Time::deltaTime;
    //
    //     currentCam.setTransform(pos);
    // }
    //
    // if (currentCam.getTransform().position.x > 100) {
    //     direction = false;
    // } else if (currentCam.getTransform().position.x < 40) {
    //     direction = true;
    // }

    // emitter->setActive(false);

    if (input.GetKeyDown(Key::Key_C)) {
        Configuration& config = EngineBravo::getInstance().getConfiguration();
        config.setConfig("render_colliders", !config.getConfig("render_colliders"));
    }

    if (input.GetKeyDown(Key::Key_F)) {
        Configuration& config = EngineBravo::getInstance().getConfiguration();
        config.setConfig("render_fps", !config.getConfig("render_fps"));
    }

    if (emitter != nullptr) {
        static bool emitterMode = false;

        if (input.GetKeyDown(Key::Key_P)) {
            emitter->setActive(!emitter->isActive());
        }

        if (input.GetKeyDown(Key::Key_Space)) {
            emitterMode = !emitterMode;
        }

        if (emitterMode) {
            emitter->setAngle(0, 45);
            emitter->getRelativeTransform().rotation += 1.0f * Time::deltaTime;
        } else {
            emitter->setAngle(0, 360);
            emitter->getRelativeTransform().rotation = 0.0f;
        }
    }
}
