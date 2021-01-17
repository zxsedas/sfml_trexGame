#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGl.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include <string.h>

using namespace std;

class Trex
{
private:
	sf::Texture texture_trex;
	sf::Sprite sprite_trex;
	float trexRunRate = 0.f;               //恐龍跑的速率
	int trex_change_run = 88;      //恐龍跑的動畫img3

	float trexJumpRate = 0.3f;
	float allMove = 0;
	int trexStop = 0;
	
public:
	Trex(float upMove = 0)
	{
		this->allMove = upMove;
		if (!texture_trex.loadFromFile("img/1x-trex.png"))
		{
			cout << "1x-trex.png load filed!\n";
		}


		sprite_trex.setTexture(texture_trex);
		sprite_trex.setPosition(sf::Vector2f(50.f, 530.f - allMove)); //trex
	}
	sf::Sprite run();
	sf::Sprite jump();
	void Stop();
	int stopW = 0;                        //不讓恐龍重複跳躍
	int jumpFlat = 0;                    //控制跳躍迴圈
	float trexJumpTime = 530;   //跳躍的距離,在地板上是530
	float datumPoint = 520;


};
sf::Sprite Trex::run()
{
	if (trexStop == 0)
	{
		sprite_trex.setTextureRect(sf::IntRect(trex_change_run, 0, 43, 47));
		if (trex_change_run == 176)
			trex_change_run = 88;              //run img1
		if (trexRunRate > 9)
		{
			trex_change_run += 44;            //run img2
			trexRunRate = 0;
		}
		trexRunRate += 0.2;
	}

	return sprite_trex;
}
sf::Sprite Trex::jump()
{
	if (trexStop == 0)
	{
		if (jumpFlat)
		{
			sprite_trex.setPosition(sf::Vector2f(50.f, trexJumpTime - allMove));
			trexJumpTime -= 0.62 + trexJumpRate;
			if (trexJumpTime < 440)
				jumpFlat = 0;
		}
		if (!jumpFlat && trexJumpTime < 530)
		{
			sprite_trex.setPosition(sf::Vector2f(50.f, trexJumpTime - allMove));
			trexJumpTime += 0.323 + trexJumpRate;

		}

		if (trexJumpTime >= 530)
			stopW = 0;
	}

	return sprite_trex;
}
void Trex::Stop()
{
	trexStop = 1;
}

class terrain
{
private:
	sf::Texture texture_ground, texture_cloud, texture_obstacle, texture_score[10], texture_game_over, texture_light;
	sf::Sprite sprite_ground, sprite_cloud, sprite_obstacle, sprite_score[10], sprite_game_over, sprite_light;
	float ground = 0.f;            //地板range
	float groundRate = 0.5f;            //地板跑的速率          
	float cloudLeftPos = 740.f;    //雲朵更新時間
	float obstacleType[7][2] = { {0,53},{0,102},{0,150},{0,200},{98,200},{98,100},{148,104} };
	int random[100], Index = 0;
	float obstacleHeight = 100.f;

	float allMove = 0;                                              //整體移動
	float scoreX = 500, scoreY = 320;                    //score pos
	int terrainStop = 0;                                          //圖片暫停移動

	float gameOverX = 300, gameOverY = 400;           //game over img pos

	float lightX = 800, lightY = 480;              //light init x,y
	
	int lightRandom[100];             //light pos *
	int lightIndex = 0;

