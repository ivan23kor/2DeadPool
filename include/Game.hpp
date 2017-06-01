#pragma once
#include <SFML/Graphics.hpp>
#include "Table.hpp"
#include "Score.hpp"

class Game
{
public:
	Table table;
	Score score;
	//std::vector<Table> table;
	//std::vector<Score> score;

	Game( const sf::VideoMode& video_mode, const std::string& table_file, const std::string& ball_file,
		const std::string& billiard_file, const std::vector<std::string>& player_names, const std::string& font_file );
	~Game();

	void NextTurn( sf::RenderWindow& window, int player_number );
	void Draw( sf::RenderWindow& window, int player_number );
};