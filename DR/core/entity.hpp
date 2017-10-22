//
//  entity.hpp
//  Dungeons
//
//  Created by Alan Yuan on 2016-12-02.
//  Copyright © 2016 Alan Yuan. All rights reserved.
//

#ifndef entity_hpp
#define entity_hpp
#include "objects.hpp"
#include <stdio.h>
#include "Rooms.hpp"
#include <math.h> 
#include "weapon_type.hpp"

class entity: public object{
public:
    std::vector<eManager::node> path;
    
    entity(sf::String path, int x, int y, int team, int bsX, int bsY, int hsX,int hsY, int hos):object(path, x, y, team,bsX,bsY,hsX,hsY,hos){
        destination.x = x;
        destination.y = y;
    }
    
    int getXMove(){
        return xMove;
    }
    int getYMove(){
        return yMove;
    }
    int* getMXHP(){
        return &HP_PT;
    }
    int* getHP(){
        return &hp;
    }
    void setLoc(int x,int y){
        this->x = x;
        this->y = y;
        destination.x = x;
        destination.y = y;
        
    }
    void setDes(int x,int y){
        
        destination.x = x;
        destination.y = y;
        
    }
    
    
    void setLock(entity*ent){
        lockedOn = ent;
    }
    
    virtual void init(eManager*e){
        
        eman = e;
        e->sHasEnt(x,y,true);
    }
    
    // adding xMove to X and checking collision
    bool takeErg(int amount){
        if(erg-amount<0){
            
            return false;
        }else{
            erg-=amount;
            return true;
        }
    }
    
    virtual void initMove(){
        
        eman->sHasEnt(x+xMove,y+yMove,false);
        
        //path.pop_back();
        if(path.size()>0){
            path.pop_back();
            
            if(path.end()->x > x){
                xMove = 1;
                dir = 0;
                if(path.end()->y > y){
                    yMove = 1;
                    
                }else if(path.end()->y < y){
                    yMove = -1;
                    
                }
            }else if(path.end()->x < x){
                xMove = -1;
                dir = 1;
                if(path.end()->y > y){
                    yMove = 1;
                    
                }else if(path.end()->y < y){
                    yMove = -1;
                    
                }
            }else if(path.end()->y > y){
                yMove = 1;
                
            }else if(path.end()->y < y){
                yMove = -1;
                
            }
        
            
            
            if (eman->moveIn(x+xMove, y+yMove)&&!eman->getRoom()->gHasEnt(x+xMove, y+yMove)) {
                moveing = true;
            }else {
                xMove = 0;
                yMove = 0;
                moveing = false;
            }
            
        }
        
        eman->sHasEnt(x+xMove,y+yMove,true);
    }
    
    virtual void fpath(sf::Vector2i vec){
        destination = vec;
        path.clear();
        if(x!=vec.x||y!=vec.y){
            path = eman->pathFind(this, vec);
        }
    }
    
    virtual bool movePhase(){
        
        if(moveing){
            double ani_dir = atan2((y+yMove)*eman->getTileSizeY()-y*eman->getTileSizeY()-yOffSet,(x+xMove)*eman->getTileSizeX()-x*eman->getTileSizeX()-xOffSet);
            
            
            
            if (eman->moveIn((xOffSet+cos(ani_dir)*ani_moveSpd+eman->getTileSizeX()/2+(bodyHitBox.getSize().x/2-1))/eman->getTileSizeX()+x, (yOffSet+eman->getTileSizeY()/2)/eman->getTileSizeY()+y)
                &&
                eman->moveIn((xOffSet+cos(ani_dir)*ani_moveSpd+eman->getTileSizeX()/2-(bodyHitBox.getSize().x/2-1))/eman->getTileSizeX()+x, (yOffSet+eman->getTileSizeY()/2)/eman->getTileSizeY()+y)) {
                xOffSet+=cos(ani_dir)*ani_moveSpd;
            }else{
                yOffSet+=(sin(ani_dir))*ani_moveSpd/2;
            }
            
            if (eman->moveIn((xOffSet+eman->getTileSizeX()/2+(bodyHitBox.getSize().x/2-1))/eman->getTileSizeX()+x, (yOffSet+sin(ani_dir)*ani_moveSpd+eman->getTileSizeY()/2)/eman->getTileSizeY()+y)
                &&
                eman->moveIn((xOffSet+eman->getTileSizeX()/2-(bodyHitBox.getSize().x/2-1))/eman->getTileSizeX()+x, (yOffSet+sin(ani_dir)*ani_moveSpd+eman->getTileSizeY()/2)/eman->getTileSizeY()+y)) {
                yOffSet+=sin(ani_dir)*ani_moveSpd;
            }else{
                xOffSet+=(cos(ani_dir))*ani_moveSpd/2;
            }
//            xOffSet+=cos(ani_dir)*ani_moveSpd;
//            yOffSet+=sin(ani_dir)*ani_moveSpd;
//
        }
        
        double nx = ((xMove+x)*eman->getTileSizeX() - (x*eman->getTileSizeX() + xOffSet));
        double ny = ((yMove+y)*eman->getTileSizeY() - (y*eman->getTileSizeY() + yOffSet));
        
        if(sqrt(nx*nx + ny*ny)<ani_moveSpd*1.01){
            finish();
        }
        
        if (!moveing) {
            // this means this ent is done moving
            return true;
        }else{
            return false;
        }
        
    }
    
    
    
