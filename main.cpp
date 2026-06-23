#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>




using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;


class mushroom{
int mx;
int my;
int health;
public:
mushroom(){
	mx=0;
	my=0;
	health=2;
}
mushroom(int x, int y){
	this->mx = x;
	this->my = y;
}
int getx(){
	return mx;
}
int gety(){
	return my;
}
void sethealth(){
	health-=1;
}
int gethealth(){
	return health;
}
};
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullet(float bullet[], sf::Clock& bulletClock);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
//player movement
void moveplayer(float player[]);
void drawMushroom(sf::RenderWindow& window, mushroom* m, sf::Sprite& mushroomSprite);
//mushroom class




int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode({resolutionX, resolutionY}), "Centipede", sf::Style::Close |sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);

	// Initializing Background.
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("Textures/background.png");
	sf::Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = (gameColumns * 3 / 4) * boxPixelsY;
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Textures/player.png");
	sf::Sprite playerSprite(playerTexture);
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect({0,0}, {boxPixelsX, boxPixelsY}));

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY;
	bullet[exists] = true;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	bulletTexture.loadFromFile("Textures/bullet.png");
	sf::Sprite bulletSprite(bulletTexture);
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect({0,0}, {boxPixelsX, boxPixelsY}));

//mushroom generation
int s=rand() % 11 + 20;
mushroom *mush;
mush=new mushroom[s];
sf::Texture mushroomtexture;
mushroomtexture.loadFromFile("Textures/mushroom.png");
sf::Sprite mushrooms(mushroomtexture);
mushrooms.setTexture(mushroomtexture);
	mushrooms.setTextureRect(sf::IntRect({0,0}, {boxPixelsX, boxPixelsY}));
for(int i = 0; i < s; i++){
mush[i]=mushroom(rand()%gameColumns*32,rand()%(gameRows-5)*32);
gameGrid[mush[i].getx()/32][mush[i].gety()/32]=1;
}


	while(window.isOpen()) {

		//moveplayer with arrows
	

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////
  
		window.draw(backgroundSprite);
		drawPlayer(window, player, playerSprite);
		for(int i = 0; i < s; i++){
			drawMushroom(window, &mush[i], mushrooms);
		}
		moveplayer(player);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && bullet[exists] == false){
			bullet[x] = player[x];
			bullet[y] = player[y] - boxPixelsY;
			bullet[exists] = true;
		}
		if (bullet[exists] == true) {
			moveBullet(bullet, bulletClock);
			drawBullet(window, bullet, bulletSprite);
		}
	 for(int i=0;i<s;i++){
		if(mush[i].gethealth()>0){
		  if(mush[i].getx()==bullet[x] && mush[i].gety()==bullet[y]){
			mush[i].sethealth();
			bullet[exists]=false;
	 		 if(mush[i].gethealth() == 0){
               for(int j = i; j < s-1; j++){
                  mush[j] = mush[j+1];
                }
                  s--; 
             }

		  }
		}
	 }
	    
		std::optional<sf::Event> event;
		while (const std::optional event = window.pollEvent()) {

          if (event->is<sf::Event::Closed>()) {
            window.close();
          }
 
       }	
		window.display();
		window.clear();
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(sf::Vector2f(player[x], player[y]));
	window.draw(playerSprite);
}
void drawMushroom(sf::RenderWindow& window, mushroom* m, sf::Sprite& mushroomSprite){
mushroomSprite.setPosition(sf::Vector2f(m->getx(), m->gety()));
window.draw(mushroomSprite);
}

void moveBullet(float bullet[], sf::Clock& bulletClock) {
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
		return;

	bulletClock.restart();
	bullet[y] -= 10;	
	if (bullet[y] < -32)
		bullet[exists] = false;
}
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
    bulletSprite.setPosition(sf::Vector2f(bullet[x], bullet[y]));
	window.draw(bulletSprite);
}
void moveplayer(float player[]) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && player[x] > 0){
		player[x] -= 0.5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && player[x] < resolutionX - boxPixelsX)
		player[x] += 0.5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && player[y] > 0)
	{
		if(player[y]< (boxPixelsY*5)){
	       player[y] -=0.5;
		}else{
        player[y] = (boxPixelsY*5);
	    }
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && player[y] < resolutionY - boxPixelsY){
		player[y] += 0.5;
	}
}





