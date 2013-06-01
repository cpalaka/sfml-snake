#include "Game.h"

Game::Game() :
	currentState(gStart),
	snkdir(Left),
	food5present(0),
	food10present(0),
	food50present(0),
	score(0)
{
	sf::Time tt = gameTimer.getElapsedTime();
	std::srand(tt.asMilliseconds());
	//initialize snake body (length 6, starting at [x:25,y:25])
	sf::Vector2i v(25, 25);
	for(int i=0; i<5; i++)
	{
		snake.push_back(v);
		v.x++;
	}

	foodpos[0].x = 1000;
	foodpos[0].y = 1000;
	foodpos[1].x = 1001;
	foodpos[1].y = 1001;
	foodpos[2].x = 1002;
	foodpos[2].y = 1002;

	timedelay10 = 5 + rand()%10;
	std::srand(time(0));
	timedelay50 = 20 + rand()%10;
	//load snake body.png and set sprite to it
	if(!snakebody.loadFromFile("resources/snakebod.png"))
	{
		std::cout<<"Error loading image."<<std::endl;
	}	
	snkspr.setTexture(snakebody);

	//load game over screen
	GameOverScreen.loadFromFile("resources/gameover.png");
	GOS.setTexture(GameOverScreen);
	bgtext.loadFromFile("resources/bg.png");
	bg.setTexture(bgtext);

	scorefont.loadFromFile("arial.ttf");
	s_score.setFont(scorefont);
	s_score.setCharacterSize(20);
	s_score.setColor(sf::Color(0,0,0));
	s_score.setPosition(2, 2);

	intro.setFont(scorefont);
	intro.setCharacterSize(30);
	intro.setColor(sf::Color(0,0,0));
	intro.setPosition(30, 30);
	intro.setString("PRESS SOMETHING\n\n\nEAT FOOD:\nRED=5\nGREEN=10\nGOLD=50\n");

	//load food.png and set sprite
	food5_t.loadFromFile("resources/apple.png");
	food50_t.loadFromFile("resources/goldenapple.png");
	food10_t.loadFromFile("resources/grapes.png");
	food10.setTexture(food10_t);
	food5.setTexture(food5_t);
	food50.setTexture(food50_t);
	food50.setPosition(1000, 1000);
	food10.setPosition(1000, 1001);

	//load soundbufferss and set sound
	munch.loadFromFile("resources/munch.wav");
	_munch.setBuffer(munch);
	pop.loadFromFile("resources/pop.wav");
	_pop.setBuffer(pop);
	die.loadFromFile("resources/die.wav");
	_die.setBuffer(die);

	//create window
	window.create(sf::VideoMode(500, 500), "Snake");
}

void Game::Start()
{
	//window.setFramerateLimit(5);
	while(currentState != Exit)
	{
		gameloop();
	}
	window.close();
}

