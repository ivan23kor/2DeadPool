#include <iostream>
#include "Game.hpp"
#include "../Vector_aux/vector_operations.hpp"

Game::Game( const sf::VideoMode& video_mode, const std::string& table_file, const std::string& ball_file,
		const std::string& Cue_file, const std::vector<std::string>& player_names, const std::string& font_file )
{
    current_player = 1;
	table = Table( video_mode, table_file, ball_file, Cue_file );
	score = Score( video_mode, player_names, font_file );

    sf::SoundBuffer temp;
    std::string filename = "resources/Audio/Hit1.wav";
    temp.loadFromFile( filename );
    ball_sound_buffer.push_back( temp );
}

Game::~Game() {}

void Game::NextTurn( sf::RenderWindow& window )
{
	sf::Vector2f hit_velocity( 0, 0 );

    if ( table.balls.back().position == sf::Vector2f( -1, -1 ) * table.balls.back().radius )
		table.SetCueBall( window, score, current_player );

    table.cue[0].position = table.balls.back().position;
    table.cue[0].is_visible = true;
    
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
        this->Draw( window );
        //score.Draw( window, current_player );
        window.display();
    }

    hit_velocity = table.cue[0].SetHit( window, table, score, current_player );
    table.balls.back().velocity = hit_velocity;
}

int Game::Update( float time )
{
    // for balls' collisions
    sf::Vector2f rel_distance( 0, 0 );
    sf::Vector2f vel_difference( 0, 0 );
    sf::Vector2f delta_velocity( 0, 0 );

    int function_return = OK;

    // balls' positions update
    for (int i = 0; i < table.balls.size(); ++i)
    {
        for (int j = i + 1; j < table.balls.size(); ++j)
        {
            rel_distance = table.balls[j].position - table.balls[i].position;
            vel_difference = table.balls[j].velocity - table.balls[i].velocity;
            if ( ( table.balls[i].radius + table.balls[j].radius
                - getLength( rel_distance ) > 0.0 ) && ( getScalar( vel_difference, rel_distance ) < 0.0 ) )
            {
                delta_velocity = getNorm( rel_distance ) * getScalar( vel_difference, getNorm( rel_distance ) );
                table.balls[i].velocity += delta_velocity * BALL_REFLECTION;
                table.balls[j].velocity -= delta_velocity * BALL_REFLECTION;

                // hit sound setup
                sound.setBuffer( ball_sound_buffer[0] );
                sound.setVolume( getLength( delta_velocity ) * SOUND_VOLUME + MIN_VOLUME);
                sound.play();
            }
        }

        if ( table.balls[i].Update( time, table ) == 0 )
        {
            // check for the first non-cue ball getting into a pocket
            if ( ( score.players[0].score == 0 ) && ( score.players[1].score == 0 ) )
            {
                if ( i < 7 )
                {
                    score.players[1 - current_player].ball_type = 1;
                    score.AddBall( table.balls[i], current_player );
                }
                else if ( i == CUE_BALL )
                {
                    table.balls[i].position = sf::Vector2f( -1, -1 ) * table.balls[i].radius;
                    function_return = CUE_BALL_FOUL;
                }
                else if ( i == 7 )
                {
                    score.AddBall( table.balls[i], current_player );
                    function_return = GAME_LOST;
                }
                else
                {
                    score.players[current_player].ball_type = 1;
                    score.AddBall( table.balls[i], current_player );
                }
            }
            else if ( i == CUE_BALL )
            {
                table.balls[i].position = sf::Vector2f( -1, -1 ) * table.balls[i].radius;
                if ( function_return != GAME_LOST )
                    function_return = CUE_BALL_FOUL;
            }
            else if ( ( i == BALL7 ) && ( score.players[current_player].score != BALL7 ) )
                {
                    score.AddBall( table.balls[i], current_player );
                    function_return = GAME_LOST;
                }
            else
                score.AddBall( table.balls[i], current_player );
        }
    }

    return function_return;
}

void Game::Draw( sf::RenderWindow& window )
{
	table.Draw( window );
	score.Draw( window, current_player );
}