#include "Score.hpp"
#include "../Ball/Ball.hpp"
#include <iostream>

Score::Score( const sf::VideoMode& video_mode, const std::vector<std::string>& player_names_,
	const std::string& font_file )
{
	// load the font from file
	font.loadFromFile( font_file );

	player_names = player_names_;

	// players initialization
	Player temp_player;
	temp_player.score = 0;
	temp_player.ball_type = 0;
	temp_player.text = sf::Text( player_names[0], font );
	temp_player.text.setCharacterSize( MAJOR_FONT_SIZE );
	temp_player.text.setPosition( sf::Vector2f( video_mode.width / 8, video_mode.height / 15 ) );
	players.push_back( temp_player );
	temp_player.text = sf::Text( player_names[1], font );
	temp_player.text.setColor( sf::Color::Black );
	temp_player.text.setCharacterSize( MINOR_FONT_SIZE );
	temp_player.text.setPosition( sf::Vector2f( video_mode.width * 2 / 3, video_mode.height / 15 ) );
	players.push_back( temp_player );

	// score position initialization
	left_score = sf::Vector2f( video_mode.width / 20, video_mode.height / 10 );
    right_score = sf::Vector2f( video_mode.width * 19 / 20, video_mode.height / 10 );
}

Score::Score(const Score& score)
{
	this->players = score.players;
	this->left_score = score.left_score;
	this->right_score = score.right_score;
	this->font = score.font;
}

Score::~Score() {}

void Score::AddBall( Ball& ball, int player_number )
{
	int where_to_put = ( ball.style < BALL7 ) ^ ( players[0].ball_type == 0 );
	sf::Vector2f scale = ball.sprite.getScale();
	ball.sprite.setScale( scale.x * SCORED_BALL_SCALE, scale.y * SCORED_BALL_SCALE );
	ball.radius *= SCORED_BALL_SCALE;

	if ( ball.style == BALL7 )
	{
		if ( player_number == 0 )
		{
			ball.position = left_score + sf::Vector2f( 0, ball.radius * 2.5f * players[player_number].score );
			players[0].score++;	
		}
		else
		{
			ball.position = right_score + sf::Vector2f( 0, ball.radius * 2.5f * players[player_number].score );
			players[1].score++;
		}
	}
	else if ( where_to_put == 0)
	{
		ball.position = left_score + sf::Vector2f( 0, ball.radius * 2.5f * players[0].score );
		players[0].score++;
	}
	else
	{
		ball.position = right_score + sf::Vector2f( 0, ball.radius * 2.5f * players[1].score );
		players[1].score++;
	}
}

void Score::Draw( sf::RenderWindow& window, int player_number )
{
	players[player_number].text = sf::Text( player_names[player_number], font );
	players[player_number].text.setColor( sf::Color::White );
	players[player_number].text.setCharacterSize( MAJOR_FONT_SIZE );
	players[1 - player_number].text = sf::Text( player_names[1 - player_number], font );
	players[1 - player_number].text.setColor( sf::Color::Black );
	players[1 - player_number].text.setCharacterSize( MINOR_FONT_SIZE );

	players[0].text.setPosition( sf::Vector2f( window.getSize().x / 8.0f, window.getSize().y / 15.0f ) );
	window.draw( players[0].text );
	players[1].text.setPosition( sf::Vector2f( window.getSize().x * 2.0f / 3.0f, window.getSize().y / 15.0f ) );
	window.draw( players[1].text );
}

std::vector<int> Score::GetScore() const
{
	std::vector<int> temp_vector( 2 );
	temp_vector[0] = players[0].score;
	temp_vector[1] = players[1].score;
	return temp_vector;
}