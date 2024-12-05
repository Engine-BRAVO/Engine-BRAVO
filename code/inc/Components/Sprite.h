#pragma once

#include <string>

#include "Color.h"
#include "Component.h"
#include "Rect.h"
#include "Texture.h"
#include "Transform.h"

class Sprite : public Component
{
public:
	Sprite(Texture* aTexture, int aWidth, int aHeight, Rect aSourceRect = Rect(), std::string aTag = "defaultSprite");
	~Sprite();

	// Rule of Five
	Sprite(const Sprite& other);				// Copy constructor
	Sprite& operator=(const Sprite& other);		// Copy assignment operator
	Sprite(Sprite&& other) noexcept;			// Move constructor
	Sprite& operator=(Sprite&& other) noexcept; // Move assignment operator

	// Override the clone method
	std::unique_ptr<Component> clone() const override;

	Texture* getTexture() const;

	Transform getRelativePosition() const;
	void setRelativePosition(Transform aNewTransform);

	Rect getSource() const;
	void setSource(Rect aSourceRect);

	Color getColorFilter() const;
	void setColorFilter(Color aColor);

	void setFlipX(bool state) { mFlipX = state; }

	bool getFlipX() const { return mFlipX; }

	void setFlipY(bool state) { mFlipY = state; }

	bool getFlipY() const { return mFlipY; }

	void setWidth(int aWidth) { mWidth = aWidth; }

	int getWidth() const { return mWidth; }

	void setHeight(int aHeight) { mHeight = aHeight; }

	int getHeight() const { return mHeight; }

	void setLayer(int aLayer) { mLayer = aLayer; }

	int getLayer() { return mLayer; }

private:
	Transform mTransform;

	Texture* mTexture = nullptr;
	Rect mSourceRect;
	Color mColorFilter;

	std::string mSprite;
	float mWidth = 0;
	float mHeight = 0;
	bool mFlipX;
	bool mFlipY;

	int mLayer = 0;
};