	float allSpeed = 0;
public:
	terrain(float upMove,int ran)
	{
		this->allMove = upMove;
		srand(ran);
		for (int i = 0; i < 100; i++)
		{
			random[i] = rand() % (6 - 1 + 1) + 1; //1~6亂數
			lightRandom[i] = rand() % (10 - 1 + 1) + 1;
		}
		if (!texture_ground.loadFromFile("img/3x-horizon.png"))
		{
			cout << "3x-horizon.png load filed!\n";
		}
		if (!texture_cloud.loadFromFile("img/1x-cloud.png"))
		{
			cout << "1x-cloud.png load filed!\n";
		}
		if (!texture_obstacle.loadFromFile("img/2x-obstacle-large.png"))
		{
			cout << "2x-obstacle-large.png load filed!\n";
		}
		if (!texture_game_over.loadFromFile("img/2x-text.png"))
		{
			cout << "2x-text.png load filed!\n";
		}
		for (int i = 0; i < 10; i++)
		{
			if (!texture_score[i].loadFromFile("img/2x-text.png"))
			{
				cout << "score " << i << " .png load filed!\n";
			}
			texture_score[i].setSmooth(true);
			sprite_score[i].setTexture(texture_score[i]);                                               //score
			sprite_score[i].scale(sf::Vector2f(0.5f, 0.5f));
			sprite_score[i].setTextureRect(sf::IntRect(i * 20, 0, 18, 24));
			sprite_score[i].setPosition(sf::Vector2f(scoreX, scoreY - allMove));
		}
		if (!texture_light.loadFromFile("img/light.png"))
		{
			cout << "light load faild" << endl;
		}

		texture_game_over.setSmooth(true);
		texture_obstacle.setSmooth(true);
		texture_light.setSmooth(true);

		sprite_ground.setTexture(texture_ground);
		sprite_ground.setPosition(sf::Vector2f(0.f, 560.f - allMove));//ground

		sprite_cloud.setTexture(texture_cloud);
		sprite_cloud.setPosition(sf::Vector2f(740.f, 460.f - allMove)); //cloud

		sprite_obstacle.setTexture(texture_obstacle);
		sprite_obstacle.setPosition(sf::Vector2f(obstacleMove, 540.f - allMove)); //obstacle
		sprite_obstacle.scale(sf::Vector2f(0.4f, 0.4f));

		sprite_game_over.setTexture(texture_game_over);
		sprite_game_over.setTextureRect(sf::IntRect(0, 25, 400, 25));
		sprite_game_over.scale(sf::Vector2f(0.4f, 0.4f));
		sprite_game_over.setPosition(sf::Vector2f(gameOverX, gameOverY - allMove));          //game over img

		sprite_light.setTexture(texture_light);
		sprite_light.scale(sf::Vector2f(0.6f, 0.6f));
		sprite_light.setPosition( sf::Vector2f(lightX, lightY- allMove) );
	}
	sf::Sprite field();
	sf::Sprite cloud();
	sf::Sprite obstacle();
	sf::Sprite light();
	sf::Sprite score(int, float);
	sf::Sprite gameOver();
	void Stop();
	void Speed(float);
	float obstacleMove = 800.f;
	float lightMove = 800;
};
sf::Sprite terrain::field()
{
	if (terrainStop == 0)
	{
		if (ground >= 1200)
			ground = 0;
		sprite_ground.setTextureRect(sf::IntRect(ground, 0, 2400, 12));//切割ground img
		ground += 0.3 + groundRate + allSpeed;
	}

	return sprite_ground;
}
sf::Sprite terrain::cloud()
{
	if (terrainStop == 0)
	{
		if (cloudLeftPos <= -20)                    //循環雲朵
		{
			cloudLeftPos = 740.f;
		}
		sprite_cloud.setPosition(sf::Vector2f(cloudLeftPos, 460.f - allMove));
		cloudLeftPos -= 0.1;                     //雲朵移動速率  
	}

	return sprite_cloud;
}
sf::Sprite terrain::obstacle()
{
	if (terrainStop == 0)
	{
		if (Index == 100)
			Index = 0;
		if (obstacleMove < 0)
		{
			obstacleMove = 740;
			Index++;
		}
		sprite_obstacle.setPosition(sf::Vector2f(obstacleMove, 540.f - allMove));
		sprite_obstacle.setTextureRect(sf::IntRect(obstacleType[random[Index]][0], 0, obstacleType[random[Index]][1], obstacleHeight));

		obstacleMove -= 0.8 + allSpeed;
	}
	return sprite_obstacle;
}
sf::Sprite terrain::light()
{
	if (terrainStop == 0)
	{
		if (lightIndex == 100)
		{
			lightIndex = 0;
		}
		if (lightMove <= 0)
		{
			lightMove = 800 + (1000 * lightRandom[ lightIndex++ ]);
		}
		sprite_light.setPosition(sf::Vector2f(lightMove, lightY - allMove));
		lightMove -= 0.8 + allSpeed;
	}
	
	return sprite_light;
}
sf::Sprite terrain::score(int scoreIndex, float offsetX = 0)
{

	sprite_score[scoreIndex].setPosition(sf::Vector2f(scoreX - offsetX, scoreY - allMove));

	return sprite_score[scoreIndex];
}
sf::Sprite terrain::gameOver()
{
	return sprite_game_over;
}
void terrain::Stop()
{
	terrainStop = 1;
}
void terrain::Speed(float s)
{
	allSpeed += s;
}

