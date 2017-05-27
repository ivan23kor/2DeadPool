#include "billiard.hpp"
#include "score.hpp"
#include "vector_operations.hpp"
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>

Billiard::Billiard()
{
	position = direction = sf::Vector2f( 0, 0 );
}

Billiard::Billiard( const sf::Vector2f& position_, const sf::Vector2f& direction_, const std::string& filename )
{
	position = position_;
	direction = direction_;

	// graphical initialization
	texture.loadFromFile( filename );
	sf::Vector2f size( texture.getSize() );
	sprite.setScale( sf::Vector2f( BILLIARD_SCALE, BILLIARD_SCALE ) );
	sprite.setTexture( texture );
	sprite.setPosition( position );
	sprite.setRotation( 90 + atan2f( direction.x, direction.y ) * 180 / PI );
}

Billiard::~Billiard() {}

sf::Vector2f Billiard::setHit( sf::RenderWindow& window, Table& table, Score& score, int player_number )
{
	// hit power
	float power = 0.0;
	position = table.balls[table.balls.size() - 1].position;
	sf::Vector2f mouse_position = sf::Vector2f( sf::Mouse::getPosition( window ) );
	float initial_power = getLength( mouse_position - position );			// zero hit power level

	// power bar
	sf::RectangleShape powerBar( sf::Vector2f( window.getSize().x / 20, window.getSize().y / 3 ) );
	powerBar.setPosition( sf::Vector2f( window.getSize().x * .925, window.getSize().y * .5 ) );
	sf::RectangleShape powerBar_color;
	powerBar_color.setFillColor( sf::Color( 255, 0, 0 ) );

	while ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) && ( window.isOpen() ) )
	{
		while ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) && ( window.isOpen() ) )
		{
			// check all the window's events that were triggered since the last iteration of the loop
	        sf::Event event; 
	        while ( window.pollEvent( event ) )
	        {
	            // close the window if closure was triggered
	            if ( event.type == sf::Event::Closed )
	            {
	            	window.close();
	         		return sf::Vector2f( 0, 0 );
	            }
	        }
			// retrieving the hit power
			mouse_position = sf::Vector2f( sf::Mouse::getPosition( window ) );
			power = ( getLength( mouse_position - position ) - initial_power ) / 5;
			if ( power > MAX_POWER )
	        	power = MAX_POWER;
	        if ( power < 0 )
	        	power = 0;

	       	// power bar setup
			powerBar_color.setSize( sf::Vector2f( powerBar.getSize().x,	powerBar.getSize().y * power / MAX_POWER ) );
			powerBar_color.setPosition( sf::Vector2f( window.getSize().x * .925, 
				window.getSize().y * 5 / 6 - powerBar.getSize().y * power / MAX_POWER ) );

	        // billiard texture displacement
	        mouse_position = sf::Vector2f( sf::Mouse::getPosition( window ) );
	        setRotation( mouse_position );
	        sprite.setPosition( position - getNorm( direction )
	       		* ( power * PULL_BACK + table.balls[table.balls.size() - 1].radius ) );
	        sprite.setTexture( texture );

	        // displaying everything
	        window.clear( sf::Color( 0, 100, 0, 0 ) );
	        table.draw( window );
	        window.draw( sprite );
	        score.draw( window, player_number );
	        window.draw( powerBar );
	        window.draw( powerBar_color );
	        window.display();
		}
		if ( power < MIN_POWER )
			while ( !sf::Mouse::isButtonPressed( sf::Mouse::Left ) && ( window.isOpen() ) )
			{
				sf::Event event; 
		        while ( window.pollEvent( event ) )
		        {
		            // close the window if closure was triggered
		            if ( event.type == sf::Event::Closed )
		            {
		            	window.close();
		         		return sf::Vector2f( 0, 0 );
		            }
		        }

				window.clear( sf::Color( 0, 100, 0, 0 ) );
	        	table.draw( window );
	        	score.draw( window, player_number );
	        	window.display();
			}

		mouse_position = sf::Vector2f( sf::Mouse::getPosition( window ) );
		initial_power = getLength( mouse_position - position );
	}

	hitAnimation( window, table, score, player_number, power );

	return getNorm( direction ) * power;
}

void Billiard::hitAnimation( sf::RenderWindow& window, Table& table, Score& score, int player_number, float power )
{
	float step = power * power * ANIMATION_STEP;
	float current_interval = power * PULL_BACK;
	sf::Vector2f initial_position = sprite.getPosition() + getNorm( direction ) * current_interval;

	while ( window.isOpen() && ( current_interval > 0 ) )
	{
		sf::Event event;
		while ( window.pollEvent( event ) )
		{
			if ( event.type == sf::Event::Closed )
				window.close();
		}

		// setting the position closer to the cue ball each step
		current_interval -= step * PULL_BACK;
		sprite.setPosition( initial_position - getNorm( direction ) * current_interval );

		// displaying everything
        window.clear( sf::Color( 0, 100, 0, 0 ) );
        table.draw( window );
        window.draw( sprite );
        score.draw( window, player_number );
        window.display();
	}
}

void Billiard::setRotation( const sf::Vector2f& mousePosition_ )
{
	direction = position - mousePosition_;
	sprite.setRotation( 142 + atan2f( direction.y, direction.x ) * 180 / PI );
}