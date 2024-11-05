#include "RenderSystem.h"

#include <iostream>
#include <memory>

#include "Animation.h"
#include "Color.h"
#include "ParticleEmitter.h"
#include "SDL_timer.h"
#include "ScopedTimer.h"
#include "Sprite.h"
#include "Time.h"

RenderSystem::RenderSystem() : WindowWidth(800), WindowHeight(480) {
    mWindow = std::make_unique<Window>(WindowWidth, WindowHeight);
    mRenderer = std::make_unique<Renderer>(*mWindow);

    mBackgroundColor = Color(255, 255, 255);

    return;
}

void RenderSystem::renderSprite(Camera& aCurrentCamera, GameObject* aGameObject, Sprite* aSprite) {
    int spriteWidth = aSprite->getWidth();
    int spriteHeight = aSprite->getHeight();

    int windowWidth = mWindow->getSize().x;
    int windowHeight = mWindow->getSize().y;

    Vector2 texturePosition = aGameObject->getTransform().position + aSprite->getRelativePosition().position;

    // Calculate the camera's origin and position the sprite relative to it
    Vector2 cameraOrigin =
        aCurrentCamera.getTransform().position - Vector2(static_cast<int>(aCurrentCamera.getWidth() / 2.0f),
                                                         static_cast<int>(aCurrentCamera.getHeight() / 2.0f));

    Vector2 drawPosition = texturePosition - cameraOrigin;

    // Snap position to integer to avoid subpixel offsets
    drawPosition.x = std::round(drawPosition.x * (static_cast<float>(windowWidth) / aCurrentCamera.getWidth()));
    drawPosition.y = std::round(drawPosition.y * (static_cast<float>(windowHeight) / aCurrentCamera.getHeight()));

    // Adjust the width and height slightly to cover gaps
    spriteWidth = std::round(spriteWidth * (static_cast<float>(windowWidth) / aCurrentCamera.getWidth())) + 1; // +1 to
                                                                                                               // cover
                                                                                                               // gaps
    spriteHeight =
        std::round(spriteHeight * (static_cast<float>(windowHeight) / aCurrentCamera.getHeight())) + 1; // +1 to
                                                                                                        // cover
                                                                                                        // gaps

    // Render the sprite with adjusted size
    mRenderer->renderTexture(*aSprite->getTexture(), aSprite->getSource(), drawPosition, spriteWidth, spriteHeight,
                             aSprite->getFlipX(), aSprite->getFlipY(),
                             aGameObject->getTransform().rotation + aSprite->getRelativePosition().rotation);
}

void RenderSystem::renderAnimation(Camera& aCurrentCamera, GameObject* aGameObject, Animation* aAnimation) {
    Sprite* currentFrame = aAnimation->getFrameAtTime(SDL_GetTicks());
    // Sprite* currentFrame = aAnimation->getFrame(0);

    renderSprite(aCurrentCamera, aGameObject, currentFrame);
}

void RenderSystem::renderParticle(Camera& aCurrentCamera, Particle& aParticle) {
    float particleWidth = aParticle.getSize().x;
    float particleHeight = aParticle.getSize().y;

    int WindowWidth = mWindow->getSize().x;
    int WindowHeight = mWindow->getSize().y;

    Vector2 particlePosition = aParticle.getPosition();

    Vector2 cameraOrigin = aCurrentCamera.getTransform().position -
                           Vector2(aCurrentCamera.getWidth() / 2.0f, aCurrentCamera.getHeight() / 2.0f);

    Vector2 drawPosition = particlePosition - cameraOrigin;

    drawPosition.x = drawPosition.x * (static_cast<float>(WindowWidth) / aCurrentCamera.getWidth());
    drawPosition.y = drawPosition.y * (static_cast<float>(WindowHeight) / aCurrentCamera.getHeight());

    particleWidth = static_cast<int>(static_cast<float>(particleWidth) *
                                     (static_cast<float>(WindowWidth) / static_cast<float>(aCurrentCamera.getWidth())));
    particleHeight =
        static_cast<int>(static_cast<float>(particleHeight) *
                         (static_cast<float>(WindowHeight) / static_cast<float>(aCurrentCamera.getHeight())));

    if (aParticle.getRotation() == 0) {
        mRenderer->renderSquare(drawPosition, static_cast<int>(particleWidth), static_cast<int>(particleHeight),
                                aParticle.getColor(), true);
    } else {
        mRenderer->renderSquare(drawPosition, static_cast<int>(particleWidth), static_cast<int>(particleHeight),
                                aParticle.getRotation(), aParticle.getColor(), true);
    }
}

