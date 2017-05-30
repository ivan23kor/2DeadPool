#pragma once
#include <SFML/Graphics.hpp>
#include "ball.hpp"

#define BILLIARD_SCALE .5f
#define PI 3.1415926f
#define MAX_POWER 7
#define MIN_POWER .1f
#define PULL_BACK 20.0f
#define ANIMATION_STEP 1e-5f

class Billiard
{
	sf::Vector2f position; 		// specifies the position of the ball to hit
	sf::Vector2f direction;		// the direction of the hit
	float hit_power;			// the power of the hit

	sf::Texture texture;
	sf::Sprite sprite;
	bool is_visible;

	void hitAnimation( sf::RenderWindow& window, Table& table, Score& score, int player_number );
	void draw( sf::RenderWindow& window, float ball_radius );

	friend class Table;

public:
	Billiard();
	Billiard( const sf::Vector2f& position_, const sf::Vector2f& direction_, const std::string& filename );
	~Billiard();

	sf::Vector2f setHit( sf::RenderWindow& window, Table& table, Score& score, int player_number );
};