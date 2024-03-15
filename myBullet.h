#ifndef MYBULLET_H
#define MYBULLET_H

#include <SFML/Graphics.hpp>

using namespace sf;

class myBullet{

public:
    myBullet(const Vector2f& initPos);
    void draw(RenderWindow& window);
    void updateBullet();
    bool isBulletActive();
    Sprite& getBulletSprite();

private:
    Sprite bulletSprite;
    Texture bulletTexture;
    float bulletSpeed;
    bool bbulletActive;
};

#endif
