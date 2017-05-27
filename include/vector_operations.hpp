#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

float getInterval( const sf::Vector2f& point1, const sf::Vector2f& point2 );
float getScalar( const sf::Vector2f& vector1, const sf::Vector2f& vector2 );
float getLength( const sf::Vector2f& vector );
sf::Vector2f getNorm( const sf::Vector2f& vector );