void Game::gameloop()
{
	if(currentState == gStart)
	{
		sf::Event evt;
		window.pollEvent(evt);
		switch(evt.type)
		{
		case sf::Event::Closed:
			currentState = Exit;
			break;
		case sf::Event::KeyPressed:
			gameTimer.restart();
			currentState = Playing;
			break;
		case sf::Event::MouseButtonPressed:
			gameTimer.restart();
			currentState = Playing;
			break;
		}

		//Draw opening screen below
		window.clear(sf::Color(255,255,255));
		window.draw(intro);
		window.display();
	}
	if(currentState == Playing)
	{
		sf::Event evt;
		window.pollEvent(evt);
		switch(evt.type)
		{
		case sf::Event::Closed:
			currentState=Exit;
			break;

		case sf::Event::KeyPressed:
			switch(evt.key.code)
			{
			case sf::Keyboard::Escape:
				currentState=Exit;
				break;

			case sf::Keyboard::Left:
				if(snkdir == Up||snkdir == Down)
				{
					moveSnake(1);
					snkdir = Left;
				}
				break;

			case sf::Keyboard::Right:
				if(snkdir == Up||snkdir == Down)
				{
					moveSnake(2);
					snkdir = Right;
				}
				break;
	
			case sf::Keyboard::Up:
				if(snkdir == Left||snkdir == Right)
				{
					moveSnake(3);
					snkdir = Up;
				}
				break;

			case sf::Keyboard::Down:
				if(snkdir == Left||snkdir == Right)
				{
				moveSnake(4);
				snkdir = Down;
				}
				break;

			default:
				if(snkdir == Left)
					moveSnake(1);
				if(snkdir == Right)
					moveSnake(2);
				if(snkdir == Up)
					moveSnake(3);
				if(snkdir == Down)
					moveSnake(4);
				break;
		}
		break;

		default:
		if(snkdir == Left)
			moveSnake(1);
		if(snkdir == Right)
			moveSnake(2);
		if(snkdir == Up)
			moveSnake(3);
		if(snkdir == Down)
			moveSnake(4);
			break;
		}

		window.clear(sf::Color(255,255,255));

		//create the foods
		if(!food5present)
		{
			createFood5();
		}

		//different timers for the foods and gametime
		sf::Time t50 = food50timer.getElapsedTime();
		sf::Time t10 = food10timer.getElapsedTime();
		sf::Time _t = gameTimer.getElapsedTime();
		srand(_t.asMilliseconds());
		//food50 appears every 40-60 sec
		if(_t.asSeconds() > 20 && t50.asSeconds() > timedelay50 && !food50present)
		{
			createFood50();
		}
		if(t50.asSeconds() > timedelay50 + 5)
		{
			food50.setPosition(1000, 1000);
			foodpos[2].x = 1000;
			foodpos[2].y = 1000;
			food50present = false;
			food50timer.restart();
		}
		
		//food10 appears every 10-15 sec
		if(_t.asSeconds() >  10 && t10.asSeconds() > timedelay10 && !food10present)
		{
			createFood10();
		}
		if(t10.asSeconds() > timedelay10 + 6)
		{
			food10.setPosition(1000, 1001);
			foodpos[1].x = 1000;
			foodpos[1].y = 1001;
			food10present = false;
			food10timer.restart();
		}
		//set score string
		std::stringstream s;
		std::string score1;
		s<<score;
		score1 = s.str();
		s_score.setString("Score: " + score1);
		checkCollision();
		drawAll();
		window.display();
		
		//control the fps
		sf::Time t = sf::milliseconds(60);
		sf::sleep(t);
	}
	else if(currentState == GameOver)
	{
		sf::Event evt;
		window.pollEvent(evt);
		if(evt.type == sf::Event::KeyPressed)
		{
			if(evt.key.code == sf::Keyboard::Escape||evt.key.code == sf::Keyboard::Q)
			{
				currentState = Exit;
			}
		}
		if(evt.type == sf::Event::Closed)
		{
			currentState = Exit;
		}
		if(evt.type == sf::Event::MouseButtonPressed)
		{
			currentState = Exit;
		}
		std::stringstream s1;
		std::string score2;
		s1<<score;
		score2 = s1.str();
		s_score.setString("Final Score: " + score2);
		s_score.setPosition(2, 2);

		window.clear(sf::Color(255,255,255));
		window.draw(GOS);
		window.draw(s_score);
		window.display();
	}
}

bool Game::checkFoodOverlap(int q)
{
	bool val = false;
	for(int i=0; i<3; i++)
	{
		if(i != q)
		{
			if(foodpos[q] == foodpos[i])
			{
				val = true;
			}
		}
	}
	return val;
}

void Game::createFood5()
{
	sf::Time tt = gameTimer.getElapsedTime();
	std::srand(tt.asMilliseconds());
	foodpos[0].x = (rand()%50);
	foodpos[0].y = (rand()%50);
	if(checkFoodxy(foodpos[0]) || checkFoodOverlap(0))
	{
		createFood5();
		return;
	}
	food5.setPosition(foodpos[0].x*10, foodpos[0].y*10);
	food5present = true;
}

