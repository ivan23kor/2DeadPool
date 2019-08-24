#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Table.hpp"
#include "../Score/Score.hpp"
#include "../Vector_aux/vector_operations.hpp"

#define UPPER_LEFT_POCKET( x ) ( 200 - x )

Table::Table( const sf::VideoMode& video_mode,	const std::string& table_file,
	const std::string& ball_file, const std::string& cue_file )
{
	// general initialization
	position = sf::Vector2f( video_mode.width / 2, video_mode.height * 11 / 20 );
	dimensions.x = video_mode.width * .8f;
	dimensions.y = video_mode.height * .8f;

	// determining the scale factor
	texture.loadFromFile( table_file );
	sf::Vector2f texture_size( texture.getSize() );
	sf::Vector2f scale( dimensions.x / texture_size.x, dimensions.y / texture_size.y );

	// borders setup
	borders.push_back(sf::Vector2f(79, 50));
	borders.push_back(sf::Vector2f(442, 50));
	borders.push_back(sf::Vector2f(500, 50));
	borders.push_back(sf::Vector2f(862, 50));
	borders.push_back(sf::Vector2f(896, 79));
	borders.push_back(sf::Vector2f(896, 428));
	borders.push_back(sf::Vector2f(862, 455));
	borders.push_back(sf::Vector2f(500, 455));
	borders.push_back(sf::Vector2f(442, 455));
	borders.push_back(sf::Vector2f(79, 455));
	borders.push_back(sf::Vector2f(45, 428));
	borders.push_back(sf::Vector2f(45, 79));
	//scaling to the window
	for (unsigned int i = 0; i < borders.size(); i++)
	{
		borders[i].x *= scale.x;
		borders[i].y *= scale.y;
		borders[i] += position - sf::Vector2f( dimensions.x / 2, dimensions.y / 2 );
	}

	// pockets setup
	pockets.push_back(sf::Vector2f(36, 41));		//left top
	pockets.push_back(sf::Vector2f(471, 30));		//middle top
	pockets.push_back(sf::Vector2f(906, 41));		//right top
	pockets.push_back(sf::Vector2f(906, 463));		//right bottom
	pockets.push_back(sf::Vector2f(471, 480));		//middle bottom
	pockets.push_back(sf::Vector2f(36, 463));		//left bottom
	//scaling to the window
	for (int i = 0; i < pockets.size(); i++)
	{
		pockets[i].x *= scale.x;
		pockets[i].y *= scale.y;
		pockets[i] += position - sf::Vector2f( dimensions.x / 2, dimensions.y / 2 );
	}
	corner_radius = 95 * dimensions.y / 1775 * scale.y;
	middle_radius = 80 * dimensions.y / 1775 * scale.y;

	// balls initialization
	float ball_radius = BALL_RADIUS * dimensions.y * scale.y / 1775;
	sf::Vector2f null_velocity( 0, 0 );
	std::vector<sf::Vector2f> ball_positions( BALL_COUNT ); // relatively to the table's center
	ball_positions[0] = sf::Vector2f( ball_radius * 2 * sqrt(3), -ball_radius * 2 );
	ball_positions[1] = sf::Vector2f( ball_radius * 4 * sqrt(3), ball_radius * 2 );
	ball_positions[2] = sf::Vector2f( ball_radius * 3 * sqrt(3), -ball_radius );
	ball_positions[3] = sf::Vector2f( ball_radius * 4 * sqrt(3), -ball_radius * 2 );
	ball_positions[4] = sf::Vector2f( ball_radius * 4 * sqrt(3), -ball_radius * 4 );
	ball_positions[5] = sf::Vector2f( ball_radius * 3 * sqrt(3), ball_radius * 3 );
	ball_positions[6] = sf::Vector2f( ball_radius * 1 * sqrt(3), ball_radius );
	ball_positions[7] = sf::Vector2f( ball_radius * 2 * sqrt(3), 0 );
	ball_positions[8] = sf::Vector2f( 0, 0 );
	ball_positions[9] = sf::Vector2f( ball_radius * 3 * sqrt(3), ball_radius );
	ball_positions[10] = sf::Vector2f( ball_radius * 4 * sqrt(3), ball_radius * 4 );
	ball_positions[11] = sf::Vector2f( ball_radius * 1 * sqrt(3), -ball_radius );
	ball_positions[12] = sf::Vector2f( ball_radius * 4 * sqrt(3), 0 );
	ball_positions[13] = sf::Vector2f( ball_radius * 3 * sqrt(3), -ball_radius * 3 );
	ball_positions[14] = sf::Vector2f( ball_radius * 2 * sqrt(3), ball_radius * 2 );
	ball_positions[15] = sf::Vector2f( dimensions.x * (-.5), 0 );
	for (int i = 0; i < BALL_COUNT; ++i)
		balls.push_back( Ball( sf::Vector2f( ball_positions[i].x * scale.x / SCALE_X, ball_positions[i].y * scale.y
			/ SCALE_Y )	+ position + sf::Vector2f( dimensions.x / 4 * scale.x / SCALE_X, 0 ), ball_radius, ball_file, i ) );
	
	// graphical initialization
	sprite.setTexture( texture );
	sprite.setScale( scale );
	sprite.setPosition( position - sf::Vector2f( dimensions.x / 2, dimensions.y / 2 ) );

	//Cue setup
	cue.push_back( Cue( balls.back().position, cue_file ) );
}

