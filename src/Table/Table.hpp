#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../Ball/Ball.hpp"
#include "../Cue/Cue.hpp"

#define BALL_COUNT 16
#define SCALE_X	1.16047
#define SCALE_Y 1.22555
#define IN_POCKET 1.5f

class Score;

class Table
{
	// geometrical properties
	sf::Vector2f position;							// geometrical center of the table
	sf::Vector2u dimensions;
	float corner_radius, middle_radius;				// pockets' properties

	std::vector <sf::Vector2f> pockets, borders;	// special origin points for balls, borders, pockets 
	std::vector<Ball> balls;						// the balls of the game
	std::vector<Cue> cue;					// the Cue

	// graphical properties
	sf::Texture texture;
	sf::Sprite sprite;

	friend class Ball;
	friend class Cue;
	friend class Game;

	void SetCueBall( sf::RenderWindow& window, Score& score, int player_number );

public:
	Table() {}
	Table( const sf::VideoMode& video_mode,	const std::string& table_file,
		const std::string& ball_file, const std::string& Cue_file );
	Table( const Table& table );
	~Table();

	/* calculates the positions and the velocities of the balls
	return values: GAME_LOST when the 8ball was shot earlier the others
	CUE_BALL_FOUL when the cue ball was shot
	OK when either some balls got into pockets or nothing special*/
	//int Update( float time, Score& score, int& player_number );
	int BallsStopped() const;  			// stop_flag == 1 when all balls' velocities are null
	void SetHit( sf::RenderWindow& window, Score& score, int player_number );	//assigns the hit to the cue ball
	void Draw( sf::RenderWindow& window );	// draws the table
}; 