#pragma once
#include <SFML/Graphics.hpp>
#include "../Table/Table.hpp"
#include "../Score/Score.hpp"

#define CUE_BALL 15
#define GAME_LOST -1
#define OK 0
#define CUE_BALL_FOUL 2
#define SOUND_VOLUME 35
#define MIN_VOLUME 10

class Game
{
public:
	int current_player;

	Table table;
	Score score;

	std::vector<sf::SoundBuffer> ball_sound_buffer;
	sf::Sound sound;

	sf::RenderWindow window;

	Game( const sf::VideoMode& video_mode, const std::string& table_file, const std::string& ball_file,
		const std::string& Cue_file, const std::vector<std::string>& player_names, const std::string& font_file );
	~Game();

	void NextTurn( sf::RenderWindow& window );
	int Update( float time );
	void Draw( sf::RenderWindow& window );
};