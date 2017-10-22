//
//  state.hpp
//  Dungeons
//
//  Created by Alan Yuan on 2016-12-20.
//  Copyright © 2016 Alan Yuan. All rights reserved.
//

#ifndef state_hpp
#define state_hpp

#include <SFML/Graphics.hpp>
#include "game.hpp"
#include <stdio.h>
class state{
public:
    virtual void init(sf::RenderWindow*);
    virtual void tick(sf::RenderWindow*);
    virtual void render(sf::RenderWindow*);
    
};
class game_state:public state{
public:
    game world;
    void init(sf::RenderWindow*);
    void tick(sf::RenderWindow*);
    void render(sf::RenderWindow*);
};
class menu_state:public state{
public:
    void init(sf::RenderWindow*){
        
    }

    void tick(sf::RenderWindow*){
        
    }

    void render(sf::RenderWindow*){
        
    }

};
class option_state:public state{
public:
    void init(sf::RenderWindow*){
        
    }

    void tick(sf::RenderWindow*){
        
    }

    void render(sf::RenderWindow*){
        
    }

    
};
class credit_state:public state{
public:
    void init(sf::RenderWindow*){
        
    }

    void tick(sf::RenderWindow*){
        
    }

    void render(sf::RenderWindow*){
        
    }

    
};
#endif /* state_hpp */
