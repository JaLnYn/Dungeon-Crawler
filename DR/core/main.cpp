
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include "state.hpp"
#include <iostream>
#include "engine.hpp"
using namespace std;

int main(int argc, char **argv){
    const int tileSize = 32;
    // set up window
    sf::Vector2i window_di; // window dimensions
    window_di.x = sf::VideoMode::getDesktopMode().width;
    window_di.y = sf::VideoMode::getDesktopMode().height;
    sf::RenderWindow window(sf::VideoMode(window_di.x, window_di.y), "SFML window",sf::Style::Fullscreen);
    window.setFramerateLimit(55);
    window.setVerticalSyncEnabled(true);
    
    
    //set up states
    game_state gState;
    credit_state cState;
    menu_state mState;
    option_state oState;
    engine engine;
    engine.setState(&gState);
    
    engine.init(&window);
    
    sf::Event event;
    
    while (window.isOpen()){
        
        window.clear();
        
        
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::EventType::Closed){
                window.close();
            }
        }
        
        //VV tick and render
        engine.tick(&window);
        engine.render(&window);
        
        
        window.display();
    }
    
    return EXIT_SUCCESS;
}
