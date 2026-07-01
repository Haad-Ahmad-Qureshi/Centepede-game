#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

using namespace std;
int score = 0;
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;
const int gameColumns = resolutionY / boxPixelsY;

bool flag=true;

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
void settype(string t){
    type=t;
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
void drawcentepede(sf::RenderWindow& window, centepede* c,sf::Sprite& head,sf::Sprite& body,int l);
void movecentepede(centepede* c, int l);
void drawgameover(sf::RenderWindow& window, sf::Sprite& gameover);

// ---------------- MAIN ----------------
int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(resolutionX, resolutionY)), "Centipede");
    window.setSize(sf::Vector2u(640, 640));
    window.setPosition(sf::Vector2i(100, 0));

    sf::Music bgMusic;
    if(!bgMusic.openFromFile("Music/field_of_hopes.ogg"))
        cout << "Music failed to load\n";
    bgMusic.play();

    sf::Texture bgTex;
    if(!bgTex.loadFromFile("Textures/background.png"))
        cout << "background.png failed to load\n";
    sf::Sprite bg(bgTex);
    bg.setColor(sf::Color(255,255,255,64));
sf::Texture gameovertex;
if(!gameovertex.loadFromFile("Textures/over.jpeg"))
    cout << "over.jpeg failed to load\n";
sf::Sprite gameover(gameovertex);
    gameover.setTextureRect(sf::IntRect({0,0},{960,960}));


    float player[2];
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = resolutionY - boxPixelsY;

    sf::Texture playerTex;
    if(!playerTex.loadFromFile("Textures/player.png"))
        cout << "player.png failed to load\n";
    sf::Sprite playerSprite(playerTex);
    playerSprite.setTextureRect(sf::IntRect({0,0},{boxPixelsX,boxPixelsY}));

    float bullet[3];
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = false;

    sf::Clock bulletClock;

    sf::Texture bulletTex;
    if(!bulletTex.loadFromFile("Textures/bullet.png"))
        cout << "bullet.png failed to load\n";
    sf::Sprite bulletSprite(bulletTex);
    bulletSprite.setTextureRect(sf::IntRect({0,0},{boxPixelsX,boxPixelsY}));

    int s = rand()%11 + 20;
    mushroom* mush = new mushroom[s];
vector<mushroom> pmushrooms;

    sf::Texture mushTex;
    if(!mushTex.loadFromFile("Textures/mushroom.png"))
        cout << "mushroom.png failed to load\n";
    sf::Sprite mushSprite(mushTex);
    mushSprite.setTextureRect(sf::IntRect({0,0},{boxPixelsX,boxPixelsY}));

    for(int i=0;i<s;i++){
        mush[i] = mushroom(rand()%gameColumns*32, rand()%(gameRows-5)*32);
		gameGrid[mush[i].getx()/32][mush[i].gety()/32] = 1;
}



//centepede creation
vector<vector<centepede>> centipedes;
    sf::Texture headTex, bodyTex;
    if(!headTex.loadFromFile("Textures/c_head_left_walk.png"))
        cout << "c_head_left_walk.png failed to load\n";
    if(!bodyTex.loadFromFile("Textures/c_body_left_walk.png"))
        cout << "c_body_left_walk.png failed to load\n";

    sf::Sprite head(headTex);
    head.setTextureRect(sf::IntRect({0,0},{boxPixelsX,boxPixelsY}));
	

    sf::Sprite body(bodyTex);
    body.setTextureRect(sf::IntRect({0,0},{boxPixelsX,boxPixelsY}));
vector<centepede> first;

for(int i = 0; i < 12; i++)
{
    first.push_back(centepede(i*32, 0, 1, "body"));
}

first[12-1].settype("head") ;

centipedes.push_back(first);

sf::Clock moveClock;
    sf::Clock pclock;


    // score display 
sf::Font font;
if (!font.openFromFile("Fonts/Roboto-Regular.ttf")) {
    cout << "Font failed to load\n";
}
sf::Text scoreText(font);
scoreText.setCharacterSize(24);
scoreText.setFillColor(sf::Color::Red);
scoreText.setPosition({10.f, 10.f});



////////////////////////////////////////////////////////////////////////////////////////////////////
    // ---------------- LOOP ----------------
