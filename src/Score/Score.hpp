#pragma once
#include <SFML/Graphics.hpp>

#define MINOR_FONT_SIZE 25
#define MAJOR_FONT_SIZE 45
#define BALL7 7
#define SCORED_BALL_SCALE 1.4f

class Ball;

struct Player
{
	int score;
	int ball_type;	// ==0 if this player's balls are solid, ==1 if striped
	sf::Text text; 	// used for displaying this player's name
};

class Score
{
	// the scores of the players
	std::vector<Player> players;

	// the names of the players
	std::vector<std::string> player_names;

	// scores' positions
	sf::Vector2f left_score, right_score;

	// sets the font for the name displaying
	sf::Font font;

	// puts the balls got into a pocket to the appropriate score
	void AddBall( Ball& ball, int player_number );

	friend class Table;
	friend class Game;

public:
	Score() {};
	Score(const Score& score);
	Score( const sf::VideoMode& video_mode,	const std::vector<std::string>& player_names_,
		const std::string& font_file );
	~Score();

	void Draw( sf::RenderWindow& window, int player_number );
	std::vector<int> GetScore() const;
};