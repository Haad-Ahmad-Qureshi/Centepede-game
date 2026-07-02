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
bool isClicked(sf::RectangleShape &btn, sf::RenderWindow &window)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        if (btn.getGlobalBounds().contains(worldPos))
        {
            return true;
        }
    }
    return false;
}
// FUNCTIONS
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& sprite);
void moveplayer(float player[]);
void moveBullet(float bullet[], sf::Clock& clock);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& sprite);
void drawMushroom(sf::RenderWindow& window, mushroom* m, sf::Sprite& sprite);
void drawcentepede(sf::RenderWindow& window, centepede* c,sf::Sprite& head,sf::Sprite& body,int l);
void movecentepede(centepede* c, int l);
void drawgameover(sf::RenderWindow& window, sf::Sprite& gameover);
void drawwin(sf::RenderWindow& window, sf::Sprite& win);
void drawstartpage(sf::RenderWindow&window, sf::Sprite& startpage);
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
//start page texture//////////////////.......................................^^^^^
sf::Texture startpageTex;
if(!startpageTex.loadFromFile("Textures/start.jfif"))
    cout << "startpage.png failed to load\n";   
    sf::Sprite startpage(startpageTex);
    sf::Vector2u startimgS = startpageTex.getSize();    
    float startx = (float)resolutionX / startimgS.x;
    float starty = (float)resolutionY / startimgS.y;
    startpage.setScale({startx, starty});

//game win texture//////////////////.......................................^^^^^
sf::Texture gamewintex;
if(!gamewintex.loadFromFile("Textures/win.jpeg"))
    cout << "win.jpeg failed to load\n";
sf::Sprite gamewin(gamewintex);
sf::Vector2u imgS = gamewintex.getSize();
float sX = (float)resolutionX / imgS.x;
float sY = (float)resolutionY / imgS.y;
gamewin.setScale({sX, sY});

    ///game over texture//////////////////.......................................^^^^^
sf::Texture gameovertex;
if(!gameovertex.loadFromFile("Textures/over.jpeg"))
    cout << "over.jpeg failed to load\n";
sf::Sprite gameover(gameovertex);
sf::Vector2u imgSize = gameovertex.getSize();
float scaleX = (float)resolutionX / imgSize.x;
float scaleY = (float)resolutionY / imgSize.y;
gameover.setScale({scaleX, scaleY});


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
scoreText.setStyle(sf::Text::Bold);
scoreText.setCharacterSize(30);
scoreText.setFillColor(sf::Color::Red);
scoreText.setPosition({10.f, 10.f});
//total score
sf::Text tscore(font);
tscore.setStyle(sf::Text::Bold);
tscore.setCharacterSize(30);
tscore.setFillColor(sf::Color::Red);
tscore.setPosition({20.f, 50.f});


bool w=false;//for win or lose detection

int cSpeed = 200;
int mushExtra = 0;

sf::RectangleShape restartBtn, exitBtn,startBtn,easy,medium,hard,backBtn;
sf::Text restartText(font), exitText(font),startText(font),easyText(font),mediumText(font),hardText(font),backText(font),instructionsText(font);

// Restart Button
restartBtn.setSize({200, 60});
restartBtn.setFillColor(sf::Color::Green);
restartBtn.setPosition({350, 780});
restartText.setString("Restart");
restartText.setCharacterSize(28);
restartText.setPosition({restartBtn.getPosition().x + 40, restartBtn.getPosition().y + 10});
restartText.setFillColor(sf::Color::White);
// Exit Button
exitBtn.setSize({200, 60});
exitBtn.setFillColor(sf::Color::Red);
exitBtn.setPosition({350, 860});
exitText.setString("Exit");
exitText.setCharacterSize(28);

startBtn.setSize({200, 60});
startBtn.setFillColor(sf::Color::Blue);
startBtn.setPosition({350, 400}); 
startText.setString("Instructions");
startText.setCharacterSize(28);
startText.setPosition({startBtn.getPosition().x + 25, startBtn.getPosition().y + 10});
startText.setFillColor(sf::Color::White);

easy.setSize({200, 60});
easy.setFillColor(sf::Color::Black);
easy.setPosition({350, 500});
easyText.setString("Easy");
easyText.setCharacterSize(28);
easyText.setPosition({easy.getPosition().x + 60, easy.getPosition().y + 10});
easyText.setFillColor(sf::Color::White);

medium.setSize({200, 60});
medium.setFillColor(sf::Color::Black);
medium.setPosition({350, 600});
mediumText.setString("Medium");
mediumText.setCharacterSize(28);
mediumText.setPosition({medium.getPosition().x + 40, medium.getPosition().y + 10});
mediumText.setFillColor(sf::Color::White);

hard.setSize({200, 60});
hard.setFillColor(sf::Color::Black);
hard.setPosition({350, 700});
hardText.setString("Hard");
hardText.setCharacterSize(28);
hardText.setPosition({hard.getPosition().x + 70, hard.getPosition().y + 10});
hardText.setFillColor(sf::Color::White);

backBtn.setSize({200, 60});
backBtn.setFillColor(sf::Color::White);
backBtn.setPosition({350, 850});
backText.setString("Back");
backText.setCharacterSize(28);
backText.setFillColor(sf::Color::Black);
backText.setPosition({backBtn.getPosition().x + 65, backBtn.getPosition().y + 10});

