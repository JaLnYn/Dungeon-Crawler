//
//  enemy.hpp
//  DR
//
//  Created by Alan Yuan on 2017-01-20.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#ifndef enemy_hpp
#define enemy_hpp

#include "entity.hpp"
#include <stdio.h>
#include "Player.hpp"

class enemy: public entity{
    
public:
    
    enemy(sf::String,int x,int y,int bsx,int bsy, int hsx, int hsy, int hos);
    virtual void wander();
    virtual void AI(){
       
        double distance[eman->getPlr()->getSize()];
        for (int i = 0; i < eman->getPlr()->getSize(); i++) {
            entity * e = eman->getPlr()->getHero(i);
            if (e!=nullptr) {
                
                if (e->getGoodGuy()!=this->getGoodGuy()&&e->getGoodGuy()!=0) {
                    
                    int nx = (e->getX()+e->getXMove()-this->getX()-this->getXMove());
                    int ny = (e->getY()+e->getYMove()-this->getY()-this->getYMove());
                    double currentDis = sqrt(nx*nx+ny*ny);
                    if(currentDis < viewDistance){
                        
                        Line line;
                        line = createLine(e->getX(), e->getY());
                        bool lineIsPassable = true;

                        for (int f = 0; f < line.getSize() && lineIsPassable; f++) {
                            
                            if(!eman->moveIn(line.getCoord(f).x, line.getCoord(f).y)){
                                lineIsPassable = false;
                                
                            }
                        }
                        if(lineIsPassable){
                            distance[i] = currentDis;
                        }else{
                            distance[i] = -1;
                        }
                    }else{
                        distance[i] = -1;
                    }
                }else{
                    distance[i] = -1;
                }
            }
        }
        int shortestDistanceID = -1;
        
        for (int i = 0; i < eman->getPlr()->getSize() && shortestDistanceID == -1; i++) {
            if (distance[i] > -1) {
                
                // there is something found
                wandering = wanderingWaitTime;
                
                shortestDistanceID = i;
                for (int f = i; f < eman->getPlr()->getSize() ; f++) {
                    if (distance[f]<distance[shortestDistanceID]&&distance[f]>-1) {
                        shortestDistanceID = f;
                    }
                }
                lockedOn = eman->getPlr()->getHero(shortestDistanceID);
                destination = sf::Vector2i(lockedOn->getX()+lockedOn->getXMove(),lockedOn->getYMove()+lockedOn->getY());
                
                
            }
        }

    }
    bool tick(sf::RenderWindow *window);
    
protected:
    int waitOnWonder = 20;
    int waitWonder = 0;
    int intelligent = 20;
    int AICycle = 0;
    int viewDistance;
    int spwnX, spwnY;
    int wandering = 0;
    int wanderingWaitTime = 60;
    virtual void fpath(sf::Vector2i vec){
        destination = vec;
        
        path.clear();
        if(x!=vec.x||y!=vec.y){
            
            path = eman->pathFindAi(this, vec);
        }
    }
    
};




#endif /* enemy_hpp */