class Game
{
private:
	sf::RenderWindow& window;

	sf::Texture texture_win;
	sf::Sprite  Sprite_win;
	sf::Texture texture_lose;
	sf::Sprite  Sprite_lose;
	sf::Texture texture_tie;
	sf::Sprite  Sprite_tie;

	sf::Texture texture_P1Score;
	sf::Sprite  Sprite_P1Score;
	sf::Texture texture_P2Score;
	sf::Sprite  Sprite_P2Score;

	terrain* t = new terrain(0,5);                          //下面畫面的土地
	terrain* t2 = new terrain(300,55);                 //上面畫面的土地
	Trex* trex = new Trex();                          //under trex
	Trex* trex2 = new Trex(300);                  //above trex

	char data[10];
	char sendW[10] = { 'w' };                         //sendW
	char sendI[10] = { 'i' };                            //sendI

	char sendStop1[10] = { 's', '1' };                     //snedS
	char sendStop2[10] = { 's', '2' };                     //snedS

	int recv1 = 0;
	int recv2 = 0;
	char sendScore1[10];
	char sendScore2[10];

	

	int gameOver = 0;                                     //self
	int gameOver2 = 0;                                  //p2

	std::size_t received;
	sf::Thread thread;


	float scoreRate = 0;                                   //計算分數的速度p1
	
	float scoreRate2 = 0;                                   //計算分數的速度p2
	
	int night = 0;
public:

	Game(sf::RenderWindow&);
	void run();
	void processInput();
	void update();
	void render();
	void recv();
	void send(char[]);
	void drawScore(terrain* t, int&);

	sf::TcpSocket* client;
	sf::TcpSocket* socket;

