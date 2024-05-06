#include "myBullet.h"

using namespace sf;

myBullet::myBullet(const Vector2f& initPos) : bbulletActive(true), bulletSpeed(1.0f){

    if(!bulletTexture.loadFromFile("pic_myBullet.png")){
        bbulletActive = false;
        return;
    }
    bulletTexture.setSmooth(false);
    bulletSprite.setTexture(bulletTexture);
    FloatRect bounds = bulletSprite.getLocalBounds();
    bulletSprite.setOrigin(bounds.width / 2, bounds.height / 2);
    bulletSprite.setPosition(initPos);
    bulletSprite.setScale(0.1f, 0.1f);
}

Sprite& myBullet::getBulletSprite() {
    return bulletSprite;
}


//총알 그리기
void myBullet::draw(RenderWindow& window){
   
    if(bbulletActive){
        window.draw(bulletSprite);
    }

}

//총알 업데이트
void myBullet::updateBullet() {
    
    if (bbulletActive) {
        bulletSprite.move(0, -bulletSpeed);
        if (bulletSprite.getPosition().y < 0) {
            bbulletActive = false;
        }
    }

}

bool myBullet::isBulletActive() {
    return bbulletActive;
}

