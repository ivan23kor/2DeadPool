#include <cmath>
#include <iostream>
#include "table.hpp"
#include "ball.hpp"
#include "billiard.hpp"
#include "score.hpp"
#include "vector_operations.hpp"

Table::Table( const sf::Vector2f& position_, const sf::VideoMode& video_mode,
	const std::string& table_file, const std::string& ball_file, const std::string& billiard_file )
{
	// general initialization
	position = position_;
	width = video_mode.width * TABLE_WIDTH;
	height = video_mode.height * TABLE_HEIGHT;

	// determining the scale factor
	texture.loadFromFile( table_file );
	sf::Vector2f texture_size( texture.getSize() );
	sf::Vector2f scale( width / texture_size.x, height / texture_size.y );

	// pockets' radii setup
	corner_radius = 90 * height / 1775 * scale.y;
	middle_radius = 75 * height / 1775 * scale.y;

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
		borders[i] += position - sf::Vector2f( width / 2, height / 2 );
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
		pockets[i] += position - sf::Vector2f( width / 2, height / 2 );
	}

	// balls initialization
	float ball_radius = 44 * height * scale.y / 1775;
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
	ball_positions[15] = sf::Vector2f( width * (-.5), 0 );
	for (int i = 0; i < BALL_COUNT; ++i)
		balls.push_back( Ball( sf::Vector2f( ball_positions[i].x * scale.x / SCALE_X, ball_positions[i].y * scale.y / SCALE_Y )
			+ position + sf::Vector2f( width / 4 * scale.x / SCALE_X, 0 ), null_velocity, ball_radius, ball_file, i ) );
	
	// graphical initialization
	sprite.setTexture( texture );
	sprite.setScale( scale );
	sprite.setPosition( position - sf::Vector2f( width / 2, height / 2 ) );

	//billiard setup
	billiard.push_back( Billiard( balls.back().position, sf::Vector2f( 1, 0 ), billiard_file ) );
}

Table::~Table()	{}

int Table::update( float time, Score& score, int& player_number )
{
    sf::Vector2f rel_distance( 0, 0 );
    sf::Vector2f vel_difference( 0, 0 );
    sf::Vector2f delta_velocity( 0, 0 );
    bool zero_score = ( score.players[0].score == 0 ) && ( score.players[1].score == 0 );
    int function_return = OK;

	// balls' positions update
    for (int i = 0; i < balls.size(); ++i)
    {
        for (int j = i + 1; j < balls.size(); ++j)
        {
            rel_distance = balls[j].position - balls[i].position;
            vel_difference = balls[j].velocity - balls[i].velocity;
            if ( ( balls[i].radius + balls[j].radius
                - getLength( rel_distance ) > 0.0 ) && ( getScalar( vel_difference, rel_distance ) < 0.0 ) )
            {
                delta_velocity = getNorm( rel_distance ) * getScalar( vel_difference, getNorm( rel_distance ) );
                balls[i].velocity += delta_velocity * BALL_REFLECTION;
                balls[j].velocity -= delta_velocity * BALL_REFLECTION;
            }
        }

        if ( balls[i].update( time, *this ) == 0 )
        {
        	if ( zero_score )
        	{
        		zero_score = false;
        		if ( i < 7 )
        		{
        			score.players[1 - player_number].ball_type = 1;
        			score.add_ball( balls[i], player_number );
        		}
    			else if ( i == CUE_BALL )
    			{
        			balls[i].position = sf::Vector2f( -1, -1 ) * balls[i].radius;
        			function_return = CUE_BALL_FOUL;
    			}
    			else if ( i == 7 )
    			{
    				score.add_ball( balls[i], player_number );
    				function_return = GAME_LOST;
    			}
        		else
        		{
    				score.players[player_number].ball_type = 1;
    				score.add_ball( balls[i], player_number );
    			}
        	}
        	else if ( i == CUE_BALL )
        	{
        		balls[i].position = sf::Vector2f( -1, -1 ) * balls[i].radius;
        		if ( function_return != GAME_LOST )
        			function_return = CUE_BALL_FOUL;
        	}
        	else if ( ( i == BALL7 ) && ( score.players[player_number].score != BALL7 ) )
        		{
        			score.add_ball( balls[i], player_number );
        			function_return = GAME_LOST;
        		}
        	else
       			score.add_ball( balls[i], player_number );
        }
    }

    return function_return;
}

int Table::balls_stopped() const
{
	int stop_flag = 1;
	sf::Vector2f null_vector( 0, 0 );
	for (int i = 0; i < balls.size(); ++i)
		if ( balls[i].velocity != null_vector )
			stop_flag = 0;

	return stop_flag;
}

void Table::setHit( sf::RenderWindow& window, Score& score, int player_number )
{
    sf::Vector2f hit_velocity( 0, 0 );

    if ( balls[CUE_BALL].position == sf::Vector2f( -1, -1 ) * balls[CUE_BALL].radius )
    {
    	sf::Vector2f possible_position;
    	float left_border = this->borders[11].x + balls[CUE_BALL].radius;
    	float right_border = this->borders[4].x - balls[CUE_BALL].radius;
    	float upper_border = this->borders[0].y + balls[CUE_BALL].radius;
    	float lower_border = this->borders[9].y - balls[CUE_BALL].radius;
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

	    		// check for other balls on the same place
	    		for (int i = 0; i < balls.size() - 1; ++i)
	    			if ( getInterval( possible_position, balls[i].position ) < balls[CUE_BALL].radius * 2.0f )
	    				possible_position = sf::Vector2f( -1, -1 ) * balls[CUE_BALL].radius;
	    		balls[CUE_BALL].position = possible_position;

	    		// table display
		        window.clear( sf::Color( 0, 100, 0, 0 ) );
		        this->draw( window );
		        score.draw( window, player_number );
		        window.display();

    			if ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
	    			break;
	    		if ( !window.isOpen() )
	    			return;
	    	}
	    
	    	if ( balls[CUE_BALL].position == sf::Vector2f( -1 , -1 ) * balls[CUE_BALL].radius )
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
		if ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
			break;
		if ( !window.isOpen() )
			return;
		// table display
        window.clear( sf::Color( 0, 100, 0, 0 ) );
        this->draw( window );
        score.draw( window, player_number );
        window.display();
    }
    billiard[0].position = balls[CUE_BALL].position;
    hit_velocity = billiard[0].setHit( window, *this, score, player_number );
    balls[balls.size() - 1].velocity = hit_velocity;
}

void Table::draw( sf::RenderWindow& window )
{
    window.draw( sprite );
    for (int i = 0; i < balls.size(); ++i)
		balls[i].draw( window );
	billiard[0].draw( window, balls[CUE_BALL].radius );
}