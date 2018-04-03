//
//  game.cpp
//  spaceCruiser
//
//  Created by Alan Yuan on 2016-12-01.
//  Copyright © 2016 Alan Yuan. All rights reserved.
//

#include "game.hpp"





game::game():rm(&cam){
    float x = sf::VideoMode::getDesktopMode().width;
    float y = sf::VideoMode::getDesktopMode().height;
    int CmraY = 900;
    cam.setSize(CmraY*((double)x)/y, CmraY);
}

void game::generate(){
    
    //genorates the map (world)
    for (int y = 0; y < mapSize; y++) {
        map[y] = rand();
    }
    
}

void game::changeRoom(int x, sf::RenderWindow*window){
    currentRoom = x;
    rm.genorate(map[x]);
    eman.addppl();
    //rm.genorate(time(NULL));
}

void game::init(sf::RenderWindow*window){
    generate();
    eman.init(&rm,window);
    changeRoom(0,window);
    
}

void game::tick(sf::RenderWindow*window){
    if(gamePaused == false){
        eman.tick(window);
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        if(spaceIsPressed==false){
            spaceIsPressed = true;
            if(gamePaused){
                gamePaused = false;
            }else{
                gamePaused = true;
            }
 
        }
    }else{
        spaceIsPressed = false;
    }
    
}

void game::render(sf::RenderWindow*window){
    
    rm.render(window);
    eman.render(window,&cam);
    
    eman.getPlr()->renderGui(window);
    
}
