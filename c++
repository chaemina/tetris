#include <SFML/Graphics.hpp>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <string>
#include <exception>
#include <conio.h>
#include <stdlib.h>
#include <cstdlib>
#include <algorithm>
#include <SFML/Config.hpp>
#include <SFML/System/Export.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <vector>
#include <cmath>


using namespace std;
using namespace sf;
bool automaticPlaying = true;

//테트리스 메인 보드
const int M = 20;
const int N = 10;

int board[M][N] = { 0 }; //새 벽돌을 만들어야 하는 상태

struct  Point
{
    int x, y;  //좌표
};
Point a[4];
Point c[4];  //점이 4개 들어있는 배열 생성

int abc = 0; //현재 내려간 블록 카운트
int preabc = 0; //이전

int tiles[7][4] = {
1,3,5,7, //I
2,4,5,7, //Z
3,5,4,6, //S
3,5,4,7, //T
2,3,5,7, //L
3,5,7,6, //J
2,3,4,5, //O

};

bool check()
{
    for (int i = 0; i < 4; ++i)
    {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return false;
        else if (board[a[i].y][a[i].x]) return false;
    }
    return true;
}

const float windowWidth = 500;
const float windowHeight = 500;





//text 함수
int textprint(Text& text, Font& font, int size, float x, float y, const Color& color, const Color& outColor, string p)

{
    text.setFont(font);
    text.setCharacterSize(size);
    text.setPosition(x, y);
    text.setFillColor(color);
    text.setOutlineColor(outColor);
    text.setOutlineThickness(1.f);
    text.setString(p);
    return 0;


}