	int asServer = 0;
	int end = 0;
	int score = 0;
	int score2 = 0;
};
Game::Game(sf::RenderWindow& win) :window(win), thread(&Game::recv, this)
{
	texture_win.loadFromFile("img/win.png");
	texture_lose.loadFromFile("img/lose.png");
	texture_tie.loadFromFile("img/tie.png");
	texture_P1Score.loadFromFile("img/p1score.png");
	texture_P2Score.loadFromFile("img/p2score.png");

	Sprite_win.setTexture(texture_win);
	Sprite_lose.setTexture(texture_lose);
	Sprite_tie.setTexture(texture_tie);
	Sprite_P1Score.setTexture(texture_P1Score);
	Sprite_P2Score.setTexture(texture_P2Score);

	Sprite_win.setPosition(sf::Vector2f(0.f, 400.f));
	Sprite_lose.setPosition(sf::Vector2f(0.f, 400.f));
	Sprite_tie.setPosition(sf::Vector2f(0.f, 400.f));
	Sprite_P1Score.setPosition(sf::Vector2f(-50.f, 160.f));
	Sprite_P2Score.setPosition(sf::Vector2f(-50.f, -140.f));
}
void Game::run()
{
	thread.launch();            //multiple recv

	while (window.isOpen())
	{
		processInput();
		update();
		render();
		sf::sleep(sf::Time(sf::milliseconds(1)));
	}
}
void Game::processInput()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
	if (asServer == 1)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && trex->stopW == 0 && gameOver == 0)
		{
			send(sendW);
			trex->jumpFlat = 1;
			trex->stopW = 1;

		}
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) && trex2->stopW == 0 && gameOver2 == 0)
		{
			send(sendI);
			trex2->jumpFlat = 1;
			trex2->stopW = 1;

		}
	}

}
void Game::render()
{
	
	if (gameOver == 1 && gameOver2 == 1)                          //遊戲統計畫面
	{
		if(end == 0)
			sf::sleep(sf::Time(sf::seconds(1)));
		window.clear(sf::Color::White);
		if (score > score2 && asServer == 1)
		{
			window.draw(Sprite_win);
			window.draw(Sprite_P1Score);
			window.draw(Sprite_P2Score);
			drawScore(t, score);
			drawScore(t2, score2);
		}
		else if (score < score2 && asServer == 1)
		{
			window.draw(Sprite_lose);
			window.draw(Sprite_P1Score);
			window.draw(Sprite_P2Score);
			drawScore(t, score);
			drawScore(t2, score2);
		}
		if (score > score2 && asServer == 0)
		{
			window.draw(Sprite_lose);
			window.draw(Sprite_P1Score);
			window.draw(Sprite_P2Score);
			drawScore(t, score);
			drawScore(t2, score2);
		}
		else if (score < score2 && asServer == 0)
		{
			window.draw(Sprite_win);
			window.draw(Sprite_P1Score);
			window.draw(Sprite_P2Score);
			drawScore(t, score);
			drawScore(t2, score2);
		}
		if (score == score2)
		{
			window.draw(Sprite_tie);
			window.draw(Sprite_P1Score);
			window.draw(Sprite_P2Score);
			drawScore(t, score);
			drawScore(t2, score2);
		}
		end++;
	}
	if(gameOver == 0 || gameOver2 == 0)
	{
		if (score >= 2000 || score2 >= 2000)
		{
			
			window.clear(sf::Color(255 - night, 255 - night, 255 - night));
			if (night < 255)
				night++;
			
		}
		else
		{
			window.clear(sf::Color::White);
		}
		
		window.draw(t->field());
		window.draw(t->obstacle());
		window.draw(t->cloud());
		window.draw(t->light());
		if (gameOver == 1)                                        //draw game over
			window.draw(t->gameOver());
		drawScore(t, score);
		t->Speed(0.00005);                                        //allSpeed

		window.draw(t2->field());
		window.draw(t2->obstacle());
		window.draw(t2->cloud());
		window.draw(t2->light());
		if (gameOver2 == 1)
			window.draw(t2->gameOver());
		drawScore(t2, score2);
		t2->Speed(0.00005);


		window.draw(trex->run());
		window.draw(trex->jump());

		window.draw(trex2->run());
		window.draw(trex2->jump());
	}

	window.display();
}
void Game::update()
{
	if ((int)t->lightMove == 80 && (trex->datumPoint - trex->trexJumpTime) <= 13)
	{
		t2->Speed(0.05);
		
	}
	if ((int)t2->lightMove == 80 && (trex2->datumPoint - trex->trexJumpTime) <= 13)
	{
		t->Speed(0.05);
		
	}
	if ((int)t->obstacleMove >= 14.1 && (int)t->obstacleMove <= 79 && (trex->datumPoint - trex->trexJumpTime) <= 13 && gameOver == 0 && asServer == 1)
	{
		trex->Stop();
		t->Stop();
		gameOver = 1;
		sprintf_s(sendScore1, sizeof(sendScore1),"%d",score);
		send(sendStop1);
		send(sendScore1);

	}
	if ((int)t2->obstacleMove >= 14.1 && (int)t2->obstacleMove <= 79 && (trex2->datumPoint - trex2->trexJumpTime) <= 13 && gameOver2 == 0 && asServer == 0)
	{

		trex2->Stop();
		t2->Stop();
		gameOver2 = 1;
		sprintf_s(sendScore2, sizeof(sendScore2), "%d", score2);
		send(sendStop2);
		send(sendScore2);

	}
	if (gameOver2 == 0)
	{
		if (scoreRate2 >= 8)           //score速度p2
		{
			score2++;
			scoreRate2 = 0;
		}
		scoreRate2 += 0.2 + scoreRate2;
	}
	if (gameOver == 0)
	{
		if (scoreRate >= 8)           //score速度p1
		{
			score++;
			scoreRate = 0;
		}
		scoreRate += 0.2 + scoreRate;
	}

	
	

}
void Game::recv()
{
	if (asServer == 1)                                                              //as 房主
	{
		while (1)
		{
			memset(data, 0, sizeof(data));
			client->receive(data, sizeof(data), received);
			//cout <<"asServer 1:"<<data<< endl;
			if (data[0] == 'i' || data[0] == 'I')
			{
				trex2->jumpFlat = 1;
				trex2->stopW = 1;
			}
			else if ((data[0] == 's' || data[0] == 'S') && (data[1] == '2'))
			{
				gameOver2 = 1;
				recv2 = 1;
				//cout << "gameOver2:" << gameOver2 << endl;
				t2->Stop();
				trex2->Stop();
			}
			else if ((data[0] == 's' || data[0] == 'S') && (data[1] == '1'))
			{
				gameOver = 1;
				cout << "gameOver:" << gameOver << endl;
				t->Stop();
				trex->Stop();
			}
			else if (data[0] >= '0' && data[0] <= '9' && recv2 == 1)
			{
				score2 = atoi(data);
				recv2 = 0;
				//cout <<"socre2:"<< score2 << endl;
			}
			
		}
	}
	else if (asServer == 0)
	{
		while (1)
		{
			memset(data, 0, sizeof(data));
			socket->receive(data, sizeof(data), received);
			//cout <<"asServer 0:"<<data << endl;
			if (data[0] == 'w' || data[0] == 'W')
			{

				trex->jumpFlat = 1;
				trex->stopW = 1;
			}
			else if (data[0] == 's' || data[0] == 'S' && data[1] == '1')
			{
				gameOver = 1;
				recv1 = 1;
				t->Stop();
				trex->Stop();
			}
			else if (data[0] == 's' || data[0] == 'S' && data[1] == '2')
			{
				gameOver2 = 1;
				t2->Stop();
				trex2->Stop();
			}
			else if (data[0] >= '0' && data[0] <= '9' && recv1 == 1)
			{
				score = atoi(data);
				recv1 = 0;
				//cout <<"socre:"<< score << endl;
			}
		}
	}

}
void Game::send(char send[])
{
	if (asServer == 1)
	{
		client->send(send, 10);
	}
	else if (asServer == 0)
	{
		socket->send(send, 10);
	}

}
void Game::drawScore(terrain* t, int& score)
{
	if (score < 10)
	{
		window.draw(t->score(score, 0));
	}
	else if (score >= 10 && score < 100)
	{
		window.draw(t->score(score % 10, 0));
		window.draw(t->score(score / 10, 8));
	}
	else if (score >= 100 && score < 1000)
	{
		window.draw(t->score(score % 10, 0));
		window.draw(t->score((score % 100) / 10, 8));
		window.draw(t->score(score / 100, 16));
	}
	else if (score >= 1000 && score < 10000)
	{
		window.draw(t->score(score % 10, 0));
		window.draw(t->score((score % 100) / 10, 8));
		window.draw(t->score((score % 1000) / 100, 16));
		window.draw(t->score(score / 1000, 24));
	}
	else if (score >= 10000 && score < 100000)
	{
		window.draw(t->score(score % 10, 0));                         //個位
		window.draw(t->score((score % 100) / 10, 8));             //十位
		window.draw(t->score((score % 1000) / 100, 16));       //百位
		window.draw(t->score((score % 10000) /1000, 24));   //千位
		window.draw(t->score(score / 10000, 32));                 //萬
	}
}

