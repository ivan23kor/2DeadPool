#include "vector_operations.hpp"

float getInterval( const sf::Vector2f& point1, const sf::Vector2f& point2 )
{
	return sqrt( ( point2.x - point1.x ) * ( point2.x - point1.x ) + ( point2.y - point1.y ) * ( point2.y - point1.y ) );
}

float getScalar( const sf::Vector2f& vector1, const sf::Vector2f& vector2 )
{
	return ( vector1.x * vector2.x + vector1.y * vector2.y );
}

float getLength( const sf::Vector2f& vector )
{
	return sqrt( vector.x * vector.x + vector.y * vector.y );
}

sf::Vector2f getNorm( const sf::Vector2f& vector )
{
	return vector / getLength( vector );
}