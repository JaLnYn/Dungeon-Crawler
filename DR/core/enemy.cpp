//
//  enemy.cpp
//  DR
//
//  Created by Alan Yuan on 2017-01-20.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#include "enemy.hpp"
#include "mobs.hpp"


enemy::enemy(sf::String path,int x,int y, int bsx, int bsy, int hsx, int hsy, int hos ):entity(path,x,y,2,bsx,bsy,hsx,hsy,hos){
    ani_moveSpd = 1;
    spwnX = x;
    spwnY = y;
    distanceFromDesMax = 1;
    
}

bool enemy::tick(sf::RenderWindow *window){
    entity::tick(window);
    //invinsible minions hp = 100;
    
    if (AICycle > intelligent) {
        AICycle = 0;
        AI();
    }else{
        AICycle++;
    }
    
    if(attack()){
        
    }else if (!moveing) {
        if (path.size()>0) {
            cmd = 3;
            if(path[path.size()-1].x!=destination.x && path[path.size()-1].y != destination.y){
                fpath(destination);
            }else{
                distanceFromDesMax=0;
            }
            initMove();
        }else if (abs(x-destination.x) > distanceFromDesMax || abs(y-destination.y) > distanceFromDesMax){
            fpath(destination);
            initMove();
            
        }else{
            
            if(wandering<0){
                wander();
                cmd = 6;
            }else if(wandering > 0 && path.size()<1){
                wandering--;
            }else{
                fpath(sf::Vector2i(spwnX, spwnY));
                wandering--;
                
            }
        }
    }else{
        movePhase();
        return true;
    }
    return false;
}

void enemy::wander(){
    
    if(waitWonder<waitOnWonder){
        waitWonder++;
    }else{
        setDes(getX()+rand()%3-1, getY()+rand()%3-1);
    }
    
    
}