instructionsText.setString(
    "HOW TO PLAY\n\n"
    "Move: LEFT / RIGHT / UP / DOWN arrow keys\n"
    "Shoot: SPACE\n\n"
    "Shoot the centipede to split it and score points\n"
    "Shoot mushrooms to weaken and destroy them\n"
    "Avoid touching the centipede or any mushroom\n"
    "Poisonous mushrooms left behind are deadly - avoid them\n\n"
    "Clear every centipede segment to win!"
);
instructionsText.setCharacterSize(26);
instructionsText.setFillColor(sf::Color::White);
instructionsText.setPosition({60.f, 100.f});

////////////////////////////////////////////////////////////////////////////////////////////////////
    // ---------------- LOOP ----------------
////////////////////////////////////////////////////////////////////////////////////////////////////
while(window.isOpen() && flag==true)
{
   exitBtn.setPosition({350, 820});
   exitText.setPosition({exitBtn.getPosition().x + 65, exitBtn.getPosition().y + 10});
   while(window.isOpen()){
        while(auto event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
    drawstartpage(window,startpage);
//draw button
       window.draw(startBtn);
       window.draw(startText);//making it the intruction button 
        window.draw(easy);
        window.draw(easyText);
        window.draw(medium);
        window.draw(mediumText);
        window.draw(hard);
        window.draw(hardText);
        window.draw(exitBtn);
        window.draw(exitText);

        window.display();

        if(isClicked(startBtn, window)){
            sf::sleep(sf::milliseconds(150)); // prevent double click

            while(window.isOpen()){
                while(auto event = window.pollEvent())
                {
                    if(event->is<sf::Event::Closed>())
                        window.close();
                }

                window.clear(sf::Color::Black);
                window.draw(instructionsText);
                window.draw(backBtn);
                window.draw(backText);
                window.display();

                if(isClicked(backBtn, window)){
                    sf::sleep(sf::milliseconds(150));
                    break;
                }
            }
        }

        if(isClicked(easy, window)){
            sf::sleep(sf::milliseconds(150)); // prevent double click
            cSpeed = 250;
            mushExtra = 0;
            flag=false;
            break;
        }

        if(isClicked(medium, window)){
            sf::sleep(sf::milliseconds(150)); // prevent double click
            cSpeed = 180;
            mushExtra = 10;
            flag=false;
            break;
        }

        if(isClicked(hard, window)){
            sf::sleep(sf::milliseconds(150)); // prevent double click
            cSpeed = 120;
            mushExtra = 20;
            flag=false;
            break;
        }

        if(isClicked(exitBtn, window)){
            window.close();
        }
   }

exitBtn.setPosition({350, 860});
exitText.setPosition({exitBtn.getPosition().x + 65, exitBtn.getPosition().y + 10});

delete[] mush;
s = rand()%11 + 20 + mushExtra;
mush = new mushroom[s];

for(int i=0;i<gameRows;i++)
    for(int j=0;j<gameColumns;j++)
        gameGrid[i][j]=0;

for(int i=0;i<s;i++){
    mush[i] = mushroom(rand()%gameColumns*32, rand()%(gameRows-5)*32);
    gameGrid[mush[i].getx()/32][mush[i].gety()/32] = 1;
}

flag = true;
w = false;
while(window.isOpen())
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

        if(moveClock.getElapsedTime().asMilliseconds() > cSpeed){
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
        //player and poisonous mushroom collision
for(int i=0;i<pmushrooms.size();i++){
            if(player[x]<pmushrooms[i].getx()+32 && player[x]+16>pmushrooms[i].getx() &&
               player[y]<pmushrooms[i].gety()+32 && player[y]+16>pmushrooms[i].gety())
            {
                flag=false;
                break;
            }
        }

if(!flag){
    break;
}

//player and centepede collision
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

if(!flag){
    break;
}

    if(centipedes.size() == 0)
        {
            w=true;
            flag=false;
            break;
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

        if(w){
            drawwin(window,gamewin);
        }else{
            drawgameover(window,gameover);
        }

        tscore.setString("Total Score: " + to_string(score));
        window.draw(tscore);

        //DRAW BUTTONS
        window.draw(restartBtn);
        window.draw(restartText);
        window.draw(exitBtn);
        window.draw(exitText);

        if(isClicked(restartBtn, window))
        {
            flag=true;
            sf::sleep(sf::milliseconds(150)); // prevent double click

            score = 0;

            // reset player
            player[x] = (gameColumns / 2) * boxPixelsX;
            player[y] = resolutionY - boxPixelsY;

            // reset bullet
            bullet[exists] = false;

            // reset centipede
            centipedes.clear();
            vector<centepede> first;

            for(int i = 0; i < 12; i++){
                first.push_back(centepede(i*32, 0, 1, "body"));
            }

            first[11].settype("head");
            centipedes.push_back(first);

            // reset mushrooms
            delete[] mush;
            s = rand()%11 + 20 + mushExtra;
            mush = new mushroom[s];

            // clear grid
            for(int i=0;i<gameRows;i++)
                for(int j=0;j<gameColumns;j++)
                    gameGrid[i][j]=0;

            for(int i=0;i<s;i++){
                mush[i] = mushroom(rand()%gameColumns*32, rand()%(gameRows-5)*32);
                gameGrid[mush[i].getx()/32][mush[i].gety()/32] = 1;
            }

            pmushrooms.clear();

            break;  //VERY IMPORTANT
        }
        if(isClicked(exitBtn, window)){
            window.close();
        }

        window.display();
    }
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
            void drawwin(sf::RenderWindow& window, sf::Sprite& win)
            {
                win.setPosition(sf::Vector2f(0, 0));
                window.draw(win);
            }
            void drawstartpage(sf::RenderWindow&window, sf::Sprite& startpage){
                startpage.setPosition(sf::Vector2f(0, 0));
                window.draw(startpage);
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