#include "myCharacter.h"

using namespace sf;

myCharacter::myCharacter(const Vector2f& pos) : lives(3), DamagedMoveScale(0){
    if(!characterTexture.loadFromFile("pic_myCharacter.png") || !hitTexture.loadFromFile("pic_myCharacterDamaged.png")){
        printf("Load Character Error\n");
        return;
    }
    
    characterSprite.setTexture(characterTexture);
    characterTexture.setSmooth(true);
    FloatRect bounds = characterSprite.getLocalBounds();
    characterSprite.setOrigin(bounds.width/2, bounds.height/2);
    characterSprite.setPosition(pos);
    characterSprite.setScale(0.2f, 0.2f);
}


//캐릭터 무브먼트 구현(UP, DOWN, LEFT, RIGHT)
Vector2f myCharacter::handleInput(Vector2f& pos) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        pos.y -= 0.2-DamagedMoveScale;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        pos.y += 0.2-DamagedMoveScale;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        pos.x -= 0.2-DamagedMoveScale;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        pos.x += 0.2-DamagedMoveScale;
    }
    Vector2f updatedPos(pos.x, pos.y);
    return updatedPos;
}

//캐릭터 Position 업데이트
Vector2f myCharacter::managePos(RenderWindow& window, Vector2f& pos){
    FloatRect bounds = characterSprite.getLocalBounds();
    if(pos.x < bounds.width / 2 * characterSprite.getScale().x){
        pos.x = bounds.width / 2 * characterSprite.getScale().x;
    }
    if(pos.x > window.getSize().x - bounds.width / 2 * characterSprite.getScale().x){
        pos.x = window.getSize().x - bounds.width / 2 * characterSprite.getScale().x;
    }
    if(pos.y < bounds.height / 2 * characterSprite.getScale().y){
        pos.y = bounds.height / 2 * characterSprite.getScale().y;
    }
    if(pos.y > window.getSize().y - bounds.height / 2 * characterSprite.getScale().y){
        pos.y = window.getSize().y - bounds.height / 2 * characterSprite.getScale().y;
    }
    Vector2f updatedPos(pos.x, pos.y);
    return updatedPos;
}


void myCharacter::draw(sf::RenderWindow& window) {
    window.draw(characterSprite);
}

FloatRect myCharacter::getLocalBounds() {
    return characterSprite.getLocalBounds();
}

FloatRect myCharacter::getGlobalBounds() {
    return characterSprite.getGlobalBounds();
}

void myCharacter::setPosition(const Vector2f& pos){
    characterSprite.setPosition(pos);
}


//캐릭터 충돌시 처리 메소드
void myCharacter::hit() {
    if (lives > 0) {
        --lives;
        if(lives == 1){
            characterSprite.setTexture(hitTexture);
            DamagedMoveScale += 0.1f;
        }
    }
}

int myCharacter::getLives() {
    return lives;
}