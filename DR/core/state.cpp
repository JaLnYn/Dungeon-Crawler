//
//  state.cpp
//  Dungeons
//
//  Created by Alan Yuan on 2016-12-20.
//  Copyright © 2016 Alan Yuan. All rights reserved.
//

#include "state.hpp"
#include <iostream>

void state::init(sf::RenderWindow*render){
    std::cout<<"error"<<std::endl;
    exit(0);
}

void state::tick(sf::RenderWindow*render){
    std::cout<<"error"<<std::endl;
    exit(0);
}

void state::render(sf::RenderWindow*render){
    std::cout<<"error"<<std::endl;
    exit(0);
}

void game_state::init(sf::RenderWindow*render){
    
    world.init(render);
}

void game_state::tick(sf::RenderWindow*render){
    
    world.tick(render);
}

void game_state::render(sf::RenderWindow*render){
    
    world.render(render);
    
}
