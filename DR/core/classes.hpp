//
//  classes.hpp
//  Dungeons
//
//  Created by Alan Yuan on 2017-01-11.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#ifndef classes_hpp
#define classes_hpp

#include "hero.hpp"
#include "eManager.hpp"
#include <stdio.h>
class classes:public hero{
public:
    int armourP = 0;
    int base = 0;
    int critChance = 0;
    double multiplyer = 0;
    
    classes(sf::String s, int x,int y):hero(s,x,y){
        
    }
    
};

class samurai:public classes{

public:
    void hurt(int dmg){
        if(usingAbil>0){
            
        }else{
            hp-=dmg;
            if(hp<0){
                dead = true;
                eman->removeEnt(this);
            }
        }
        
    }
    samurai(int x,int y):classes("knight.png",x,y){
        base = 3;
        armourP = 0;
        critChance = 0;
        attackRange = 1;
        equipBG.setFillColor(sf::Color(170,160,110,255));
    }
 
    void damage(entity*e,entity*h){
        
    }
};

class rogue:public classes{
    
public:
    
    rogue(int x,int y):classes("Rogue.png",x,y){
        base = 3;
        atk = 900;
        armourP = 0;
        critChance = 0;
        attackRange = 1;
        cdSubtract = 1;
        abilCD = 900;
        equipBG.setFillColor(sf::Color(150,200,255,255));
    }
    
    int getGoodGuy(){
        if(usingAbil){
            return 0;
        }else{
            return 1;
        }
    }
    
    bool attack(){
        usingAbil = false;
        return entity::attack();
    }
    void render(sf::RenderWindow*window){
        
        if(!usingAbil){
            
        }else{
            sprite.setTextureRect(sf::IntRect((dir*defaultFrames+currentFrame)*(picSize),0,picSize,picSize));
        }
        hero::render(window);
    }
    void damage(entity*e,entity*h){
        
    }
};

class medic:public classes{
    
public:
    
    medic(int x,int y):classes("sprite thing.png",x,y){
        base = 3;
        armourP = 0;
        critChance = 0;
        attackRange = 1;
        cdSubtract = 1;
        abilCD = 900;
        equipBG.setFillColor(sf::Color(0,0,0,0));
    }
    
    bool ability(){
        if(usingAbil){
            eman->gainPotion();
        }
    }
    
};

class realClass:public classes{
    
public:
    
    realClass(int x,int y, sf::String path):classes(path,x,y){
        base = 3;
        armourP = 0;
        critChance = 0;
        attackRange = 1;
        cdSubtract = 3;
        abilTimer = 50;
        abilCD = 50;
        atk = 900;
        equipBG.setFillColor(sf::Color(150,150,150,255));
    }
    
    bool ability(){
        if(usingAbil){
            //implement ability after adding long term effects
        }
    }
};


#endif /* classes_hpp */
