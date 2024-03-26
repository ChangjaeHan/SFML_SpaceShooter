#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "meteor.h"
#include "myBullet.h"

using namespace sf;
using namespace std;

Texture gameStartButtonTexture, uiTexture;
Texture backGroundTexture;

bool initializeUI(RenderWindow& uiWindow, RectangleShape& gameStartButton, Music& uiMusic, Sprite& uiSprite);
//bool initializeGame(RenderWindow& window, Music& gameMusic, Text& scoreText, Sprite& sprite, Font& font);

int main(){

    //인터페이스창
    RenderWindow uiWindow(VideoMode(800,600), "UI Window");

    RectangleShape gameStartButton(Vector2f(100,100));
    //Texture gameStartButtonTexture;
    Music uiMusic;
    Sprite uiSprite;

    if(!initializeUI(uiWindow, gameStartButton, uiMusic, uiSprite)) return -1;

    bool gameStart = false;
    //TODO: Q로직 바꾸기
    while(uiWindow.isOpen() && !Keyboard::isKeyPressed(Keyboard::Q) && !gameStart){
        Event uiEvent;
        while(uiWindow.pollEvent(uiEvent)){
            if(uiEvent.type == Event::Closed){
                uiWindow.close();
                return -1;
            }
            //게임시작 버튼 눌렀는지 확인하는 로직
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
    RenderWindow window(VideoMode(800, 600), "Gameplay");

    Sprite backGroundSprite;
    if(!backGroundTexture.loadFromFile("pic_backGround.png")) return -1;
    backGroundSprite.setTexture(backGroundTexture); 
    
    Texture heartTexture;
    if (!heartTexture.loadFromFile("pic_heart.png")) return -1;
    int characterLives = 3;
    //bool isCollided = false;

    vector<Sprite> hearts(characterLives);
    for(int i=0; i<characterLives; i++) {
        hearts[i].setTexture(heartTexture);
        hearts[i].setPosition(750-(i+1)*26,20); // 하트를 화면의 왼쪽 상단에 배치
    }

    Music gameMusic;
    if(!gameMusic.openFromFile("shooter.mp3")) return -1;

    gameMusic.play();

    Font font;
    if(!font.loadFromFile("waguri.ttf")){
	    printf("Error loading font\n");
	    return -1;
    }

    Text scoreText;
    scoreText.setFont(font); // 앞서 로드한 폰트 설정
    scoreText.setCharacterSize(24); // 텍스트 크기 설정
    scoreText.setFillColor(sf::Color::White); // 텍스트 색상 설정
    scoreText.setPosition(25, 25); // 화면 왼쪽 상단에 위치 설정
    scoreText.setString("Score: 0"); // 초기 스코어 텍스트 설정


    vector<myBullet> bullets;
    vector<Meteor> meteors;

    Vector2f pos(400,300);
    Texture characterTexture;

    if(!characterTexture.loadFromFile("pic_myCharacter.png")){
        printf("Load Character Error\n");
	    return -1;
    }
    characterTexture.setSmooth(true);
        
    Sprite characterSprite;
    characterSprite.setTexture(characterTexture);

    FloatRect bounds = characterSprite.getLocalBounds();
    characterSprite.setOrigin(bounds.width/2, bounds.height/2);
    characterSprite.setPosition(pos);
    characterSprite.setScale(0.2f, 0.2f);

    int score = 0;

    //clock
    Clock bulletClock, meteorClock;
    float bulletCooltime = 0.2f, meteorSpawnTime = 2.0f;
    float lastBulletTime = 0.0f, elapsedTime = 0.0f;

    srand(time(0));

    while (window.isOpen() && !Keyboard::isKeyPressed(Keyboard::Q)){
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed){
                window.close();
            }
        }

        float currentTime = bulletClock.getElapsedTime().asSeconds();
        float deltaTime = meteorClock.restart().asSeconds();
        elapsedTime += deltaTime;

        if(elapsedTime >= meteorSpawnTime){
            elapsedTime = 0;
            float randMeteorPos = static_cast<float>(rand()%window.getSize().x-50);
            meteors.emplace_back(Vector2f(randMeteorPos, -50.0f));
        }
        for(int i=0; i<meteors.size(); i++){
            meteors[i].updateMeteor();
            
            // 화면 아래로 완전히 벗어났는지 확인
            if (!meteors[i].isMeteorActive() || meteors[i].getMeteorSprite().getPosition().y > window.getSize().y) 
            {
                meteors.erase(meteors.begin() + i);
                --i;
            }
        }

        if(Keyboard::isKeyPressed(Keyboard::Up)){
            pos.y -= 0.2;		
        }
        if(Keyboard::isKeyPressed(Keyboard::Down)){
            pos.y += 0.2;
        }
        if(Keyboard::isKeyPressed(Keyboard::Left)){
            pos.x -= 0.2;
        }
        if(Keyboard::isKeyPressed(Keyboard::Right)){
            pos.x += 0.2;
        }

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


        if(Keyboard::isKeyPressed(Keyboard::Space) && currentTime - lastBulletTime > bulletCooltime){
            FloatRect shipBounds = characterSprite.getGlobalBounds();
            Vector2f bulletStartPos(shipBounds.left + shipBounds.width / 2.0f, shipBounds.top);
            bullets.push_back(myBullet(bulletStartPos));
            lastBulletTime = currentTime;
        }

        characterSprite.setPosition(pos);
        scoreText.setString("Score: " + to_string(score));


        window.clear();
        window.draw(backGroundSprite);
        window.draw(characterSprite);
        window.draw(scoreText);
        
        for (auto& heart : hearts){
            window.draw(heart);
        }
            

        for (auto& meteor : meteors) {
            meteor.draw(window);
        }
        for(int i=0; i<bullets.size(); i++){
            if(!bullets[i].isBulletActive()){
                bullets.erase(bullets.begin() + i);
                --i;
            }else{
                //bullets[i].updateBullet();
                bullets[i].draw(window);
                bullets[i].updateBullet();
            }
        }

        for (int i = 0; i < meteors.size(); i++) {
            for (int j = 0; j < bullets.size(); j++) {
                if (meteors[i].getMeteorSprite().getGlobalBounds().intersects(bullets[j].getBulletSprite().getGlobalBounds())) {
                    // 여기서 충돌 처리 로직을 구현합니다.
                    meteors[i].hit();
                    bullets.erase(bullets.begin() + j); // 총알 제거
                    //meteors[i].deactivateMeteor(); // 운석 비활성화
                    if(!meteors[i].isMeteorActive()){
                        meteors.erase(meteors.begin() + i);
                        --i; // 운석 벡터에서 항목을 제거했으므로 인덱스 조정
                        score += 1; // 점수 증가
                        //break; // 중첩된 루프를 빠져나옵니다.
                    }
            //break;
                }
            }
        }

        window.display();
    }
    return 0;

}

bool initializeUI(RenderWindow& uiWindow, RectangleShape& gameStartButton, Music& uiMusic, Sprite& uiSprite){

    if(!uiTexture.loadFromFile("pic_UI.jpg")) return false;
    uiSprite.setTexture(uiTexture);

    gameStartButton.setPosition(350,450); // 버튼 위치 설정
    if (!gameStartButtonTexture.loadFromFile("pic_gameStartButton.jpg")) return false;
    gameStartButton.setTexture(&gameStartButtonTexture);
    
    if(!uiMusic.openFromFile("spaceship_lady.mp3")) return false;
    uiMusic.play();   
    
    return true;
}
