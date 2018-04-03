//
//  mobs.hpp
//  DR
//
//  Created by Alan Yuan on 2017-01-20.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#ifndef mobs_hpp
#define mobs_hpp
#include "enemy.hpp"
#include "eManager.hpp"
#include <stdio.h>

class mobs:public enemy{
public:
    int armourP = 0;
    int critChance = 0;
    double multiplyer = 0;
    
    mobs(sf::String s, int x,int y,int bsx, int bsy, int hsx, int hsy,int hos):enemy(s,x,y,bsx,bsy){
        
    }
    
};

class ene_scout: public mobs{

public:
    ene_scout(int x, int y):mobs("sprite thing.png",x, y,32,48,32,32,48){
        intelligent = 18;
        viewDistance = 5;
        cmd = 6;
        ani_moveSpd = 1;
        HP_PT = 10;
        atk = 1;
        
        
    }
    void wander(){
        
        
        // if need to find new path move in dirrection until forward is blocked or left forward is blocked
        if(path.size() < 1&&!moveing){
            
            int mx = 0,my = 0,tx = 0, ty = 0;
            dir-=2;
            if(dir < 1){
                dir+=8;
                
            }
            
            switch (dir) {
                case 1:
                    tx = -2;
                    ty = -1;
                    mx = 0;
                    my = -1;
                    break;
                case 3:
                    tx = 1;
                    ty = -2;
                    mx = 1;
                    my = 0;
                    break;
                case 5:
                    tx = 2;
                    ty = 1;
                    mx = 0;
                    my = 1;
                    break;
                case 7:
                    tx = -1;
                    ty = 2;
                    mx = -1;
                    my = 0;
                    break;
                default:
                    
                    dir = 1;
                    
                    wander();
                    return;
                    break;
            }
            int curX = getX();
            int curY = getY();
            std::vector<eManager::node> newPath;
            
            
            path.clear();
            
            
            while (eman->moveIn(curX+tx, curY+ty)&&eman->moveIn(curX+mx, curY+my)) {
                
                curX += mx;
                curY += my;
                
                eManager::node nextNode;
                
                nextNode.x = curX;
                nextNode.y = curY;
                
                newPath.push_back(nextNode);
            }
            if(newPath.size()>0){
                
                int repeat = newPath.size();
                
                for (int i = 0; i < repeat; i++) {
                    path.push_back(newPath[newPath.size()-1]);
                    newPath.pop_back();
                }
                
                destination = sf::Vector2i(curX, curY);
                initMove();
                
            }
            
        }
        
    }
    
    void AI(){
        enemy::AI();
        const int callRange = 5;
        //when lock on != null call out to other guys to lock on to ur lock on
        if(getLockOn()!=nullptr){
            for (int i = 0; i < eman->getAmoutOfEnt(); i++) {
                entity * e = eman->getEnt(i);
                if (e!=nullptr) {
                    if (e->getGoodGuy()==this->getGoodGuy()) {
                        int nx = (e->getX()+e->getXMove()-this->getX()-this->getXMove());
                        int ny = (e->getY()+e->getYMove()-this->getY()-this->getYMove());
                        double currentDis = sqrt(nx*nx+ny*ny);
                        if(currentDis < callRange){
                            Line line;
                            line = createLine(e->getX(), e->getY());
                            bool lineIsPassable = true;
                            
                            for (int f = 0; f < line.getSize() && lineIsPassable; f++) {
                                
                                if(!eman->moveIn(line.getCoord(f).x, line.getCoord(f).y)){
                                    lineIsPassable = false;
                                    
                                }
                            }
                            if(lineIsPassable&&e->getLockOn()==nullptr){
                                e->setDes(this->getX(), this->getY());
                            }
                        }
                    }
                }
            }
        }
        
        
    }
    
    bool ability(){
        printf("abil");
    }
};

class ene_canon_demon: public mobs{
private:
    
public:
    ene_canon_demon(int x, int y):mobs("sprite thing.png",x, y,32,48,32,32,48){
        intelligent = 16;
        viewDistance = 5;
        cmd = 6;
        ani_moveSpd = 1.5;
        attackRange = 5;
        HP_PT = 10;
        atk = 1;
    }
    
    
    
