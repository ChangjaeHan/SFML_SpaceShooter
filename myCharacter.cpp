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

Vector2f myCharacter::handleInput(Vector2f& pos) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        //characterSprite.move(0, -0.2);
        pos.y -= 0.2-DamagedMoveScale;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        //characterSprite.move(0, 0.2);
        pos.y += 0.2-DamagedMoveScale;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        //characterSprite.move(-0.2, 0);
        pos.x -= 0.2-DamagedMoveScale;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        //characterSprite.move(0.2, 0);
        pos.x += 0.2-DamagedMoveScale;
    }
    Vector2f updatedPos(pos.x, pos.y);
    return updatedPos;
}
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


void myCharacter::updateCharacter(float deltaTime) {
    // 캐릭터의 위치 업데이트 또는 기타 상태 업데이트
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

void myCharacter::hit() {
    if (lives > 0) {
        --lives;
        // 추가적인 충돌 처리 로직 (예: 피격 애니메이션 재생)
        if(lives == 1){
            characterSprite.setTexture(hitTexture);
            DamagedMoveScale += 0.1f;
        }
    }
}

int myCharacter::getLives() {
    return lives;
}