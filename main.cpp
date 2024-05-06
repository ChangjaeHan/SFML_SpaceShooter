#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "myCharacter.h"
#include "meteor.h"
#include "myBullet.h"


using namespace sf;
using namespace std;

Texture gameStartButtonTexture, uiTexture;
Texture backGroundTexture, heartTexture;


bool initializeUI(RectangleShape& gameStartButton, Music& uiMusic, Sprite& uiSprite);
bool initializeGame(Music& gameMusic, SoundBuffer& hitBuffer, Sound& hitSound, Sprite& backGroundSprite);
void setScore(Font& font, Text& scoreText);

int main(){

    //인터페이스창
    RenderWindow uiWindow(VideoMode(800,600), "UI Window");

    RectangleShape gameStartButton(Vector2f(100,100));
    Music uiMusic;
    Sprite uiSprite;

    //UI 초기화
    if(!initializeUI(gameStartButton, uiMusic, uiSprite)) return -1;

    bool gameStart = false;

    //메인 UI화면 기능
    while(uiWindow.isOpen() && !Keyboard::isKeyPressed(Keyboard::Q) && !gameStart){
        Event uiEvent;
        while(uiWindow.pollEvent(uiEvent)){
            if(uiEvent.type == Event::Closed){
                uiWindow.close();
                return -1;
            }
            //게임시작 버튼 눌렀는지 확인 로직
            if(uiEvent.type == Event::MouseButtonPressed){
                if(uiEvent.mouseButton.button == sf::Mouse::Left) {
                    // 마우스 위치 확인
                    sf::Vector2i mousePos = sf::Mouse::getPosition(uiWindow);
                    // 버튼 영역과 마우스 클릭 위치 비교
                    if (gameStartButton.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                        gameStart = true;
                        break;
                    }
                }
            }
        }
        uiWindow.clear();
        uiWindow.draw(uiSprite);
        uiWindow.draw(gameStartButton);
        uiWindow.display();
    }
    uiWindow.close();
    uiMusic.stop();


    //게임창
    RenderWindow gameWindow(VideoMode(800, 600), "Gameplay");

    //백그라운드 및 객체
    Sprite backGroundSprite;
    Music gameMusic;
    SoundBuffer hitBuffer;
    Sound hitSound;
    Font font;
    Text scoreText;


    //게임화면 초기화
    if(!initializeGame(gameMusic, hitBuffer, hitSound, backGroundSprite)) return -1;

    //스코어판
    int score = 0;
    bool openScore = false;
    setScore(font, scoreText);

    //하트(목숨)
    if (!heartTexture.loadFromFile("pic_heart.png")) return -1;
    int characterLives = 3;
    vector<Sprite> hearts(characterLives);
    for(int i=0; i<characterLives; i++) {
        hearts[i].setTexture(heartTexture);
        hearts[i].setPosition(750-(i+1)*26,20); // 하트를 화면의 왼쪽 상단에 배치
    }

    //객체(총, UFO, 우주선)
    vector<myBullet> bullets;
    vector<Meteor> meteors;
    Vector2f characterPos(400,300);
    
    myCharacter character(characterPos);

    //clock
    Clock bulletClock, meteorClock;
    float bulletCooltime = 0.2f, meteorSpawnTime = 2.0f;
    float lastBulletTime = 0.0f, elapsedTime = 0.0f;

    srand(time(0)); //UFO 무작위 스폰 시간

    //메인 게임 화면 기능
    while (gameWindow.isOpen() && !Keyboard::isKeyPressed(Keyboard::Q)){
        Event event;
        while(gameWindow.pollEvent(event)){
            if(event.type == Event::Closed){
                gameWindow.close();
            }
        }

        //UFO와 총알 시간 계산
        float currentTime = bulletClock.getElapsedTime().asSeconds();
        float deltaTime = meteorClock.restart().asSeconds();
        elapsedTime += deltaTime;

        if(elapsedTime >= meteorSpawnTime){
            elapsedTime = 0;
            float randMeteorPos = static_cast<float>(rand()%gameWindow.getSize().x-50);
            meteors.emplace_back(Vector2f(randMeteorPos, -50.0f));
        }
        for(int i=0; i<meteors.size(); i++){
            meteors[i].updateMeteor();
            // 화면 아래로 완전히 벗어났는지 체크
            if (!meteors[i].isMeteorActive() || meteors[i].getMeteorSprite().getPosition().y > gameWindow.getSize().y) 
            {
                meteors.erase(meteors.begin() + i);
                --i;
            }
        }

        //우주선(캐릭터) 위치 업데이트
        characterPos = character.handleInput(characterPos);
        characterPos = character.managePos(gameWindow, characterPos);

        if(Keyboard::isKeyPressed(Keyboard::Space) && currentTime - lastBulletTime > bulletCooltime){
            FloatRect shipBounds = character.getGlobalBounds();
            Vector2f bulletStartPos(shipBounds.left + shipBounds.width / 2.0f, shipBounds.top);
            bullets.push_back(myBullet(bulletStartPos));
            lastBulletTime = currentTime;
        }

        character.setPosition(characterPos);
        
        scoreText.setString("Score: " + to_string(score));



        //화면 및 객체 그리기
        gameWindow.clear();
        gameWindow.draw(backGroundSprite);
        gameWindow.draw(scoreText);
        character.draw(gameWindow);

        for (auto& heart : hearts){
            gameWindow.draw(heart);
        }            

        for (auto& meteor : meteors) {
            meteor.draw(gameWindow);
        }

        for(int i=0; i<bullets.size(); i++){
            if(!bullets[i].isBulletActive()){
                bullets.erase(bullets.begin() + i);
                --i;
            }else{
                bullets[i].draw(gameWindow);
                bullets[i].updateBullet();
            }
        }

        for (int i = 0; i < meteors.size(); i++) {
            for (int j = 0; j < bullets.size(); j++) {
                if (meteors[i].getMeteorSprite().getGlobalBounds().intersects(bullets[j].getBulletSprite().getGlobalBounds())) {
                    // 충돌 처리 로직
                    meteors[i].hit();
                    bullets.erase(bullets.begin() + j); // 총알 제거
                    hitSound.play();

                    if(!meteors[i].isMeteorActive()){
                        meteors.erase(meteors.begin() + i);
                        --i; // UFO 벡터에서 항목을 제거함으로써 인덱스 조정
                        score += 1; // 점수 증가
                    }
                }
            }

            //캐릭터가 UFO에 충돌했을 경우
            if (meteors[i].getMeteorSprite().getGlobalBounds().intersects(character.getGlobalBounds())){
                character.hit();
                meteors.erase(meteors.begin() + i);
                if(character.getLives() > 0){
                    hearts.erase(hearts.begin());
                };
                --i;
            }
        }

        if(character.getLives() <= 0){
            gameWindow.close();
            openScore = true;
        }

        gameWindow.display();
    }


    //게임이 끝났을 시 Score 메뉴 창
    RenderWindow scoreWindow(VideoMode(800,600), "Score");
    while(openScore && scoreWindow.isOpen() && !Keyboard::isKeyPressed(Keyboard::Q)){
        Event event;
        while(scoreWindow.pollEvent(event)){
            if(event.type == Event::Closed){
                scoreWindow.close();
            }
        }

        scoreText.setPosition(330, 270); 
        scoreText.setString("Score: " + to_string(score));
        scoreText.setCharacterSize(30);
        backGroundSprite.setTexture(backGroundTexture); 
        scoreWindow.draw(backGroundSprite);
        scoreWindow.draw(scoreText);
        scoreWindow.display();
    }

    return 0;
}



