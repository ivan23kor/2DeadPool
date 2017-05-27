#pragma once
#include <SFML/Graphics.hpp>
#include "ball.hpp"

#define MINOR_FONT_SIZE 25
#define MAJOR_FONT_SIZE 45
#define BALL7 7

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

	// scores' positions
	sf::Vector2f left_score, right_score;

	// sets the font for the name displaying
	sf::Font font;

	// puts the balls got into a pocket to the appropriate score
	void add_ball( Ball& ball, int player_number );

	friend class Table;

public:
	Score( const sf::VideoMode& video_mode,	const std::string& player_name1,
		const std::string& player_name2, const std::string& font_file );
	~Score();

	void draw( sf::RenderWindow& window, const int player_number );
	std::vector<int> getScore() const;
};