////////////////////////////////////////////////////////////////////////////////////////////////////
    while(window.isOpen()&& flag==true)
    {
        while(auto event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(bg);
        scoreText.setString("Score: " + to_string(score));
        window.draw(scoreText);
        drawPlayer(window, player, playerSprite);
        //draw mushrooma
        for(int i=0;i<pmushrooms.size();i++){
            drawMushroom(window,&pmushrooms[i],mushSprite);
        }
        for(int i=0;i<s;i++){
            drawMushroom(window,&mush[i],mushSprite);
        }

        if(moveClock.getElapsedTime().asMilliseconds() > 200){
        for(int c = 0; c < centipedes.size(); c++)
        {
            movecentepede(&centipedes[c][0], centipedes[c].size());
        }
            moveClock.restart();
        }
        //centepede bullet collision
for(int c = 0; c < centipedes.size(); c++)
{
    bool splitOccurred = false;
    for(int i = 0; i < centipedes[c].size(); i++)
    {
      if(bullet[exists] == true){
		float bx = bullet[x] + 16;
		float by = bullet[y] + 16;

		if(bx >= centipedes[c][i].getx() && bx <= centipedes[c][i].getx() + 32 &&
		   by >= centipedes[c][i].gety() && by <= centipedes[c][i].gety() + 32)
		{ 
            if(by>=resolutionY - boxPixelsY*5 && by<resolutionY){
            gameGrid[centipedes[c][i].getx()/32][centipedes[c][i].gety()/32] = 2;//poisonous mushroom is created here.
            pmushrooms.push_back(mushroom(centipedes[c][i].getx(), centipedes[c][i].gety()));
            }
            if(centipedes[c][i].gettype()=="head"){
                score+=20;
            }else{
                score+=10;
            }
			bullet[exists] = false;

			vector<centepede> cent = centipedes[c];

			vector<centepede> firstPart(cent.begin(), cent.begin() + i);
			vector<centepede> secondPart(cent.begin() + i + 1, cent.end());

			centipedes.erase(centipedes.begin() + c);

			if(firstPart.size() > 0)
			{
			    firstPart[firstPart.size() - 1].settype("head");
			    centipedes.push_back(firstPart);
			}

			if(secondPart.size() > 0)
			{
			    secondPart[secondPart.size() - 1].settype("head");
			    centipedes.push_back(secondPart);
			}

			splitOccurred = true;
			break;
	    	}
      }
    }
    if(splitOccurred)
        c--;
}
    for(int c = 0; c < centipedes.size(); c++)
   {
    drawcentepede(window, &centipedes[c][0], head, body, centipedes[c].size());
   }
        
    if(pclock.getElapsedTime().asMilliseconds() > 80){
           moveplayer(player);
            pclock.restart();
    }
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
                        score+=5;
						gameGrid[mush[i].getx()/32][mush[i].gety()/32] = 0;
                        for(int j=i;j<s-1;j++){
                            mush[j]=mush[j+1];
	   	}
                        s--;
                        i--;
                    }
                }
            }
        }
        //player and mushroom collision
for(int i=0;i<pmushrooms.size();i++){
            if(player[x]<pmushrooms[i].getx()+32 && player[x]+16>pmushrooms[i].getx() &&
               player[y]<pmushrooms[i].gety()+32 && player[y]+16>pmushrooms[i].gety())
            {
                flag=false;
                break;
            }
        }


//playr and centepede collision
    for(int c = 0; c < centipedes.size(); c++){
    for(int i = 0; i < centipedes[c].size(); i++){
        if(player[x]+16>=centipedes[c][i].getx() && player[x]+16<=centipedes[c][i].getx()+32 &&
           player[y]+16>=centipedes[c][i].gety() && player[y]+16<=centipedes[c][i].gety()+32)
        {
            flag=false;
             break;
        }
    }
    }
        window.display();
    }

if(!flag){
    while(window.isOpen()){
        while(auto event = window.pollEvent()){
            if(event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        drawgameover(window,gameover);
        window.display();
    }
}
    delete[] mush;

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
void drawgameover(sf::RenderWindow& window, sf::Sprite& gameover)
{
    gameover.setPosition(sf::Vector2f(0, 0));
    window.draw(gameover);
}

void moveplayer(float player[])
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && player[x] > 0)
        player[x] -= 32;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) &&
        player[x] < resolutionX - boxPixelsX)
        player[x] += 32;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) &&
        player[y] > resolutionY - (boxPixelsY * 5))
        player[y] -= 32;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) &&
        player[y] < resolutionY - boxPixelsY)
        player[y] += 32;
}

void drawcentepede(sf::RenderWindow& window, centepede* c,sf::Sprite& centipedehead,sf::Sprite& centipedesbody,int l){
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
    int head = l - 1;

    int *oldx=new int[l];
    int *oldy=new int[l];

    for(int i = 0; i < l; i++)
    {
        oldx[i] = c[i].getx();
        oldy[i] = c[i].gety();
    }

    int nextX;
    int nextY = c[head].gety();

    if(c[head].getdirection() == 1)
        nextX = c[head].getx() + 32;
    else
        nextX = c[head].getx() - 32;

    bool needTurn = false;

    if(nextX < 0 || nextX >= resolutionX){
        needTurn = true;
    }
    else if(gameGrid[nextX / 32][nextY / 32] != 0){
        needTurn = true;
    }

    if(needTurn)
    {
        if(nextY >= resolutionY - boxPixelsY)
        {
            c[head].switchdirection();
        }
        else
        {
            int newY = nextY + boxPixelsY;
            if(newY >= resolutionY)
                newY = resolutionY - boxPixelsY;
            c[head].sety(newY);
            c[head].switchdirection();
        }
    }
    else
    {
        c[head].setx(nextX);
    }

    // Body follows previous positions
    for(int i = l - 2; i >= 0; i--)
    {
        c[i].setx(oldx[i + 1]);
        c[i].sety(oldy[i + 1]);
    }

    delete[] oldx;
    delete[] oldy;
}