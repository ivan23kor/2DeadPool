#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define CUE_SCALE .5f
#define PI 3.1415926f
#define MAX_POWER 5.5
#define MIN_POWER .2f
#define POWER_COEFF 3000
#define PULL_BACK 30.0f
#define ANIMATION_STEP 1e-5f

class Table;
class Score;

class Cue
{
	sf::Vector2f position; 		// specifies the position of the ball to hit
	sf::Vector2f direction;		// the direction of the hit
	float hit_power;			// the power of the hit

	sf::Texture texture;
	sf::Sprite sprite;
	bool is_visible;

	sf::SoundBuffer sound_buffer;

	sf::Vector2f SetHit( sf::RenderWindow& window, Table& table, Score& score, int player_number );
	void HitAnimation( sf::RenderWindow& window, Table& table, Score& score, int player_number );
	void Draw( sf::RenderWindow& window, float ball_radius );

	friend class Table;
	friend class Game;

public:
	Cue( const sf::Vector2f& position_, const std::string& filename );
	~Cue();
};