    void AI(){
        enemy::AI();
        entity * e = getLockOn();
        const int runRange = 3;
        if (e!=nullptr) {
            int nx = (e->getX()+e->getXMove()-this->getX()-this->getXMove());
            int ny = (e->getY()+e->getYMove()-this->getY()-this->getYMove());
            double currentDis = sqrt(nx*nx+ny*ny);
            if(currentDis <= runRange){
                path.clear();
                cmd = 2;
                if(this->getX()<e->getX()){
                    setDes(this->getX()-2, this->getY());
                    if(this->getY()>e->getY()){
                        setDes(this->getX()-2, this->getY()+2);
                        
                    }else if(this->getY()<e->getY()){
                        setDes(this->getX()-2, this->getY()-2);
                        
                    }
                }else if(this->getX()>e->getX()){
                    setDes(this->getX()+2, this->getY());
                    if(this->getY()>e->getY()){
                        setDes(this->getX()+2, this->getY()+2);
                        
                    }else if(this->getY()<e->getY()){
                        setDes(this->getX()+2, this->getY()-2);
                        
                    }
                }else if(this->getY()>e->getY()){
                    setDes(this->getX(), this->getY()+2);
                    
                }else if(this->getY()<e->getY()){
                    setDes(this->getX(), this->getY()-2);
                    
                }
                
            }
            
        
        }
    
    
    }

    bool ability(){
        printf("abil");
    }
};
class ene_warrior: public mobs{
private:
    
public:
    ene_warrior(int x, int y):mobs("sprite thing.png",x, y,32,48,32,32,48){
        intelligent = 30;
        viewDistance = 5;
        cmd = 6;
        ani_moveSpd = 1;
        attackRange = 1;
        HP_PT = 10;
        atk = 1;
    }
    
    
    void AI(){
        enemy::AI();
    }
    
    bool ability(){
        printf("abil");
    }
};

class ene_hunter:public mobs{
public:
    ene_hunter(int x, int y):mobs("sprite thing.png",x, y,32,48,32,32,48){
        intelligent = 30;
        viewDistance = 5;
        cmd = 6;
        ani_moveSpd = 3;
        attackRange = 1;
        hp = 10;
    }
    
    void AI(){
        if(lockedOn == nullptr){
            entity*e = eman->getPlr()->getHero(rand()%eman->getPlr()->getSize());
            if(e != nullptr){
                lockedOn = e;
                
                cmd = 5;
                
            }
        }else{
            destination = sf::Vector2i(lockedOn->getX()+lockedOn->getXMove(),lockedOn->getYMove()+lockedOn->getY());
            
        }
    }
    
};



class ene_boss: public mobs{
    class stoneOfPower: public mobs{
    private:
        int abilTimer = 0;
        int abilCD = 5000;
    public:
        ene_boss*owner;
        stoneOfPower(int x, int y, ene_boss*owner):mobs("sprite thing.png",x, y,32,48,32,32,48){
            this->owner = owner;
            goodGuy = 0;
        }
        void hurt(int atk){
            if(hp>0){
                hp = -1;
                owner->destroyStone();
            }
        }
        void AI(){}
        
        
    };
private:
    int abilTimer = 5000;
    int abilCD = 5000;
    int canBeKilled = 0;
public:
    ene_boss(int x, int y):mobs("sprite thing.png",x, y,32,48,32,32,48){
        intelligent = 2;
        viewDistance = 5;
        cmd = 6;
        ani_moveSpd = 1;
        attackRange = 1;
        HP_PT = 10;
        atk = 1;
        
    }
    
    
    void init(eManager*e){
        eman = e;
        e->sHasEnt(x,y,true);
        for (int i = 0; i < 4; i++) {
            bool placed = false;
            while (!placed) {
                int rx = rand()%eman->getRoom()->getRoomSize();
                int ry = rand()%eman->getRoom()->getRoomSize();
                if(eman->moveIn(rx, ry)&&!eman->getRoom()->gHasEnt(rx, ry)){
                    placed = true;
                    stoneOfPower*sop = new stoneOfPower(rx,ry,this);
                    eman->addEnt(sop);
                    canBeKilled++;
                }
            }
        }
        
    }
    void hurt(int atk){
        if(canBeKilled==0){
            hp-=atk;
            if(hp<0){
                dead = true;
                eman->removeEnt(this);
            }
            
        }
    }
    
    void destroyStone(){
        canBeKilled--;
    }
    
    void AI(){
        enemy::AI();
        if(abilTimer>abilCD){
            for (int x = 0; x < 5; x++) {
                for (int y = 0; y < 5; y++) {
                    if(!eman->gHasEnt(x + getX() - 2, y + getY() - 2) && eman->moveIn(x + getX() - 2, y + getY() - 2)){
                        enemy*e = new ene_hunter(x+getX()-2,y+getY()-2);
                        e->init(eman);
                        eman->addEnt(e);
                        y+=5;
                        x+=5;
                        
                    }
                    
                }
            }
            abilTimer = 0;
        }else{
            abilTimer++;
        }
        if(abs(getX()-spwnX)<2&&abs(getY()-spwnY)<2){
            spwnX = rand()%64;
            spwnY = rand()%64;
            while (!eman->moveIn(spwnX, spwnY)) {
                spwnX = rand()%64;
                spwnY = rand()%64;
            }
        }
    }
    
    bool ability(){
        printf("abil");
    }
};




#endif /* mobs_hpp */

