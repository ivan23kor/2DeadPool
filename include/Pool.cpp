#include "Game.hpp"
#include "Table.hpp"
#include "Ball.hpp"
#include "Billiard.hpp"
#include "Score.hpp"
#include "vector_operations.hpp"
#include <cmath>
#include <iostream>
#include <thread>

#define TIME_CONSTANT 2.5e3f
#define EARLY_END 2
#define WIN_SCORE 8
#define SLEEP_TIME 20

void Draw( sf::RenderWindow* window, Table& table, Score& score, int& player_number );
int GameStart( const std::vector<std::string>& player_names );

int main(int argc, char const *argv[])
{
    std::vector<std::string> player_names(2);
    std::cout << "Input the name of the first player." << std::endl;
    std::cin >> player_names[0];
    std::cout << "Input the name of the second player." << std::endl;
    std::cin >> player_names[1];

    // game start
    int game_result = GameStart( player_names );
    if ( game_result == EARLY_END )
        std::cout << "See ya later" << std::endl;
    else
        std::cout << player_names[game_result] << " is the winner for today! Congrats!" << std::endl;

	return 0;
}

/*void draw( sf::RenderWindow* window, Table& table, Score& score, int& player_number )
{
    // run the program as long as the window is open
    while ( 1 )
    {
        mutex.lock();
        if ( window->isOpen() )
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event; 
            while ( window->pollEvent( event ) )
            {
                // close the window if closure was triggered
                if ( event.type == sf::Event::Closed )
                    window->close();
            }

            // table display
            window->clear( sf::Color( 0, 100, 0, 0 ) );
            table.draw( *window );
            score.draw( *window, player_number );
            window->display();
            mutex.unlock();
            sf::sleep( sf::milliseconds( SLEEP_TIME ) );
        }
        else
        {
            mutex.unlock();
            sf::sleep( sf::milliseconds( SLEEP_TIME ) );
            return;
        }
    }
}*/

int GameStart( const std::vector<std::string>& player_names )
{
    // window initialization
    const sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window( video_mode, "2DeadPool", sf::Style::Fullscreen );

    // table initialization
    const std::string table_file = "../bin/Table.png";
    const std::string ball_file = "../bin/Balls.png";
    const std::string billiard_file = "../bin/Billiard.png";
    const std::string font_file = "../bin/Lithograph-Bold.ttf";
    Game game( video_mode, table_file, ball_file, billiard_file, player_names, font_file );

    // clock for the independence from CPU speed
    sf::Clock clock;
    sf::Time time = clock.getElapsedTime();
    float previous_time = time.asMicroseconds();
    float dt = 0.0;

    // flags for turns change
    int turn_flag = 0;
    int cue_foul_flag = 0;
    int game_lost_flag = 0;

    // contains the player's number whose turn it is
    int player_number = 0;

    // needed for changing turns
    std::vector<int> previous_score( 2 );
    previous_score[0] = 0;
    previous_score[1] = 0;
    std::vector<int> current_score( 2 );
    current_score[0] = 0;
    current_score[1] = 0;

    // == 0 when player1 wins, ==1 otherwise
    int update_result = 0;

    // starting the thread that draws everything
    //sf::Thread ThreadDraw( std::bind( &Draw, &window, table, score, player_number ) );
    //ThreadDraw.launch();

    // run the program as long as the window is open
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
                break;
            }
        }
        if ( !window.isOpen() )
            break;

        if ( game.table.BallsStopped() )
        {
        	// if ball8 was shot earlier than all the player's balls
        	if ( game_lost_flag != 0 )
        		return 1 - player_number;

        	// check for game end
	        if ( game.score.GetScore()[0] == WIN_SCORE )
	            return 0;
	        if ( game.score.GetScore()[1] == WIN_SCORE )
	            return 1;

	        // changing turns check
	        if ( turn_flag != 0 || cue_foul_flag != 0 )
	        {
	            current_score = game.score.GetScore();
	            if ( cue_foul_flag != 0 )
	                player_number = 1 - player_number;
	            else if ( previous_score[player_number] == current_score[player_number] )
	                    player_number = 1 - player_number; 
	            previous_score = current_score;
	            cue_foul_flag = 0;
	            turn_flag = 0;
	        }
        }

        // set hit
        if ( game.table.BallsStopped() == 1 )
        {
            game.NextTurn( window, player_number );
            turn_flag = 1;
        }
        // table update
        time = clock.getElapsedTime();
        dt = time.asMicroseconds() - previous_time;
        previous_time = time.asMicroseconds();
        update_result = game.table.Update( 1.0f, game.score, player_number );

        // update return processing
        switch ( update_result )
        {
            case GAME_LOST:
                game_lost_flag = 1;
                break;
            case CUE_BALL_FOUL:
                cue_foul_flag = 1;
                break;
        }

        // table display
        window.clear( sf::Color( 0, 100, 0, 0 ) );
        game.Draw( window, player_number );
        window.display();
    }

    return EARLY_END;
}