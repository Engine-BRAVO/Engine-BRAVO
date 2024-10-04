#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight)
    : x(0), y(0), zoom(1.0f), screenWidth(screenWidth), screenHeight(screenHeight) {}

void Camera::move(int dx, int dy)
{
    x += dx;
    y += dy;
}

void Camera::setZoom(float zoom)
{
    this->zoom = zoom;
}

float Camera::getZoom() const
{
    return zoom;
}

int Camera::getX() const
{
    return x;
}

int Camera::getY() const
{
    return y;
}