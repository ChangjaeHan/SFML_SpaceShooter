#include "meteor.h"

using namespace sf;

Meteor::Meteor(const Vector2f& startPos) : bmeteorActive(true), meteorSpeed(0.0f, 0.12f) {
    if (!meteorTexture.loadFromFile("pic_ufo.png") || !hitTexture.loadFromFile("pic_ufoDamaged.png")) {
        // 텍스처 로드 실패시
        bmeteorActive = false;
        return;
    }
    meteorTexture.setSmooth(true); 
    
    meteorSprite.setTexture(meteorTexture);
    meteorSprite.setPosition(startPos);
    meteorSprite.setScale(0.2f, 0.2f);
    hits = 0;
}

Sprite& Meteor::getMeteorSprite() {
    return meteorSprite;
}

//UFO 비활성화
void Meteor::deactivateMeteor() {
    bmeteorActive = false;
}


//화면에 UFO 그리기
void Meteor::draw(RenderWindow& window) {
    if (bmeteorActive) {
        window.draw(meteorSprite);
    }
}

//UFO 업데이트
void Meteor::updateMeteor() {
    if (bmeteorActive) {
        meteorSprite.move(meteorSpeed);
    }
}

//UFO의 활성 상태를 확인
bool Meteor::isMeteorActive() {
    return bmeteorActive;
}

//UFO 충돌시 처리 메소드
void Meteor::hit(){
    hits++;
    if(hits >= 2){
        bmeteorActive = false;
        return;
    }else{
        meteorSpeed *= 0.5f;
        meteorSprite.setTexture(hitTexture);
    }
}

int Meteor::getHits(){
    return hits;
}
