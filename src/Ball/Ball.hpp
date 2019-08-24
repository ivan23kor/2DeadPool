#pragma once
#include <SFML/Graphics.hpp>

#define BALL_RADIUS 37
#define MIN_SPEED 1e-2f
#define FRICTION 23e-4f
#define BORDER_REFLECTION .6f
#define BALL_REFLECTION .75f

class Table;

class Ball
{
	// movement properties
	sf::Vector2f position;							// the position of the center of the ball
	sf::Vector2f velocity;							// the velocity of the ball

	// material properties
	float radius;									// the radius of the ball

	// graphical variables
	sf::Texture texture;							// the texture (picture) of the ball
	sf::Sprite sprite;
	int style;

	int Update( float time, const Table& table );	// moves the ball, returns 0 if got into a pocket
	void Draw( sf::RenderWindow& window ); 			// draws the ball

	friend class Table;
	friend class Cue;
	friend class Score;
	friend class Game;

public:
	Ball( const sf::Vector2f& position_, float radius_,
		const std::string& name, int style_ );
	~Ball();
};