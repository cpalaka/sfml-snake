#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>

#define GWIDTH 50;
#define GLENGTH 50;

class Game {
public:
	Game();
	void Start();
	void gameloop();
	void drawAll();
	void drawSnake();
	void moveSnake(int dir);
	void createFood5();
	void createFood50();
	void createFood10();
	void checkCollision();
	void snakeExtend();
	bool checkFoodOverlap(int);
	bool checkFoodxy(sf::Vector2i);

	enum gamestate{gStart, Playing, GameOver, Exit};
	enum snakeDirection{Left, Up, Right, Down};

	gamestate currentState;
	snakeDirection snkdir;
	std::vector<sf::Vector2i> snake;

private:
	bool food5present;
	bool food10present;
	bool food50present;
	int score;
	sf::SoundBuffer munch;
	sf::SoundBuffer pop;
	sf::SoundBuffer die;
	sf::Sound _die;
	sf::Sound _munch;
	sf::Sound _pop;
	sf::Text intro;
	sf::Text s_score;
	sf::Font scorefont;
	sf::Vector2i foodpos[3];
	sf::RenderWindow window;
	sf::Texture snakebody;
	sf::Texture GameOverScreen;
	sf::Texture food5_t;
	sf::Texture food10_t;
	sf::Texture food50_t;
	sf::Texture bgtext;
	sf::Sprite bg;
	sf::Sprite GOS;
	sf::Sprite snkspr;
	sf::Sprite food5;
	sf::Sprite food10;
	sf::Sprite food50;
	sf::Clock gameTimer;
	sf::Clock food5timer;
	sf::Clock food10timer;
	sf::Clock food50timer;
	int timedelay10;
	int timedelay50;
};