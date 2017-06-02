#include "Game.hpp"
#include <iostream>

Game::Game( const sf::VideoMode& video_mode, const std::string& table_file, const std::string& ball_file,
		const std::string& Cue_file, const std::vector<std::string>& player_names, const std::string& font_file )
{
	table = Table( video_mode, table_file, ball_file, Cue_file );
	score = Score( video_mode, player_names, font_file );
}

Game::~Game() {}

void Game::NextTurn( sf::RenderWindow& window, int player_number )
{
	sf::Vector2f hit_velocity( 0, 0 );

    if ( table.balls[CUE_BALL].position == sf::Vector2f( -1, -1 ) * table.balls[CUE_BALL].radius )
		table.SetCueBall( window, score, player_number );
    // hit setup
    while ( 1 )
    {
		sf::Event event; 
        while ( window.pollEvent( event ) )
        {
            // close the window if closure was triggered
            if ( event.type == sf::Event::Closed )
            {
                window.close();
	            return;
            }
        }
        if ( !window.isOpen() )
			return;
		if ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
			break;

		// table display
        window.clear( sf::Color( 0, 100, 0, 0 ) );
        this->Draw( window, player_number );
        //score.Draw( window, player_number );
        window.display();
    }

    table.cue[0].position = table.balls[CUE_BALL].position;
    hit_velocity = table.cue[0].SetHit( window, table, score, player_number );
    table.balls.back().velocity = hit_velocity;
}

void Game::Draw( sf::RenderWindow& window, int player_number )
{
	table.Draw( window );
	score.Draw( window, player_number );
}