void Game::createFood50()
{
	sf::Time tt = gameTimer.getElapsedTime();
	std::srand(tt.asMilliseconds());
	foodpos[2].x = (rand()%50);
	foodpos[2].y = (rand()%50);
	if(checkFoodxy(foodpos[2]) || checkFoodOverlap(2))
	{
		createFood50();
		return;
	}
	food50.setPosition(foodpos[2].x*10, foodpos[2].y*10);
	food50present = true;
	_pop.play();
}

void Game::createFood10()
{
	sf::Time tt = gameTimer.getElapsedTime();
	std::srand(tt.asMilliseconds());
	foodpos[1].x = (rand()%50);
	foodpos[1].y = (rand()%50);
	if(checkFoodxy(foodpos[1]) || checkFoodOverlap(1))
	{
		createFood10();
		return;
	}
	food10.setPosition(foodpos[1].x*10, foodpos[1].y*10);
	food10present = true;
	_pop.play();
}

bool Game::checkFoodxy(sf::Vector2i pos)
{
	bool val = false;
	std::vector<sf::Vector2i>::iterator itr = snake.begin();
	for(itr; itr != snake.end(); itr++)
	{
		if(pos == (*itr))
		{
			val = true;
		}
	}

	return val;
}

void Game::drawAll()
{
	//draw the snake
	window.draw(bg);
	drawSnake();
	window.draw(food5);
	window.draw(food10);
	window.draw(food50);
	window.draw(s_score);
}

void Game::drawSnake()
{
	//std::vector<sf::Vector2i>::iterator itr = snake.begin();
	for(int i = 0; i<snake.size(); i++)
	{
		int s_x = snake[i].x;
		int s_y = snake[i].y;
		snkspr.setPosition(s_x*10, s_y*10);
		window.draw(snkspr);
	}
}

void Game::moveSnake(int dir)
{
	for(int i=snake.size()-1; i>0; i--)
	{
		snake[i].x = snake[i-1].x;
		snake[i].y = snake[i-1].y;
	}
	switch(dir)
	{
	case 1://LEFT
		if(snake[0].x == 0)
		{
			snake[0].x=49;
		} else {
			snake[0].x--;
		}
		break;
	case 2://RIGHT
		if(snake[0].x == 49)
		{
			snake[0].x=0;
		} else {
			snake[0].x++;
		}
		break;
	case 3://UP
		if(snake[0].y == 0)
		{
			snake[0].y=49;
		} else {
			snake[0].y--;
		}
		break;
	case 4://DOWN
		if(snake[0].y == 49)
		{
			snake[0].y=0;
		}
		else {
			snake[0].y++;
		}
		break;
	}
}

void Game::checkCollision()
{
	//first check for food
	if(snake[0] == foodpos[0])
	{
		_munch.play();
		food5present = false;
		for(int i=0; i<2; i++)
		{
			snakeExtend();
		}
		score += 5;
	}
	if(snake[0] == foodpos[2])
	{
		_munch.play();
		//srand(time(0));
		food50present = false;
		food50timer.restart();
		food50.setPosition(1000, 1000);
		for(int i=0; i<6; i++)
		{
			snakeExtend();
		}
		score += 50;
		timedelay50 = 20 + rand()%10;
	}
	if(snake[0] == foodpos[1])
	{
		_munch.play();
		//srand(time(0));
		food10present = false;
		food10timer.restart();
		food10.setPosition(1000, 1001);
		for(int i=0; i<3; i++)
		{
			snakeExtend();
		}
		score += 10;
		timedelay10 = 5 + rand()%10;
	}

	//check if snake hits itself
	for(int i = 2; i<snake.size(); i++)
	{
		if(snake[0] == snake[i])
		{
			currentState = GameOver;
			_die.play();
			_die.play();
		}
	}
}

void Game::snakeExtend()
{
	int newxpos = snake[snake.size()-1].x;
	int newypos = snake[snake.size()-1].y;
	sf::Vector2i newbod(newxpos, newypos);
	snake.push_back(newbod);
}