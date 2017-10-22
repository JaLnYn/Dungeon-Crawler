//
//  hero.cpp
//  Dungeons
//
//  Created by Alan Yuan on 2016-12-05.
//  Copyright © 2016 Alan Yuan. All rights reserved.
//


#include "hero.hpp"
#include "classes.hpp"



hero::hero(sf::String path,int x,int y):entity(path,x,y,1,24,30,16,16,30+16){
    ani_moveSpd = 2;
    hpBlackGround.setSize(sf::Vector2f(hpBarSizeX,hpBarSizeY*2));
    energyBar.setSize(sf::Vector2f(hpBarSizeX,hpBarSizeY));
    energyBar.setFillColor(sf::Color::Blue);
    hpBackGround.setSize(sf::Vector2f(hpBarSizeX+2,hpBarSizeY*2+2));
    equipBG.setSize(sf::Vector2f(renderSizeWep,renderSizeWep));
}

void hero::renderWepEquip(sf::RenderWindow *window, int i){
    //draw weapon image here;
    
    equipBG.setPosition(window->mapPixelToCoords(sf::Vector2i(0, eman->getRoom()->getYOFF()-64-32*4-(renderSizeWep+2)*2*i)));
    window->draw(equipBG);
}

void hero::heal(int rate){
    healing = rate;
}
void hero::init(eManager*e){
    entity::init(e);
    sprite.setTextureRect(sf::IntRect(0,0,picSize,picSize));
}



void hero::lvlUP(){
    atk++;
    HP_PT++;
    hp = HP_PT;
}

void hero::render(sf::RenderWindow*window){
    object::render(window);
    
    energyBar.setSize(sf::Vector2f(hpBarSizeX*abilTimer/abilCD,hpBarSizeY));
    //hpBar
    
    energyBar.setPosition(x*eman->getTileSizeX()+eman->getTileSizeX()/2+xOffSet-hpBarSizeX/2, y*eman->getTileSizeY()+eman->getTileSizeY()/2+yOffSet+hpBarSizeY+3+barOffSet);
    
    
    window->draw(energyBar);
    
}

bool hero::searchLock(){
    if(cmd > 3){
        
        bool hasLockedOn;
        double curLockedRange;
        if(lockedOn == nullptr){
            hasLockedOn = false;
            curLockedRange = __DBL_MAX__;
        }else{
            hasLockedOn = true;
            double nx = (getX() + getXMove() - lockedOn->getX() - lockedOn->getXMove());
            double ny = (getY() + getYMove() - lockedOn->getY() - lockedOn->getYMove());
            curLockedRange = sqrt(nx*nx + ny*ny);
        }
        
        int yStart = this->y - lockArraySize/2;
        
        for (int y = 0 ; y < lockArraySize; y++) {
            int xStart = this->x - lockRange[y]/2;
            for (int x = 0; x < lockRange[y]; x++) {
                entity* potentialEnt = eman->checkArea(xStart + x, yStart + y);
                if(potentialEnt != nullptr && eman->getRoom()->getSeeableTile(xStart + x, yStart + y)){
                    double nx = (getX() + getXMove() - potentialEnt->getX() - potentialEnt->getXMove());
                    double ny = (getY() + getYMove() - potentialEnt->getY() - potentialEnt->getYMove());
                    if(potentialEnt->getGoodGuy()!=goodGuy&&potentialEnt->getGoodGuy()!=0){
                        if(hasLockedOn){
                            if(sqrt(nx*nx+ny*ny) < curLockedRange){
                                // smallest distance && player has not manually locked someone && the   previous lock's lock is not on you and you are not on the same team
                                lockedOn = potentialEnt;
                                nx = (getX() + getXMove() - lockedOn->getX() - lockedOn->getXMove());
                                ny = (getY() + getYMove() - lockedOn->getY() - lockedOn->getYMove());
                                curLockedRange = sqrt(nx*nx + ny*ny);
                            }
                        }else{
                            lockedOn = potentialEnt;
                            nx = (getX() + getXMove() - lockedOn->getX() - lockedOn->getXMove());
                            ny = (getY() + getYMove() - lockedOn->getY() - lockedOn->getYMove());
                            curLockedRange = sqrt(nx*nx + ny*ny);
                            hasLockedOn = true;
                        }
                    }
                }
            }
        }
        if(lockedOn!=nullptr&&eman->getRoom()->getSeeableTile(lockedOn->getX(), lockedOn->getY())&&cmd>4){
            destination = sf::Vector2i(lockedOn->getX()+lockedOn->getXMove(),lockedOn->getY()+lockedOn->getYMove());
            fpath(destination);
        }
    }
}

bool hero::tick(sf::RenderWindow *window){
    entity::tick(window);
    hp = HP_PT;
    if(healing>0){
        
        if(hp+healing>HP_PT){
            hp = HP_PT;
            healing = 0;
        }else{
            hp+=healing;
        }
    }
    ability();
    searchLock();
    
    if(usingAbil){
        abilTimer-=cdSubtract;
        if(abilTimer<=0){
            usingAbil=false;
            abilTimer = 0;
        }
    }else if(abilTimer<abilCD){
        abilTimer++;
    }
    
    
    //NOTE: if attack is within range then attack
    if(stopForThisTurn){
        stopForThisTurn = false;
    }else if(attack()){
        currentAction = 0;
    }else if (!moveing) {
        if(cmd == 4||cmd==0){
            currentAction = 0;
        }else if (path.size()>0) {
            if(path[path.size()-1].x!=destination.x && path[path.size()-1].y != destination.y){
                fpath(destination);
            }
            initMove();
        }else if (abs(x-destination.x) > distanceFromDesMax || abs(y-destination.y) > distanceFromDesMax){
            fpath(destination);
            initMove();
            distanceFromDesMax++;
        }else{
            distanceFromDesMax = 0;
            setDes(this->getX(), this->getY());
            if(lockedOn!=nullptr){
                destination.x = lockedOn->getXMove()+lockedOn->getX();
                destination.y = lockedOn->getYMove()+lockedOn->getY();
            }else{
                cmd = 6; // sit around
                currentAction = 0;
            }
        }
    }else{
        currentAction = 1;
        movePhase();
        return true;
    }
    return false;
}

void hero::groundGui(sf::RenderWindow*window){
    
    
    
//    for (int i = 0; i<path.size(); i++) {
//        //printf("%d\n",path.size());
//        sf::RectangleShape rect;
//        rect.setSize(sf::Vector2f(eman->getTileSizeX(),eman->getTileSizeY()));
//        rect.setPosition(path[i].x*eman->getTileSizeX(), path[i].y*eman->getTileSizeY());
//
//        
//        window->draw(rect);
//    }
    
    
}


