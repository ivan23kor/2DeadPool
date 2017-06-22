#include "Game/Game.hpp"
#include "Table/Table.hpp"
#include "Ball/Ball.hpp"
#include "Cue/Cue.hpp"
#include "Score/Score.hpp"
#include "Vector_aux/vector_operations.hpp"
#include <cmath>
#include <iostream>
#include <thread>

#define TIME_CONSTANT 2.5e3f
#define EARLY_END 2
#define WIN_SCORE 8
#define SLEEP_TIME 20

int GameStart( const std::vector<std::string>& player_names );

int main(int argc, char const *argv[])
{
    // music playing on the background
    float background_volume = 50.0;
    sf::Music background_music;
    if ( !background_music.openFromFile( "resources/Audio/C418.wav" ) )
        return -1;
    background_music.setVolume( background_volume );
    background_music.play();

    // game start
    std::vector<std::string> player_names(2);
    std::cout << "Input the name of the first player." << std::endl;
    std::cin >> player_names[0];
    std::cout << "Input the name of the second player." << std::endl;
    std::cin >> player_names[1];
    int game_result = GameStart( player_names );

    // game result print
    if ( game_result == EARLY_END )
        std::cout << "See ya later" << std::endl;
    else
        std::cout << player_names[game_result] << " is the winner for today! Congrats!" << std::endl;

	return 0;
}

int GameStart( const std::vector<std::string>& player_names )
{
    // window initialization
    const sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();
    //video_mode.width -=
    sf::RenderWindow window( video_mode, "2DeadPool", sf::Style::Fullscreen );

    // table initialization
    const std::string table_file = "resources/Pictures/Table.png";
    const std::string ball_file = "resources/Pictures/Balls.png";
    const std::string cue_file = "resources/Pictures/Cue.png";
    const std::string font_file = "resources/Fonts/Lithograph-Bold.ttf";
    Game game( video_mode, table_file, ball_file, cue_file, player_names, font_file );

    // clock for the independence from CPU speed
    sf::Clock clock;
    sf::Time time = clock.getElapsedTime();
    float previous_time = time.asMicroseconds();
    float dt = 0.0;

    // update_result function return is stored here
    int update_result = 0;
    int cue_foul_flag = 0;
    int game_lost_flag = 0;

    // needed for changing turns
    std::vector<int> previous_score( 2 );
    previous_score[0] = 0;
    previous_score[1] = 0;
    std::vector<int> current_score( 2 );
    current_score[0] = 0;
    current_score[1] = 0;

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

        /*// table update
        time = clock.getElapsedTime();
        dt = time.asMicroseconds() - previous_time;
        previous_time = time.asMicroseconds();*/
        update_result = game.Update( 1.0f );

        // update_result processing
        if ( update_result == GAME_LOST)
            game_lost_flag = 1;
        if ( update_result == CUE_BALL_FOUL)
            cue_foul_flag = 1;

        // checks for turns' change
        if ( game.table.BallsStopped() )
        {
            // if ball8 was shot earlier than all the player's balls
            if ( game_lost_flag != 0 )
                return 1 - game.current_player;

            current_score = game.score.GetScore();
            if ( cue_foul_flag != 0 )
            {
                game.current_player = 1 - game.current_player;
                cue_foul_flag = 0;
            }
            else if ( previous_score[game.current_player] == current_score[game.current_player] )
                game.current_player = 1 - game.current_player;
            previous_score = current_score;

            // check for game end
            if ( game.score.GetScore()[0] == WIN_SCORE )
                return 0;
            if ( game.score.GetScore()[1] == WIN_SCORE )
                return 1;

            // set hit
            game.NextTurn( window );
        }

        // table display
        window.clear( sf::Color( 0, 100, 0, 0 ) );
        game.Draw( window );
        window.display();
    }

    return EARLY_END;
}