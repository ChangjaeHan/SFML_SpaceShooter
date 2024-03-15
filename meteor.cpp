#include "meteor.h"

using namespace sf;
//Texture Meteor::texture; // 정적 멤버 변수 초기화
//bool Meteor::textureLoaded = false; // 텍스처 로딩 상태를 추적하는 정적 변수

// 생성자 구현
Meteor::Meteor(const Vector2f& startPos) : bmeteorActive(true), meteorSpeed(0.0f, 0.12f) {
    if (!meteorTexture.loadFromFile("pic_ufo.png")) {
        // 텍스처 로딩 실패 시
        bmeteorActive = false;
        return;
    }
    meteorTexture.setSmooth(true); // 텍스처를 부드럽게 처리
    //textureLoaded = true; // 텍스처가 성공적으로 로드되었다고 표시
    
    meteorSprite.setTexture(meteorTexture);
    meteorSprite.setPosition(startPos);
    meteorSprite.setScale(0.2f, 0.2f); // 운석의 크기 조정
    hits = 0;
}

Sprite& Meteor::getMeteorSprite() {
    return meteorSprite;
}

void Meteor::deactivateMeteor() {
    bmeteorActive = false;
}


// 화면에 운석을 그리는 메소드
void Meteor::draw(RenderWindow& window) {
    if (bmeteorActive) {
        window.draw(meteorSprite);
    }
}

// 운석을 업데이트하는 메소드
void Meteor::updateMeteor() {
    if (bmeteorActive) {
        meteorSprite.move(meteorSpeed); // 운석 이동
    }
}

// 운석의 활성 상태를 확인하는 메소드
bool Meteor::isMeteorActive() {
    return bmeteorActive;
}

void Meteor::hit(){
    hits++;
    if(hits >= 2){
        bmeteorActive = false;
    }
}

int Meteor::getHits(){
    return hits;
}
