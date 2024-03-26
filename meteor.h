#ifndef METEOR_H
#define METEOR_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Meteor {
public:

    Meteor(const Vector2f& startPos);
    void draw(RenderWindow& window);
    void updateMeteor();
    bool isMeteorActive();    
    Sprite& getMeteorSprite();
    void deactivateMeteor();
    void hit();
    int getHits();

private:
    Sprite meteorSprite;
    Texture meteorTexture;
    Texture hitTexture;
    Vector2f meteorSpeed;
    bool bmeteorActive;
    int hits;
};

#endif