/*
*   초기화 함수
*/
bool initializeUI(RectangleShape& gameStartButton, Music& uiMusic, Sprite& uiSprite){

    if(!uiTexture.loadFromFile("pic_UI.jpg")) return false;
    uiSprite.setTexture(uiTexture);

    gameStartButton.setPosition(350,450); // 버튼 위치 설정
    if (!gameStartButtonTexture.loadFromFile("pic_gameStartButton.jpg")) return false;
    gameStartButton.setTexture(&gameStartButtonTexture);
    
    if(!uiMusic.openFromFile("spaceship_lady.mp3")) return false;
    uiMusic.play();   
    
    return true;
}

bool initializeGame(Music& gameMusic, SoundBuffer& hitBuffer, Sound& hitSound, Sprite& backGroundSprite){
    
    if(!backGroundTexture.loadFromFile("pic_backGround.png")) return -1;
    backGroundSprite.setTexture(backGroundTexture); 
    
    //게임 음악
    if(!gameMusic.openFromFile("shooter.mp3")) return -1;
    gameMusic.play();

    if(!hitBuffer.loadFromFile("explode.mp3")) return -1;
    hitSound.setBuffer(hitBuffer);

    return true;
}

void setScore(Font& font, Text& scoreText){
    if(!font.loadFromFile("waguri.ttf")) return;
    
    scoreText.setFont(font); // 앞서 로드한 폰트 설정
    scoreText.setCharacterSize(24); // 텍스트 크기 설정
    scoreText.setFillColor(Color::White); // 텍스트 색상 설정
    scoreText.setPosition(25, 25); // 화면 왼쪽 상단에 위치 설정
    scoreText.setString("Score: 0"); // 초기 스코어 텍스트 설정

}