//
//  game.hpp
//  spaceCruiser
//
//  Created by Alan Yuan on 2016-12-01.
//  Copyright © 2016 Alan Yuan. All rights reserved.
//

#ifndef game_hpp
#define game_hpp

#include "classes.hpp"
#include "Rooms.hpp"
#include <stdio.h>
#include "eManager.hpp"
#include "Player.hpp"

class game{
public:
    game();
    /**
     Purpose:
     Parameter:
     Return:
     **/
    void changeRoom(int,sf::RenderWindow*);
    // setup before the room
    void setup();
    //init all vars
    void init(sf::RenderWindow*);
    // generates the map. NOTE: this is not the room room
    void generate();
    // all the ai and input
    void tick(sf::RenderWindow*);
    void render(sf::RenderWindow*);
    sf::View*getCam(){
        return &cam;
    }
private:
    const int mapSize = 11;
    int lvl = 0;
    
    // checking the room numbers
    int currentRoom = 0;
    
    bool spaceIsPressed = false;
    int map[10];
    room rm;
    eManager eman;
    sf::View cam;
    bool gamePaused = false;
};

#endif /* game_hpp */
