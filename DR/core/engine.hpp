//
//  engine.hpp
//  Dungeons
//
//  Created by Alan Yuan on 2016-12-20.
//  Copyright © 2016 Alan Yuan. All rights reserved.
//

#ifndef engine_hpp
#define engine_hpp

#include "state.hpp"
#include <SFML/Graphics.hpp>
#include <stdio.h>
class engine{
public:
    state*mainState;
    void setState(state*state){
        this->mainState = state;
    }
    void init(sf::RenderWindow*window){
        mainState->init(window);
    }
    void tick(sf::RenderWindow*window){
        mainState->tick(window);
    }
    void render(sf::RenderWindow*window){
        mainState->render(window);
        
    }
};
#endif /* engine_hpp */