int RenderSystem::getLowestLayer(Scene* aScene) {
    int lowestLayer = 0;
    for (auto& gameObject : aScene->getGameObjects()) {
        if (gameObject->hasComponent<Sprite>()) {
            for (auto sprite : gameObject->getComponents<Sprite>()) {
                if (sprite->getLayer() < lowestLayer) {
                    lowestLayer = sprite->getLayer();
                }
            }
        }
        if (gameObject->hasComponent<Animation>()) {
            for (auto animation : gameObject->getComponents<Animation>()) {
                if (animation->getLayer() < lowestLayer) {
                    lowestLayer = animation->getLayer();
                }
            }
        }
        if (gameObject->hasComponent<ParticleEmitter>()) {
            for (auto particleEmitter : gameObject->getComponents<ParticleEmitter>()) {
                if (particleEmitter->getLayer() < lowestLayer) {
                    lowestLayer = particleEmitter->getLayer();
                }
            }
        }
    }
    return lowestLayer;
}

int RenderSystem::getHighestLayer(Scene* aScene) {
    int highestLayer = 0;
    for (auto& gameObject : aScene->getGameObjects()) {
        if (gameObject->hasComponent<Sprite>()) {
            for (auto sprite : gameObject->getComponents<Sprite>()) {
                if (sprite->getLayer() > highestLayer) {
                    highestLayer = sprite->getLayer();
                }
            }
        }
        if (gameObject->hasComponent<Animation>()) {
            for (auto animation : gameObject->getComponents<Animation>()) {
                if (animation->getLayer() > highestLayer) {
                    highestLayer = animation->getLayer();
                }
            }
        }
        if (gameObject->hasComponent<ParticleEmitter>()) {
            for (auto particleEmitter : gameObject->getComponents<ParticleEmitter>()) {
                if (particleEmitter->getLayer() > highestLayer) {
                    highestLayer = particleEmitter->getLayer();
                }
            }
        }
    }
    return highestLayer;
}

void RenderSystem::renderLayer(Scene* aScene, int aLayer) {
    Camera& activeCamera = aScene->getActiveCamera();

    for (auto& gameObject : aScene->getGameObjects()) {
        if (gameObject->hasComponent<Animation>()) {
            for (auto animation : gameObject->getComponents<Animation>()) {
                if (animation->isActive() && animation->getLayer() == aLayer) {
                    renderAnimation(activeCamera, gameObject, animation);
                }
            }
        } else if (gameObject->hasComponent<Sprite>()) {
            for (auto sprite : gameObject->getComponents<Sprite>()) {
                if (sprite->isActive() && sprite->getLayer() == aLayer) {
                    renderSprite(activeCamera, gameObject, sprite);
                }
            }
        }
        if (gameObject->hasComponent<ParticleEmitter>()) {
            for (auto particleEmitter : gameObject->getComponents<ParticleEmitter>()) {
                if (particleEmitter->isActive() && particleEmitter->getLayer() == aLayer) {
                    for (auto& particle : particleEmitter->getParticles()) {
                        renderParticle(activeCamera, particle);
                    }
                }
            }
        }
    }
}

void RenderSystem::render(Scene* aScene) {
    ScopedTimer scopedTimer("RenderSystem::render");
    mRenderer->clear(mBackgroundColor);

    int lowestLayer = getLowestLayer(aScene);
    int highestLayer = getHighestLayer(aScene);

    for (int layer = lowestLayer; layer <= highestLayer; ++layer) {
        renderLayer(aScene, layer);
    }

    renderDeubgInfo(aScene);

    mRenderer->show();
}

void RenderSystem::renderDeubgInfo(Scene* aScene) {
    if (Time::deltaTime == 0) {
        return;
    }
    int fps = 1.0f / Time::deltaTime;

    // Render FPS counter in the top left corner of the screen with black text color (0, 0, 0)
    mRenderer->renderText("FPS: " + std::to_string(fps), Vector2(10, 10), Color(0, 255, 0));
}

Renderer& RenderSystem::getRenderer() { return *mRenderer; }
Window& RenderSystem::getWindow() { return *mWindow; }
