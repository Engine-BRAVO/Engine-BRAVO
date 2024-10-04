#pragma once

class Camera
{
public:
    Camera(int screenWidth, int screenHeight);

    void move(int dx, int dy);
    void setZoom(float zoom);
    float getZoom() const;

    int getX() const;
    int getY() const;

private:
    int x, y;
    float zoom;
    int screenWidth, screenHeight;
};