class Start
{
private:
	sf::RenderWindow& window;
	sf::Texture texture_2p, texture_room, texture_player1, texture_player2, texture_start, texture_connect, texture_dec1;
	sf::Sprite sprite_2p, sprite_room, sprite_player1, sprite_player2, sprite_start, sprite_connect,sprite_dec1;

	float B2pX = 280, B2pY = 150;                      //2p img pos
	float BconX = 280, BconY = 240;                  //connect img pos
	float RoomX = 130, RoomY = 50;                   //roomBgc img pos
	float p1X = 210, p1Y = 30;                            //p1 img pos
	float p2X = 210, p2Y = 80;                           //p2 img pos
	float startX = 200, startY = 270;                //start img pos
	int enterRoom = 0;                                       //room not enter 0/room enter 1
	int enterP2 = 0;											//p2 entered 1/not 0                                    
	int enterP1 = 0;
	int recvStarted = 0;

	char data[100];
	size_t received;
	sf::Thread thread;
	sf::Thread recvthread;

	sf::TcpSocket client;                                   //client用來接收客戶端的
	sf::TcpSocket socket;                                 //socket本地伺服端
	sf::TcpListener listener;								//listner
	Game* game;                                               //用來把client和socket傳給game class

	int asServer = 0;
public:
	Start(sf::RenderWindow& win) :window(win), thread(&Start::waitConnec, this), recvthread(&Start::recvStart, this)      //multiple thread  wait connect
	{
		if (!texture_2p.loadFromFile("img/2P.png"))
		{
			cout << "2p.png load filed!\n";
		}
		if (!texture_connect.loadFromFile("img/connect.png"))
		{
			cout << "connect.png load filed!\n";
		}
		if (!texture_room.loadFromFile("img/room.png"))
		{
			cout << "room.png load filed!\n";
		}
		if (!texture_player1.loadFromFile("img/player1.png"))
		{
			cout << "p1.png load filed!\n";
		}
		if (!texture_player2.loadFromFile("img/player2.png"))
		{
			cout << "p2.png load filed!\n";
		}
		if (!texture_start.loadFromFile("img/start.png"))
		{
			cout << "start.png load filed!\n";
		}
		if (!texture_dec1.loadFromFile("img/home.png"))
		{
			cout << "home.png load failed\n";
		}
		texture_2p.setSmooth(true);
		texture_room.setSmooth(true);
		texture_player1.setSmooth(true);
		texture_player2.setSmooth(true);
		texture_start.setSmooth(true);
		texture_connect.setSmooth(true);
		texture_dec1.setSmooth(true);

		sprite_2p.setTexture(texture_2p);
		sprite_2p.scale(sf::Vector2f(0.4f, 0.4f));
		sprite_2p.setPosition(sf::Vector2f(B2pX, B2pY));                //2p_sprite

		sprite_connect.setTexture(texture_connect);
		sprite_connect.scale(sf::Vector2f(0.4f, 0.4f));
		sprite_connect.setPosition(sf::Vector2f(BconX, BconY));     //connect_sprite

		sprite_room.setTexture(texture_room);
		sprite_room.scale(sf::Vector2f(1.5f, 1.5f));
		sprite_room.setPosition(sf::Vector2f(RoomX, RoomY));      //room_sprite

		sprite_player1.setTexture(texture_player1);
		sprite_player1.setPosition(sf::Vector2f(p1X, p1Y));             //player1_sprite

		sprite_player2.setTexture(texture_player2);
		sprite_player2.setPosition(sf::Vector2f(p2X, p2Y));           //player2_sprite

		sprite_start.setTexture(texture_start);
		sprite_start.setPosition(sf::Vector2f(startX, startY));      //start_sprite

		sprite_dec1.setTexture(texture_dec1);
		sprite_dec1.setPosition(sf::Vector2f(200, 320));
		sprite_dec1.scale(sf::Vector2f(0.5, 0.5));
	}
	void run()
	{
		while (window.isOpen())
		{
			render();
			processInput();
			if (recvStarted == 1)
				game->run();
		}
	}
	void render()
	{
		if (!enterRoom)                                  //option img
		{
			option();
			window.display();
		}
		else                                                   //room img
		{
			window.clear(sf::Color(255, 201, 107));
			window.draw(room());
			window.draw(p1());
			if (enterP2 && enterP1)                                  //show player2
			{
				window.draw(p2());
			}
			if (asServer == 1 && enterP2 == 1)
			{
				window.draw(start());
			}
			window.display();
		}


	}
	void option()
	{
		window.clear(sf::Color(243, 249, 255));
		window.draw(sprite_2p);
		window.draw(sprite_connect);
		window.draw(sprite_dec1);
	}
	void processInput()
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if ((event.mouseButton.x >= 288 && event.mouseButton.x <= 417) && (event.mouseButton.y >= 199 && event.mouseButton.y <= 244))//開房
					{
						game = new Game(window);
						asServer = 1;
						enterP1 = 1;
						enterRoom = 1;
						game->asServer = 1;                                                      //當房主
						if (listener.listen(53000) != sf::Socket::Done)              //listen
						{
							cout << "listen failed\n";
						}
						thread.launch(); //waitConnect
					}
					if ((event.mouseButton.x >= 288 && event.mouseButton.x <= 417) && (event.mouseButton.y >= 289 && event.mouseButton.y <= 335))//連接房間
					{
						game = new Game(window);
						asServer = 0;
						enterRoom = 1;
						game->asServer = 0;
						enterP1 = 1;
						enterP2 = 1;                                                     //p2 enter
						Connect();                                                        //連接
						recvthread.launch();
						
					}
				}
			}
			if (event.type == sf::Event::MouseButtonPressed && (enterRoom == 1)) // 開始遊戲
			{
				if ((event.mouseButton.x >= 281 && event.mouseButton.x <= 414) && (event.mouseButton.y >= 374 && event.mouseButton.y <= 412) && (enterP2 == 1))
				{
					client.send("start", strlen("start"));
					sf::sleep(sf::Time(sf::seconds(0.019)));
					game->run();                     //開始遊戲
					
				}
			}

		}
	}
	sf::Sprite room()
	{
		return sprite_room;
	}
	sf::Sprite p1()
	{
		return sprite_player1;
	}
	sf::Sprite p2()
	{
		return sprite_player2;
	}
	sf::Sprite start()
	{
		return sprite_start;
	}

	void waitConnec()
	{

		if (listener.accept(client) != sf::Socket::Done)
		{
			cout << "accept failed\n";
			enterP2 = 0;
		}
		else
		{
			cout << "accept success\n";
			game->client = &client;
			enterP2 = 1;
		}

	}
	void Connect()
	{
		socket.connect("127.0.0.1", 53000);
		game->socket = &socket;                        //連接的話,你就是client,所以是傳本地端的socket,不是接受的client端
		

	}
	void recvStart()
	{
		socket.receive(data, sizeof(data), received);
		if (strcmp(data, "start") == 0)
		{
			//cout << "recv start game run" << endl;
			recvStarted = 1;
		}
	}
	void clientStart(int Started)
	{
		if(Started == 1)
			game->run();
	}

};

int main()
{

	sf::RenderWindow window;
	window.create(sf::VideoMode(700, 600), "Start");
	Start start(window);
	//Game game(window);
	//game.run();
	start.run();


	return 0;
}