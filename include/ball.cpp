#include "ball.hpp"
#include "vector_operations.hpp"
#include <iostream>
#include <cmath>

Ball::Ball()
{
	position = velocity = sf::Vector2f( 0.0, 0.0 );
	angle = angular_velocity = 0.0;
	style = 0;
}

Ball::Ball( const sf::Vector2f& position_, const sf::Vector2f& velocity_, float radius_,
	const std::string& name, int style_ )
{
	position = position_;
	velocity = velocity_;
	angle = angular_velocity = 0.0;
	radius = radius_;
	style = style_;

	sf::Image image;
	sf::Vector2i text_size(188, 188);
	int y = 0;
	switch ( style / 4 )
	{
	case 1:
		y = 265;
		break;
	case 2:
		y = 550;
		break;
	case 3:
		y = 835;
		break;
	default:
		break;
	}
	int x = 0;
	switch ( style % 4 )
	{
	case 1:
		x = 215;
		break;
	case 2:
		x = 425;
		break;
	case 3:
		x = 640;
		break;
	default:
		break;
	}
	// magic and you get image and rect - input params for loadFromImage
	sf::Vector2i text_pos(x, y);
	sf::IntRect rect(text_pos.x, text_pos.y, text_size.x, text_size.y);
	image.loadFromFile(name);
	image.createMaskFromColor(sf::Color::White);

	texture.loadFromImage(image, rect);
	sprite.setTexture( texture );
	sf::Vector2f scale( 2 * radius / texture.getSize().x, 2 * radius / texture.getSize().y );
	sprite.setScale( scale );
	sprite.setPosition( position - sf::Vector2f( radius, radius ) );
}

Ball::~Ball() {}

// returns 1 if the ball is still on the table, 0 if got into a pocket
int Ball::update( float time, const Table& table )
{
	// absolute value of the ball's velocity
	float speed = sqrt( velocity.x * velocity.x + velocity.y * velocity.y );
	// velocity normal to a pocket's corner
	sf::Vector2f normal_velocity;
	sf::Vector2f tangent_velocity;
	// a direction from the ball to the corner of a pocket
	sf::Vector2f normal;

	// rotation setup
	if ( angular_velocity > MAX_ANG_SPEED )
		angular_velocity = MAX_ANG_SPEED;
	if ( angular_velocity >= MIN_ANG_SPEED )
	{
		angle += angular_velocity * time;
		angular_velocity -= time * ANGULAR_FRICTION;
	}
	else
		angular_velocity = 0.0;

	// moving the ball as if there were no borders
	if ( speed > MIN_SPEED )
	{
		sf::Vector2f acceleration = -velocity / speed * FRICTION;
		position += velocity * time + acceleration * time * time / 2.0f;
		velocity += acceleration * time;
	}
	else
	{
		velocity = sf::Vector2f( 0, 0 );
		return 1;
	}

	// checks if the ball is inside of a pocket
	for (int i = 0; i < table.pockets.size(); ++i)
	{
		if ( ( i == 1 ) || ( i == 4 ) )
		{
			if ( getInterval( position, table.pockets[i] ) < table.middle_radius )
			{
				velocity.x = velocity.y = 0.0;
				return 0;
			}
		}
		if ( ( i != 1 ) && ( i != 4 ) )
		{
			if ( getInterval( position, table.pockets[i] ) < table.corner_radius )
			{
				velocity.x = velocity.y = 0.0;
				return 0;
			}	
		}
	}

	// reflection from the pockets' corners
	for (int i = 0; i < table.borders.size(); ++i)
	{
		if ( getInterval( position, table.borders[i] ) < radius )
		{
			normal = table.borders[i] - position;
			float normal_length = getLength( normal );
			normal /= normal_length;
			normal_velocity = normal * getScalar( velocity, normal );
			tangent_velocity = velocity - normal_velocity;
			tangent_velocity += getNorm( tangent_velocity ) * angular_velocity;
			velocity -= normal_velocity * ( 1.0f + BORDER_REFLECTION );
			position += velocity * time;
			return 1;
		}
	}

	// reflection from horizontal borders
	if ( ( ( position.x >= table.borders[0].x ) && ( position.x <= table.borders[1].x ) ) || 
		( ( position.x >= table.borders[2].x ) && ( position.x <= table.borders[3].x ) ) )
	{
		if ( position.y < table.borders[0].y + radius )
		{
			position.y = ( table.borders[0].y + radius ) * 2 - position.y;
			velocity.y = -velocity.y * BORDER_REFLECTION;
			velocity.x -= angular_velocity * BORDER_ANG_COEFF;
		}
		if ( position.y > table.borders[6].y - radius )
		{
			position.y = ( table.borders[6].y - radius ) * 2 - position.y;
			velocity.y = -velocity.y * BORDER_REFLECTION;
			velocity.x += angular_velocity * BORDER_ANG_COEFF;
		}
	}

	// reflection from vertical borders
	if ( ( position.y >= table.borders[4].y ) && ( position.y <= table.borders[5].y ) )
	{
		if ( position.x > table.borders[5].x - radius )
		{
			position.x = ( table.borders[5].x - radius ) * 2 - position.x;
			velocity.x = -velocity.x * BORDER_REFLECTION;
			velocity.y -= angular_velocity * BORDER_ANG_COEFF;
		}
		if ( position.x < table.borders[10].x + radius )
		{
			position.x = ( table.borders[10].x + radius ) * 2 - position.x;
			velocity.x = -velocity.x * BORDER_REFLECTION;
			velocity.y += angular_velocity * BORDER_ANG_COEFF;
		}
	}

	return 1;
}

void Ball::draw( sf::RenderWindow& window)
{
	sprite.setTexture( texture );
	sprite.setPosition( position );
	sf::Vector2u circleSize = texture.getSize();
	sprite.setOrigin( circleSize.x / 2, circleSize.y / 2 );
	sprite.setRotation( angle );
	window.draw( sprite );
}