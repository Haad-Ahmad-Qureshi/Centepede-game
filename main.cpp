#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;
const int gameColumns = resolutionY / boxPixelsY;
int l = 12;

int gameGrid[gameRows][gameColumns] = {};

const int x = 0;
const int y = 1;
const int exists = 2;

class centepede{
    int cx;
    int cy;
    int direction;
    string type;

public:
    centepede():cx(0),cy(0),direction(0),type(""){}

    centepede(int x, int y, int d, string t){
        cx = x;
        cy = y;
        direction = d;
        type = t;
    }

    int getx(){ return cx; }
    int gety(){ return cy; }

    void setx(int v){ cx = v; }
    void sety(int v){ cy = v; }

    int getdirection(){ return direction; }

    void switchdirection(){
        direction = -direction;
    }

    string gettype(){ return type; }
};

class mushroom{
    int mx;
    int my;
    int health;

public:
    mushroom(){ mx=0; my=0; health=2; }

    mushroom(int x, int y){
        mx = x;
        my = y;
        health = 2;
    }

    int getx(){ return mx; }
    int gety(){ return my; }

    void sethealth(){ health--; }
    int gethealth(){ return health; }
};

// FUNCTIONS
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& sprite);
void moveplayer(float player[]);
void moveBullet(float bullet[], sf::Clock& clock);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& sprite);
void drawMushroom(sf::RenderWindow& window, mushroom* m, sf::Sprite& sprite);
void drawcentepede(sf::RenderWindow& window, centepede* c, sf::Sprite& head, sf::Sprite& body);
void movecentepede(centepede* c, int l);

// ---------------- MAIN ----------------
int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode({resolutionX, resolutionY}), "Centipede");
    window.setSize(sf::Vector2u(640, 640));
    window.setPosition(sf::Vector2i(100, 0));

    sf::Music bgMusic;
    bgMusic.openFromFile("Music/field_of_hopes.ogg");
    bgMusic.play();

    sf::Texture bgTex;
    bgTex.loadFromFile("Textures/background.png");
    sf::Sprite bg(bgTex);
    bg.setColor(sf::Color(255,255,255,64));

    float player[2];
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = (gameRows * 3 / 4) * boxPixelsY;

    sf::Texture playerTex;
    playerTex.loadFromFile("Textures/player.png");
    sf::Sprite playerSprite(playerTex);
    playerSprite.setTextureRect(sf::IntRect({0,0},{boxPixelsX,boxPixelsY}));

    float bullet[3];
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = false;

    sf::Clock bulletClock;

    sf::Texture bulletTex;
    bulletTex.loadFromFile("Textures/bullet.png");
    sf::Sprite bulletSprite(bulletTex);
    bulletSprite.setTextureRect(sf::IntRect({0,0},{boxPixelsX,boxPixelsY}));

    int s = rand()%11 + 20;
    mushroom* mush = new mushroom[s];

    sf::Texture mushTex;
    mushTex.loadFromFile("Textures/mushroom.png");
    sf::Sprite mushSprite(mushTex);
    mushSprite.setTextureRect(sf::IntRect({0,0},{boxPixelsX,boxPixelsY}));

    for(int i=0;i<s;i++)
        mush[i] = mushroom(rand()%gameColumns*32, rand()%(gameRows-5)*32);

    centepede* cent = new centepede[l];

    sf::Texture headTex, bodyTex;
    headTex.loadFromFile("Textures/c_head_left_walk.png");
    bodyTex.loadFromFile("Textures/c_body_left_walk.png");

    sf::Sprite head(headTex);
    sf::Sprite body(bodyTex);

    for(int i=0;i<l-1;i++)
        cent[i] = centepede(i*32,0,1,"body");

    cent[l-1] = centepede((l-1)*32,0,1,"head");

    sf::Clock moveClock;

    // ---------------- LOOP ----------------
    while(window.isOpen())
    {
        while(auto event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(bg);

        drawPlayer(window, player, playerSprite);

        for(int i=0;i<s;i++){
            drawMushroom(window,&mush[i],mushSprite);
        }

        if(moveClock.getElapsedTime().asMilliseconds() > 200){
            movecentepede(cent,l);
            moveClock.restart();
        }

        drawcentepede(window,cent,head,body);
        moveplayer(player);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !bullet[exists]){
            bullet[x]=player[x];
            bullet[y]=player[y]-boxPixelsY;
            bullet[exists]=true;
        }

        if(bullet[exists]){
            moveBullet(bullet,bulletClock);
            drawBullet(window,bullet,bulletSprite);
        }

        // COLLISION FIX (WORKING)
        for(int i=0;i<s;i++){
            if(bullet[exists]){

                float bx = bullet[x] + 16;
                float by = bullet[y] + 16;

                if(bx>=mush[i].getx() && bx<=mush[i].getx()+32 &&
                   by>=mush[i].gety() && by<=mush[i].gety()+32)
                {
                    mush[i].sethealth();
                    bullet[exists] = false;

                    if(mush[i].gethealth() <= 0){
                        for(int j=i;j<s-1;j++)
                            mush[j]=mush[j+1];

                        s--;
                        i--;
                    }
                }
            }
        }

        window.display();
    }

    delete[] mush;
    delete[] cent;
}
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite)
{
    playerSprite.setPosition(sf::Vector2f(player[x], player[y]));
    window.draw(playerSprite);
}

void drawMushroom(sf::RenderWindow& window, mushroom* m, sf::Sprite& mushroomSprite)
{
    mushroomSprite.setPosition(sf::Vector2f(m->getx(), m->gety()));
    window.draw(mushroomSprite);
}

void moveBullet(float bullet[], sf::Clock& bulletClock)
{
    if (bulletClock.getElapsedTime().asMilliseconds() < 20)
        return;

    bulletClock.restart();
    bullet[y] -= 10;

    if (bullet[y] < -32)
        bullet[exists] = false;
}

void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite)
{
    bulletSprite.setPosition(sf::Vector2f(bullet[x], bullet[y]));
    window.draw(bulletSprite);
}

void moveplayer(float player[])
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && player[x] > 0)
        player[x] -= 0.5;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) &&
        player[x] < resolutionX - boxPixelsX)
        player[x] += 0.5;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) &&
        player[y] > resolutionY - (boxPixelsY * 5))
        player[y] -= 0.5;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) &&
        player[y] < resolutionY - boxPixelsY)
        player[y] += 0.5;
}

void drawcentepede(sf::RenderWindow& window, centepede* c,
                    sf::Sprite& centipedehead,
                    sf::Sprite& centipedesbody)
{
    for (int i = 0; i < l; i++)
    {
        if (c[i].gettype() == "head")
        {
            centipedehead.setPosition(sf::Vector2f(c[i].getx(), c[i].gety()));
            window.draw(centipedehead);
        }
        else
        {
            centipedesbody.setPosition(sf::Vector2f(c[i].getx(), c[i].gety()));
            window.draw(centipedesbody);
        }
    }
}

void movecentepede(centepede* c, int l)
{
    for (int i = 0; i < l; i++)
    {
        if (c[i].getdirection() == 1)
        {
            c[i].setx(c[i].getx() + 32);

            if (c[i].getx() >= resolutionX)
            {
                c[i].sety(c[i].gety() + 32);
                c[i].switchdirection();
            }
        }
        else if (c[i].getdirection() == -1)
        {
            c[i].setx(c[i].getx() - 32);

            if (c[i].getx() <= 0)
            {
                c[i].sety(c[i].gety() + 32);
                c[i].switchdirection();
            }
        }
    }
}