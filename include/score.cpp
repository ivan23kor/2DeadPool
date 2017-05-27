#include "score.hpp"

Score::Score( const sf::VideoMode& video_mode,	const std::string& player_name1,
		const std::string& player_name2, const std::string& font_file )
{
	// load the font from file
	font.loadFromFile( font_file );

	// players initialization
	Player temp_player;
	temp_player.score = 0;
	temp_player.ball_type = 0;
	temp_player.text = sf::Text( player_name1, font );
	temp_player.text.setCharacterSize( MAJOR_FONT_SIZE );
	temp_player.text.setPosition( sf::Vector2f( video_mode.width / 8, video_mode.height / 15 ) );
	players.push_back( temp_player );
	temp_player.text = sf::Text( player_name2, font );
	temp_player.text.setColor( sf::Color::Black );
	temp_player.text.setCharacterSize( MINOR_FONT_SIZE );
	temp_player.text.setPosition( sf::Vector2f( video_mode.width * 2 / 3, video_mode.height / 15 ) );
	players.push_back( temp_player );

	// score position initialization
	left_score = sf::Vector2f( video_mode.width / 20, video_mode.height / 10 );
    right_score = sf::Vector2f( video_mode.width * 19 / 20, video_mode.height / 10 );
}

Score::~Score() {}

void Score::add_ball( Ball& ball, int player_number )
{
	int where_to_put = ( ball.style < BALL7 ) ^ ( players[0].ball_type == 0 );
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

void Score::draw( sf::RenderWindow& window, const int player_number )
{
	players[player_number].text.setColor( sf::Color::White );
	players[player_number].text.setCharacterSize( MAJOR_FONT_SIZE );
	players[1 - player_number].text.setColor( sf::Color::Black );
	players[1 - player_number].text.setCharacterSize( MINOR_FONT_SIZE );
	window.draw( players[0].text );
	window.draw( players[1].text );
}

std::vector<int> Score::getScore() const
{
	std::vector<int> temp_vector( 2 );
	temp_vector[0] = players[0].score;
	temp_vector[1] = players[1].score;
	return temp_vector;
}