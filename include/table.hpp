#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#define BALL_COUNT 16
#define TABLE_WIDTH .8
#define TABLE_HEIGHT .8
#define SCALE_X	1.16047
#define SCALE_Y 1.22555
#define CUE_BALL 15
#define GAME_LOST -1
#define OK 0
#define CUE_BALL_FOUL 2

class Ball;
class Billiard;
class Score;

class Table
{
	// geometrical properties
	sf::Vector2f position;							// geometrical center of the table
	unsigned int width, height;						// dimensions of the table
	float corner_radius, middle_radius;				// pockets' properties

	std::vector <sf::Vector2f> pockets, borders;	// special origin points for balls, borders, pockets 
	std::vector<Ball> balls;						// the balls of the game
	std::vector<Billiard> billiard;					// the billiard

	// graphical properties
	sf::Texture texture;
	sf::Sprite sprite;

	friend class Ball;
	friend class Billiard;

public:
	// position_ is the center of the table, width and height are the dimensions in pixels, name picture filename
	Table( const sf::Vector2f& position_, const sf::VideoMode& video_mode,
		const std::string& table_file, const std::string& ball_file, const std::string& billiard_file );
	~Table();

	/* calculates the positions and the velocities of the balls
	return values: GAME_LOST when the 8ball was shot earlier the others
	CUE_BALL_FOUL when the cue ball was shot
	OK when either some balls got into pockets or nothing special*/
	int update( float time, Score& score, int& player_number );
	int balls_stopped() const;  			// stop_flag == 1 when all balls' velocities are null
	void setHit( sf::RenderWindow& window, Score& score, int player_number ); 	// ball to hit and the hit setup
	void draw( sf::RenderWindow& window );	// draws the table
}; 