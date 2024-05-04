#ifndef MYCHARACTER_H
#define MYCHARACTER_H

#include <SFML/Graphics.hpp>

using namespace sf;

class myCharacter{
public:
    myCharacter(const Vector2f& pos); // 생성자
    Vector2f handleInput(Vector2f& pos); // 입력처리
    Vector2f managePos(RenderWindow& window, Vector2f&pos);
    void updateCharacter(float deltaTime);
    void draw(RenderWindow& window);
    FloatRect getLocalBounds();
    FloatRect getGlobalBounds();

    void setPosition(const Vector2f& pos);
    void hit();
    int getLives();

private:
    Sprite characterSprite;
    Texture characterTexture;
    Texture hitTexture;
    int lives;
    float DamagedMoveScale;
};

#endif