    virtual bool tick(sf::RenderWindow*window){
        
        attackxOffSet = 0;
        attackyOffSet = 0;
        
        if(currentFrameTime>frameSwitchTime){
            currentFrameTime=0;
            currentFrame++;
            if(currentFrame>=defaultFrames){
                currentFrame = 0;
            }
        }else{
            currentFrameTime++;
        }
        
        if(attackWaitTime>0){
            attackWaitTime--;
        }
        
        
        sprite.setTextureRect(sf::IntRect((currentAction*defaultFrames+currentFrame)*(picSize),(dir+1)*picSize,picSize,picSize));
        return false;
    }
    
    virtual bool attackPhase(){
        
        return true;
    }
    
    
    
    virtual void damage(entity*e,entity*a){
        
    }
    
    virtual bool ability(){
        
    }
    
    virtual void hurt(int dmg){
        healing = 0;
        hp-=dmg;
        if(hp<0){
            dead = true;
            eman->removeEnt(this);
        }
        
        
    }
    
    Line createLine(int nx, int ny){
        Line line;
        
        if(getX() == nx){
            // straight line up or down
            int startY, endY;
            
            if(getY() < ny){
                startY = getY();
                endY = ny;
            }else{
                startY = ny;
                endY = getY();
            }
            
            for (int y = startY; y < endY+1; y++) {
                line.addCoord(getX(), y);
            }
            
            return line;
            
        }else if (getY() == ny){
            // straight line left or right
            int startX, endX;
            if(getX() < nx){
                startX = getX();
                endX = nx;
            }else{
                startX = nx;
                endX = getX();
            }
            
            for (int x = startX; x < endX+1; x++) {
                line.addCoord(x, getY());
            }
            
            return line;
            
        }
        
        
        double slope = ((ny - (double)getY())/(nx - getX()));
        double threshold = 0.5;
        int adjust = 0;
        double offset = 0;
        
        if(slope <= 1 && slope >= -1){
            if(getX()<nx){
                if(slope>0){
                    // x -> nx & y goes up
                    for (int x = getX(); x < nx+1; x++) {
                        line.addCoord(x, adjust+getY());
                        offset+=slope;
                        if(abs(offset)>=threshold){
                            threshold = threshold + 1;
                            adjust++;
                        }
                        
                        
                    }
                }else{
                    // x -> nx & y goes down
                    for (int x = getX(); x < nx+1; x++) {
                        line.addCoord(x, getY()-adjust);
                        offset+=slope;
                        if(offset<=-threshold){
                            threshold = threshold + 1;
                            adjust++;
                        }
                    
                    }
                }
            }else{
                if(slope>0){
                    // nx <- x & y goes up
                    for (int x = getX(); x > nx-1; x--) {
                        line.addCoord(x, getY()-adjust);
                        offset+=slope;
                        if(abs(offset)>=threshold){
                            threshold = threshold + 1;
                            adjust++;
                        }
                        
                    }
                }else{
                    // nx <- x & y goes down
                    for (int x = getX(); x > nx-1; x--) {
                        line.addCoord(x, getY()+adjust);
                        offset+=slope;
                        if(offset<=-threshold){
                            threshold = threshold + 1;
                            adjust++;
                        }
                    }
                }
            }
            
            
        }else{
            slope = ((nx - getX())/(ny - (double)getY()));

            if(getY()<ny){
                if(slope>0){
                    // y -> ny & x goes right
                    for (int y = getY(); y < ny+1; y++) {
                        line.addCoord(adjust+getX(), y);
                        offset+=slope;
                        if(abs(offset)>=threshold){
                            threshold = threshold + 1;
                            adjust++;
                        }
                        
                        
                    }
                }else{
                    // y -> ny & x goes left
                    for (int y = getY(); y < ny+1; y++) {
                        line.addCoord(getX()-adjust, y);
                        offset+=slope;
                        if(offset<=-threshold){
                            threshold = threshold + 1;
                            adjust++;
                        }
                        
                    }
                }
            }else{
                if(slope>0){
                    // ny <- x & x goes right
                    for (int y = getY(); y > ny-1; y--) {
                        line.addCoord(getX()-adjust, y);
                        offset+=slope;
                        if(abs(offset)>=threshold){
                            threshold = threshold + 1;
                            adjust++;
                        }
                        
                    }
                }else{
                    // ny <- y & x goes left
                    for (int y = getY(); y > ny-1; y--) {
                        line.addCoord(getX()+adjust, y);
                        offset+=slope;
                        if(offset<=-threshold){
                            threshold = threshold + 1;
                            adjust++;
                        }
                    }
                }
            }

        }
        return line;
        
    }
    
