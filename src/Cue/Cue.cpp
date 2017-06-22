#include <cmath>
#include <iostream>
#include "Cue.hpp"
#include "../Table/Table.hpp"
#include "../Ball/Ball.hpp"
#include "../Score/Score.hpp"
#include "../Vector_aux/vector_operations.hpp"

Cue::Cue( const sf::Vector2f& position_, const std::string& filename )
{
	position = position_;
	direction = sf::Vector2f( 1, 0 );
	hit_power = 0.0;

	// graphical initialization
	texture.loadFromFile( filename );
	sf::Vector2f size( texture.getSize() );
	sprite.setScale( sf::Vector2f( CUE_SCALE, CUE_SCALE ) );
	sprite.setTexture( texture );
	sprite.setPosition( position );
	sprite.setRotation( 90 + atan2f( direction.x, direction.y ) * 180 / PI );

	// not visible until setHit called
	is_visible = true;

	// loading hit sound from buffer
	sound_buffer.loadFromFile( "resources/Audio/Cue.wav" );
}

Cue::~Cue() {}

sf::Vector2f Cue::SetHit( sf::RenderWindow& window, Table& table, Score& score, int player_number )
{
	sf::Vector2f return_value;

	sf::Vector2f mouse_position = sf::Vector2f( sf::Mouse::getPosition( window ) );
	float initial_power = getLength( mouse_position - position )
		* getLength( mouse_position - position ) / POWER_COEFF;
	float hint_length = 1000.0;
	sf::Vertex hint[] =
	{
	    sf::Vertex( position ),
	    sf::Vertex( position + direction * hint_length )
	};

	// power bar
	/*sf::RectangleShape powerBar( sf::Vector2f( window.getSize().x / 20, window.getSize().y / 3 ) );
	powerBar.setPosition( sf::Vector2f( window.getSize().x * .925, window.getSize().y * .5 ) );
	sf::RectangleShape powerBar_color;
	powerBar_color.setFillColor( sf::Color( 255, 0, 0 ) );*/

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
	         		return sf::Vector2f( 0, 0 );
	            }
	        }
			// retrieving the hit power
			mouse_position = sf::Vector2f( sf::Mouse::getPosition( window ) );
			hit_power = getLength( mouse_position - position ) * getLength(
				mouse_position - position ) / POWER_COEFF - initial_power;
			if ( hit_power > MAX_POWER )
	        	hit_power = MAX_POWER;
	        if ( hit_power < 0.0 )
	        	hit_power = 0.0;

	        hint[1] = sf::Vertex( position + direction * hint_length );

	        // displaying everything
	        window.clear( sf::Color( 0, 100, 0, 0 ) );
	        table.Draw( window );
	        score.Draw( window, player_number );
	        window.draw( hint, 2, sf::Lines);
	        window.display();

	        if ( !sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
    			break;
    		if ( !window.isOpen() )
    			return sf::Vector2f( 0, 0 );
		}

		if ( hit_power < MIN_POWER )
			while ( 1 )
			{
				// check all the window's events that have been triggered
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
		        if ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
	    			break;
	    		if ( !window.isOpen() )
	    			return sf::Vector2f( 0, 0 );

	    		// displaying everything
		        window.clear( sf::Color( 0, 100, 0, 0 ) );
		        table.Draw( window );
		        score.Draw( window, player_number );
		        window.display();
			}

		mouse_position = sf::Vector2f( sf::Mouse::getPosition( window ) );
		initial_power = getLength( mouse_position - position )
			* getLength( mouse_position - position ) / POWER_COEFF;

		if ( !sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
			break;
		if ( !window.isOpen() )
			return sf::Vector2f( 0, 0 );
	}

	is_visible = false;
	HitAnimation( window, table, score, player_number );
	return_value = getNorm( direction ) * hit_power;
	hit_power = 0.0;
	return return_value;
}

void Cue::HitAnimation( sf::RenderWindow& window, Table& table, Score& score, int player_number )
{
	int i = 0;
	float step = 0.0;
	//float step = hit_power * hit_power * ANIMATION_STEP;
	float current_interval = hit_power * PULL_BACK;
	sf::Vector2f initial_position = sprite.getPosition() + getNorm( direction ) * current_interval;

	while ( window.isOpen() && ( current_interval > 0.0 ) )
	{
		sf::Event event;
		while ( window.pollEvent( event ) )
		{
			if ( event.type == sf::Event::Closed )
				window.close();
		}

		// setting the position closer to the cue ball each step
		i++;
		step += ANIMATION_STEP * (float)i;
		current_interval -= step * PULL_BACK;
		sprite.setPosition( initial_position - getNorm( direction ) * current_interval );

		// displaying everything
        window.clear( sf::Color( 0, 100, 0, 0 ) );
        table.Draw( window );
        window.draw( sprite );
        score.Draw( window, player_number );
        window.display();
	}

	sf::Sound sound;
	sound.setBuffer( sound_buffer );
	//sound.play();
	sound.setVolume( hit_power / MAX_POWER * 100.0f );
	while( sound.getStatus() == sf::Sound::Playing ) {}
}

void Cue::Draw( sf::RenderWindow& window, float ball_radius )
{
	if ( is_visible )
	{
		sf::Vector2f mouse_position = sf::Vector2f( sf::Mouse::getPosition( window ) );
		direction = position - mouse_position;
		sprite.setRotation( 142 + atan2f( direction.y, direction.x ) * 180 / PI );
		sprite.setPosition( position - getNorm( direction )
	       		* ( hit_power * PULL_BACK + ball_radius ) );
		sprite.setTexture( texture );
		window.draw( sprite );
	}
}