Table::Table( const Table& table )
{
	this->position = table.position;
	this->dimensions = table.dimensions;
	this->corner_radius = table.corner_radius;
	this->middle_radius = table.middle_radius;
	this->pockets = table.pockets;
	this->borders = table.borders;
	this->balls = table.balls;
	this->cue = table.cue;
	this->texture = table.texture;
	this->sprite = table.sprite;
}

Table::~Table()	{}

int Table::BallsStopped() const
{
	int stop_flag = 1;
	sf::Vector2f null_vector( 0, 0 );
	for (int i = 0; i < balls.size(); ++i)
		if ( balls[i].velocity != null_vector )
			stop_flag = 0;

	return stop_flag;
}

void Table::SetCueBall( sf::RenderWindow& window, Score& score, int player_number )
{
	sf::Vector2f possible_position;
	float left_border = this->borders[11].x + balls.back().radius;
	float right_border = this->borders[4].x - balls.back().radius;
	float upper_border = this->borders[0].y + balls.back().radius;
	float lower_border = this->borders[9].y - balls.back().radius;

	while ( 1 )
	{
		while ( 1 )
    	{
	        // check all the window's events that were triggered since the last iteration of the loop
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

    		possible_position = sf::Vector2f( sf::Mouse::getPosition( window ) );

    		// check for out of table displacement
    		if ( possible_position.x < left_border )
    			possible_position.x = left_border;
    		if ( possible_position.x > right_border )
    			possible_position.x = right_border;
    		if ( possible_position.y < upper_border )
    			possible_position.y = upper_border;
    		if ( possible_position.y > lower_border )
    			possible_position.y = lower_border;

    		// check for in-pocket placement
    		for (int i = 0; i < pockets.size(); ++i)
    			if ( ( i != 1 ) && ( i != 4 ) && ( getInterval( possible_position,
    				pockets[i] ) < corner_radius * IN_POCKET ) )
   				possible_position = pockets[i] + ( possible_position - pockets[i] ) * IN_POCKET;

    		// check for other balls on the same place
    		for (int i = 0; i < balls.size() - 1; ++i)
    			if ( getInterval( possible_position, balls[i].position ) < balls.back().radius * 2.0f )
    				possible_position = sf::Vector2f( -1, -1 ) * balls.back().radius;
    		balls.back().position = possible_position;

    		// table display
	        window.clear( sf::Color( 0, 100, 0, 0 ) );
	        this->Draw( window );
	        score.Draw( window, player_number );
	        window.display();

			if ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
    			break;
    	}
    
    	if ( balls.back().position == sf::Vector2f( -1 , -1 ) * balls.back().radius )
    	{
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
	    		if ( !sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
	    			break;
	    		if ( !window.isOpen() )
	    			return;
	    	}
	    }

	    if ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
    			break;
    		if ( !window.isOpen() )
    			return;
	}
}

void Table::SetHit( sf::RenderWindow& window, Score& score, int player_number )
{
    sf::Vector2f hit_velocity( 0, 0 );

    if ( balls.back().position == sf::Vector2f( -1, -1 ) * balls.back().radius )
		this->SetCueBall( window, score, player_number );

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
        score.Draw( window, player_number );
        window.display();
    }
    cue[0].position = balls.back().position;
    hit_velocity = cue[0].SetHit( window, *this, score, player_number );
    balls.back().velocity = hit_velocity;
}

void Table::Draw( sf::RenderWindow& window )
{
	sprite.setTexture( texture );
    window.draw( sprite );
    for (int i = 0; i < balls.size(); ++i)
		balls[i].Draw( window );
	cue[0].Draw( window, balls.back().radius );
}