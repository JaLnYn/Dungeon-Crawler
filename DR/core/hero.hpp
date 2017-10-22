//
//  hero.hpp
//  Dungeons
//
//  Created by Alan Yuan on 2016-12-05.
//  Copyright © 2016 Alan Yuan. All rights reserved.
//

#ifndef hero_hpp
#define hero_hpp
#include "entity.hpp"
#include <stdio.h>


class hero: public entity{
public:
    hero(sf::String,int,int);
    
    // this function grabs the command of the player and applies it
    
    void init(eManager*);
    void groundGui(sf::RenderWindow*window);
    
    
    void useAbil(){
        if(abilTimer==abilCD){
            usingAbil = true;
        }
    }
    virtual void render(sf::RenderWindow*window);
    void lvlUP();
    void heal(int amount);
    
    void renderWepEquip(sf::RenderWindow * window,int i);
    
protected:
    
    
    bool tick(sf::RenderWindow *window);
    
    virtual bool searchLock();
    
    
protected:
    const int lockRange[7] = {3,5,7,7,7,5,3};
    const int lockArraySize = 7;
    int const renderSizeWep = 32;
    sf::RectangleShape equipBG;
    bool usingAbil = false;
    int cdSubtract = 20;
    int abilTimer = 600;
    int abilCD = 600;
    bool stopForThisTurn  = false;
    
    
    
    sf::RectangleShape energyBar;
};
#endif /* hero_hpp */