    // below is for reseting moving after moving slowly to the correct square
    void finish(){
        
        x+=xMove;
        xOffSet = 0;
        xMove = 0;
        y+=yMove;
        yOffSet = 0;
        yMove = 0;
        moveing =false;
    }
    
    //pathfind
    
    entity*getLockOn(){
        return lockedOn;
    }
    void setLockOn(entity*l){
        lockedOn = l;
    }
    virtual int getGoodGuy(){
        return goodGuy;
    }
    void setCMD(int cmd){
        this->cmd = cmd;
    }
    virtual int getAtk(){
        return atk;
    }
    virtual bool attack(){
        
        
        
        if(cmd>2 && getLockOn()!=nullptr && moveing == false){
            int xdis = getLockOn()->getX()-getX();
            
            int ydis = getLockOn()->getY()-getY();
            if (abs(ydis) <= attackRange&&abs(xdis) <= attackRange) {
                
                if(lockedOn->x > x){
                    dir = 0;
                }else if(lockedOn->x < x){
                    dir = 1;
                }
                if(attackWaitTime == 0){
                    attackxOffSet = (lockedOn->getX()+lockedOn->getXMove()-getX()-getXMove())*eman->getTileSizeX()/4;
                    attackyOffSet = (lockedOn->getY()+lockedOn->getYMove()-getYMove()-getY())*eman->getTileSizeY()/4;
                    getLockOn()->hurt(getAtk()+currentWeapon.getATK());
                    attackWaitTime = currentWeapon.getTime()+2;
                    
                }else if (attackWaitTime<3){
                    attackxOffSet = (lockedOn->getX()+lockedOn->getXMove()-getX()-getXMove())*eman->getTileSizeX()/4;
                    attackyOffSet = (lockedOn->getY()+lockedOn->getYMove()-getYMove()-getY())*eman->getTileSizeY()/4;
                }else if (attackWaitTime < 6){
                    attackxOffSet = (lockedOn->getX()+lockedOn->getXMove()-getX()-getXMove())*eman->getTileSizeX()/8;
                    attackyOffSet = (lockedOn->getY()+lockedOn->getYMove()-getYMove()-getY())*eman->getTileSizeY()/8;
                }
                
                moveing = false;
                return true;
            }
            
            
        }
        return false;
    }
    weapon * getCurrentWeapon(){
        return &currentWeapon;
    }
    void setWeapon(weapon wep){
        currentWeapon = wep;
    }
protected:
    
    sf::Vector2i destination;
    weapon currentWeapon = Knife();
    // direction in which the entity is facing (gt = go to)
    short dir = 1;
    //
    // 0 1
    //   
    double attackRange = 1;
    int currentFrame = 0;
    const int frameSwitchTime = 6;
    int currentFrameTime = 0;
    int currentAction = 0;
    int defaultFrames = 4;
    const int totalFrames[4] = {8,8,8,8};
    int attackWaitTime = 0;
    
    bool moveing = false;
    
    int distanceFromDesMax = 0;
    
    entity* lockedOn = nullptr;
    
    int cmd = 0;
    // 0 complete stop
    // 1 follow
    // 2 move
    // 3 manual lock
    // 4 hold
    // 5 a attack
    // 6 sit around
    
    int healing = 0;
    double ani_moveSpd = 1;
    int ani_moveFrames = 1;
    int amountOfAttackFrames = 2;
    int damage_frame = 1;
};
#endif /* entity_hpp */