int main() {
    srand(time(0));



    int score = 0;
    Text text;
    Text text2;
    Uint8 r = 0, g = 0, b = 0;
    string comment = "tetris Game Start";
    string hi = "PRESS THIS BUTTON TO START !"; //마우스를 누르면 게임시작
    int x = 0, y = 0;

    RenderWindow window(VideoMode(windowWidth, windowHeight), "tetris");


    window.setFramerateLimit(60);

    //시작화면 text
    Font font;
    if (!font.loadFromFile("./자료/DungGeunMo.ttf"))
        throw exception("font error");
    textprint(text, font, 100, 700, 30, Color::Green, Color::Black, comment);
    textprint(text2, font, 30, 50, 300, Color::Black, Color::Black, hi);

    Text scoreCurrent;
    scoreCurrent.setFont(font);
    scoreCurrent.setFillColor(Color::Red);
    scoreCurrent.setStyle(Text::Regular);
    scoreCurrent.setCharacterSize(30);
    scoreCurrent.setPosition(250, 30);

   


    Texture button;
    button.loadFromFile("./자료/NicePng_start-button-png_1108890.png");
    Sprite button2(button);
    button2.setPosition(220, 370);
    button2.setScale(0.09f, 0.09f);
    SoundBuffer buffer;

    if (!buffer.loadFromFile("./자료/bgm.wav")) {
        cout << "sound error" << endl;
    }

    Sound sound;
    sound.setBuffer(buffer);
    sound.play();
    sound.setLoop(true);
    window.display();

    while (window.isOpen()) {

        text.move(-1, 0);

        window.clear(Color::White);
        window.draw(text);
        window.draw(button2);
        window.draw(text2);
        window.display();
        Event event;

        while (window.pollEvent(event)) {


            switch (event.type) {

            case Event::Closed:
                window.close();
                break;

            case Event::MouseButtonPressed:
                if (event.mouseButton.button == Mouse::Left) {
                    window.create(VideoMode(windowWidth, windowHeight), "tetris game");

                    button2.move(1000, 0);
                    text2.move(1000, 0);



                    time_t start, end;
                    double result;
                    start = time(NULL);

                    Texture t1;
                    t1.loadFromFile("./자료/tile.png");



                    Sprite tileSprite(t1);


                    Texture t2;
                    t2.loadFromFile("./자료/line2.png");
                    Sprite deadline(t2);
                    deadline.setPosition(0, 23.0001);

                  /*  Texture t3;
                    t3.loadFromFile("./자료/frame.jpg");
                    Sprite frame;
                    frame.setTexture(t3);      프레임도 적용 가능한데 디자인상 지웠습니다 rame을 그리면 보드의 정확한 넓이를 확인 할 수 있습니다. */  



                    int dx = 0;
                    bool rotate = false;
                    int colorNum = 1;
                    float timer = 0;
                    float delay = 0.3;
                    int currentScore = 0;
                    Clock clock;
                    bool collide;


                    a[0].x = 0; a[0].y = 1;
                    a[1].x = 1; a[1].y = 1;
                    a[2].x = 1; a[2].y = 2;
                    a[3].x = 1; a[3].y = 3;


                    while (window.isOpen()) {
                        float time = clock.getElapsedTime().asSeconds();  //초 단위로 경과시간을 얻음
                        clock.restart(); //시계 재시작
                        timer += time;  //시간 누적

                        Event e;
                        while (window.pollEvent(e)) {
                            if (e.type == Event::Closed)
                                window.close();
                            if (e.type == Event::KeyPressed)
                                if (e.key.code == Keyboard::Up) rotate = true;
                                else if (e.key.code == Keyboard::Left) dx = -1;
                                else if (e.key.code == Keyboard::Right) dx = 1;    //원래 dx =0이니까 왼쪽 -1 오른쪽 +1
                        }

                        if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;



                        //이동//
                        for (int i = 0; i < 4; ++i) {
                            c[i] = a[i];
                            a[i].x += dx;    //좌우로
                        }
                        if (!check()) for (int i = 0; i < 4; ++i) a[i] = c[i];

                        // Rotate //
                        if (rotate)
                        {
                            Point p = a[1]; // center of rotation
                            for (int i = 0; i < 4; ++i)
                            {
                                int x = a[i].y - p.y;
                                int y = a[i].x - p.x;
                                a[i].x = p.x - x;
                                a[i].y = p.y + y;
                            }
                            if (!check()) for (int i = 0; i < 4; ++i) a[i] = c[i];
                        }
                        //시간//
                        if (timer > delay) {
                            for (int i = 0; i < 4; ++i) {
                                c[i] = a[i]; a[i].y += 1;   //아래로
                            }
                            if (!check()) {
                                if ((preabc == abc) && preabc != 0 && preabc == 1) { //이전움직임과 현재 움직임 동일(움직임이 없다고 볼때), 0으로 선언했으니까 시작과 동시에 꺼지는 것 방지, 움직임 같을때도 꺼지니까 이전움직임 1일때 
                                   
                                    window.close();
                                    sound.stop();

                                }
                                for (int i = 0; i < 4; ++i) {
                                    board[c[i].y][c[i].x] = colorNum;
                                }
                                cout << "preabc :" << preabc << endl;
                                colorNum = 1 + rand() % 7;
                                int n = rand() % 7;
                                for (int i = 0; i < 4; ++i) {
                                    a[i].x = tiles[n][i] % 2;
                                    a[i].y = tiles[n][i] / 2;
                                }
                                cout << "abc: " << abc << endl; 
                                preabc = abc; //abc를 이전의 것으로 저장 
                                abc = 0; //abc는 다시 세야되니까 초기화

                            }

                            timer = 0;
                            abc++;
                        }

                        //한줄 차면 지우기 //
                        int k = M - 1;
                        for (int i = M - 1; i > 0; i--) {
                            int count = 0;
                            for (int j = 0; j < N; ++j) {
                                if (board[i][j]) count++;
                                board[k][j] = board[i][j];
                            }
                            if (count < N) k--;
                            else {
                                score++;
                            }
                        }
                        dx = 0; delay = 0.3; rotate = false;

                        //그리기//
                        window.clear(Color::Black);





                        for (int i = 0; i < M; ++i) {
                            for (int j = 0; j < N; ++j) {
                                if (board[i][j] == 0) continue;
                                tileSprite.setTextureRect(IntRect(board[i][j] * 18, 0, 18, 18));
                                tileSprite.setPosition(j * 18, i * 18);
                                tileSprite.move(28, 31);
                                window.draw(tileSprite);
                            }
                        }

                        //출력//

                        for (int i = 0; i < 4; ++i) {
                            tileSprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
                            tileSprite.setPosition(a[i].x * 18, a[i].y * 18);
                            tileSprite.move(28, 31);
                            window.draw(tileSprite);

                    
                        }

                        currentScore = ("%f", score);
                        stringstream s;
                        s << currentScore;
                        scoreCurrent.setString("Score : " + s.str());
                        window.draw(scoreCurrent);
                        window.draw(deadline);
                        window.display();

                    }


                }

            }

        }

    }
    window.create(VideoMode(windowWidth, windowHeight), "Game Over");
    Text gameover;
    String over = "GAMEOVER !!";
    textprint(gameover, font, 70, 60, 30, Color::Magenta, Color::White, over);
    
    
    scoreCurrent.move(-90, 100);
    scoreCurrent.setFillColor(Color::White);
    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {

            if (e.type == Event::Closed) {
                window.close();
                break;
            }
            window.clear(Color::Black);
            window.draw(gameover);
            
            window.draw(scoreCurrent);
            window.display();
        }
    }
    return 0;

}
