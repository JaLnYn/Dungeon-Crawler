 //
//  eManager.cpp
//  DR
//
//  Created by Alan Yuan on 2017-01-13.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#include "eManager.hpp"
#include "classes.hpp"
#include "Rooms.hpp"
#include <limits.h>
#include <Player.hpp>
#include "mobs.hpp"

struct eManager::sorter{
    /**
     Purpose: 
        To sort the entities given by their y position
     Parameter: 
        Entity a is the first entity you want to compare
        Entity b is the secnd entity you want to compare
     Return:
        whether or not the y position of a is smaller than the y position of b
     **/
    bool operator() ( entity * a, entity * b)
    {
        return (a->getY()+a->getYMove() < b->getY()+b->getYMove());
    }
};


void eManager::setRoom(room*room){
    this->rm = room;
}

int eManager::getTileSizeX(){
    return rm->getRenderSizeX();
}


int eManager::getTileSizeY(){
    return rm->getRenderSizeY();
    
}


void eManager::addEnt(entity * ent){
    
        //ents[ents.size()]= ent;
        ents.push_back(ent);
        ent->init(this);
    
}
void eManager::addTooBG(enemy *e){
    badGuys.push_back(e);
}

void eManager::addppl(){
    ents.clear();
    for (int i = 0; i < badGuys.size(); i++) {
        delete badGuys[i];
    }
    badGuys.clear();
    for (int i = 0; i < rm->getCampSize(); i++) {
        sf::Vector2i location;
        location = rm->getCampLoc(i);
        if(rand()%2){
            enemy*x = new ene_scout(location.x,location.y);
            addEnt(x);
            badGuys.push_back(x);
            enemy*y = new ene_scout(location.x+1,location.y);
            addEnt(y);
            badGuys.push_back(y);
        }
        if(rand()%2){
            enemy*asd = new ene_canon_demon(location.x+1,location.y+1);
            addEnt(asd);
            badGuys.push_back(asd);
            if(rand()%2){
                enemy*ad = new ene_canon_demon(location.x,location.y+1);
                addEnt(ad);
                badGuys.push_back(ad);
            }
        }
        
        enemy*asd = new ene_warrior(location.x-1,location.y-1);
        addEnt(asd);
        badGuys.push_back(asd);
        if(rand()%2){
            enemy*ad = new ene_canon_demon(location.x,location.y-1);
            addEnt(ad);
            badGuys.push_back(ad);
        }
        
    }
    
    bossMan->setLoc(rm->getEndX(), rm->getEndY());
    
    heros[1]->setLoc(rm->getSpwnX()+1, rm->getSpwnY()+1);
    heros[2]->setLoc(rm->getSpwnX()+1, rm->getSpwnY()-1);
    heros[3]->setLoc(rm->getSpwnX()-1, rm->getSpwnY()+1);
    heros[0]->setLoc(rm->getSpwnX()-1, rm->getSpwnY()-1);
    
    addEnt(bossMan);
    addEnt(heros[1]);
    addEnt(heros[2]);
    addEnt(heros[3]);
    addEnt(heros[0]);
    
    
    
    plr->setVision();
}

int eManager::getAmoutOfEnt(){
    
    return ents.size();
}

void eManager::init(room*room, sf::RenderWindow*window){
    
    dots[0].setFillColor(sf::Color::White);
    dots[1].setFillColor(sf::Color::Green);
    dots[2].setFillColor(sf::Color::Red);
    
    heros.push_back(new soldier(0,0));
    heros.push_back(new soldier(0,0));
    heros.push_back(new soldier(0,0));
    heros.push_back(new soldier(0,0));
    
    
    plr = new Player(this,window,heros[0],heros[1],heros[2],heros[3]);
    setRoom(room);
    plr->SetGuiStart();
    tileShader.setSize(sf::Vector2f(getTileSizeX(),getTileSizeY()));
    dots[0].setSize(sf::Vector2f(rm->getMapPixelSize()/2,rm->getMapPixelSize()/2));
    dots[1].setSize(sf::Vector2f(rm->getMapPixelSize()/2,rm->getMapPixelSize()/2));
    dots[2].setSize(sf::Vector2f(rm->getMapPixelSize()/2,rm->getMapPixelSize()/2));

    bossMan = new ene_boss(0,0);
    
    plr->init();
    
}

void eManager::gainPotion(){
    
    plr->plusPotion();
    

}

void eManager::removeEnt(entity * ent){
    for (int i = 0; i < ents.size(); i++) {
        if(ent == ents[i]->getLockOn()){
            ents[i]->setLockOn(nullptr);
        }
    }
    
    if(ent->getGoodGuy()==2){
        for (int i = 0; i < badGuys.size(); i++) {
            if (ent == badGuys[i]) {
                badGuys.erase(badGuys.begin()+i);
                
            }
        }
    }else if(ent->getGoodGuy()==1){
        for (int i = 0; i < heros.size(); i++) {
            if (ent == heros[i]) {
                heros.erase(heros.begin()+i);
                plr->removeH(ent);
                
            }
        }
    }
    sHasEnt(ent->getX()+ent->getXMove(), ent->getY()+ent->getYMove(), false);
    for (int i = 0; i < ents.size(); i++) {
        if (ent == ents[i]) {
            ents.erase(ents.begin()+i);
            delete ent;
        }
    }
}

void eManager::tick(sf::RenderWindow * window){
    monsterXp+=300;
    if(monsterLv>=xpPerLv){
        monsterXp = 0;
        monsterLv++;
    }
    
    
    bool hasMoved = 0;
    
    for (int i = 0; i < ents.size(); i++) {
        if(ents[i]->tick(window)){
            hasMoved = true;
        }
    }
    
    if(hasMoved){
        std::sort(ents.begin(), ents.end(), sorter());
        plr->setVision();
    }
    
    plr->tick(window);
}

void eManager::render(sf::RenderWindow * window, sf::View * cam){
    
    plr->renderGroundGui(window);
    
    window->setView(*getView());
    
    //ground gui here
    //h->groundGui(window);
    
    for (int i = 0; i<ents.size(); i++) {
        if(ents[i]->getSee()){
            ents[i]->render(window);
        }
    }
    
    int startX = (cam->getCenter().x - (20 * getTileSizeX()) )/getTileSizeX();
    int endX = (cam->getCenter().x + (20 * getTileSizeX()) )/getTileSizeX();
    
    int startY = (cam->getCenter().y - (14 * getTileSizeY()) )/getTileSizeY();
    int endY = (cam->getCenter().y + (9 * getTileSizeY()) )/getTileSizeY();
    
    startX = std::max(startX, 0);
    startY = std::max(startY, 0);
    endX = std::min(endX, rm->getRoomSize());
    endY = std::min(endY, rm->getRoomSize());
    
    /* Same with the X values so that we don't access values outside our array. */
    
    
    for(int y = startY; y < endY; y++){
        for(int x = startX; x < endX; x++){
            if(rm->getSeeableTile(x, y)>0){
                //max is 6
                tileShader.setFillColor(sf::Color(0,0,0,(7-rm->getSeeableTile(x, y))*30));
                tileShader.setPosition(x*getTileSizeX(), y*getTileSizeY());
                window->draw(tileShader);
            }else if(rm->getSeenTile(x, y)){
                tileShader.setFillColor(sf::Color(0,0,0,210));
                tileShader.setPosition(x*getTileSizeX(), y*getTileSizeY());
                window->draw(tileShader);
            }else{
                tileShader.setFillColor(sf::Color(0,0,0,250));
                tileShader.setPosition(x*getTileSizeX(), y*getTileSizeY());
                window->draw(tileShader);
            }

        }
    }
    
    
}

void eManager::nextEnt(){
    
    std::sort(ents.begin(), ents.end(), sorter());
    turn++;
    if (turn>(ents.size()-1)) {
        turn = 0;
        phase++;
    }
}

std::vector<eManager::node> eManager::pathFind(entity*ent,sf::Vector2i go){
    std::vector<node> nodes;
    
    std::vector<eManager::node> returnThis;
    
    node n;
    //add this to open node
    n.x = ent->getX();
    n.y = ent->getY();
    n.open = true;
    n.fNode = true;
    
    nodes.push_back(n);
    
    int current = openNodeFirst(0,ent, go, &nodes);
    
    while (current) {
        returnThis.push_back(nodes[current]);
        current = nodes[current].parent;
    }
    
    return returnThis;
}

bool eManager::lineCollision(Line line){
    for (int i = 0; i < line.getSize(); i++) {
        if(getRoom()->isSolid(line.getCoord(i).x, line.getCoord(i).y)){
            return false;
        }
    }
    return true;
}

int eManager::openNodeFirst (int id,entity*ent, sf::Vector2i go,std::vector<node>*nodes){
    //open all surroudning stuff
    (*nodes)[id].open = false;
    for (int i = 0; i<9; i++) {
        if(i == 4){
            continue; // go to 5
        }
        node newn;
        newn.x = (*nodes)[id].x-1+i/3;
        newn.y = (*nodes)[id].y-1+i%3;
        newn.parent = id;
        
        if( moveIn(newn.x, newn.y) && (!rm->gHasEnt(newn.x, newn.y)||(newn.x==ent->getXMove()+ent->getX()&&newn.y == ent->getYMove()+ent->getY())) && rm->getSeenTile(newn.x, newn.y)){ // not solid
            if ( newn.x == go.x && newn.y == go.y) {
                (*nodes).push_back(newn);
                return nodes->size()-1;// we have reached the end
            }
            if(newn.x == (*nodes)[id].x || newn.y == (*nodes)[id].y){
                newn.gcost = (*nodes)[id].gcost + 1;
            }else{
                newn.gcost = (*nodes)[id].gcost + 1.4;
            }
            int nx,ny;
            nx = newn.x - go.x;
            ny = newn.y - go.y;
            newn.hcost = sqrt(nx*(double)nx + (double)ny*ny);
            newn.fcost = newn.gcost + newn.hcost;
            int preID = -1;
            for (int i = 0; i<nodes->size(); i++) {
                if((*nodes)[i].x == newn.x && (*nodes)[i].y == newn.y){
                    preID = i;
                }
            }
            if(preID == -1){
                // the new x and y is not a node
                // meaning we can add this after setting fcost hcost parent and open
                newn.open = true;
                nodes->push_back(newn);
            }else{
                // there is already a node that is on that x and y
                // meaning we haave to check whether this is shorter to get there or the other one is
                node pre = (*nodes)[preID];
                if(pre.gcost > newn.gcost){
                    pre.parent = id; // it is shorter so this one should replace it
                    pre.gcost = newn.gcost;
                }
            }
        }
    }
    double lowestCost = __DBL_MAX__;
    int curID = 0;
    bool picked = false;
    int i = 1;
    while (picked == false && i < nodes->size()) {
        if((*nodes)[i].open){
            lowestCost = (*nodes)[i].fcost;
            curID = i;
            picked = true;
        }else{
            // if nothing is open then we return the lowest hcost node
            if((*nodes)[i].hcost < lowestCost){
                lowestCost = (*nodes)[i].hcost;
                curID = i;
            }
            i++;
        }
    }
    if(!picked){
        return curID;
    }
    for (int i = 0; i<(*nodes).size(); i++) {
        if((*nodes)[i].fcost < lowestCost && (*nodes)[i].open){
            lowestCost = (*nodes)[i].fcost;
            curID = i;
        }
    }
    return openNode(curID, go, nodes);
}

int eManager::openNode(int id, sf::Vector2i go, std::vector<node>*nodes){
    
    //open all surroudning stuff
    //
    
    (*nodes)[id].open = false;
    
    for (int i = 0; i<9; i++) {
        if(i == 4){
            continue; // go to 5
        }
        node newn;
        newn.x = (*nodes)[id].x-1+i/3;
        newn.y = (*nodes)[id].y-1+i%3;
        newn.parent = id;
        
        
        //printf("%d, %d\n",newn.x,newn.y);
        if( moveIn(newn.x, newn.y) && rm->getSeenTile(newn.x, newn.y)){ // not solid
            if ( newn.x == go.x && newn.y == go.y) {
                (*nodes).push_back(newn);
                return nodes->size()-1;// we have reached the end
            }
            if(newn.x == (*nodes)[id].x || newn.y == (*nodes)[id].y){
                newn.gcost = (*nodes)[id].gcost + 1;
            }else{
                newn.gcost = (*nodes)[id].gcost + 1.4;
            }
            
            int nx,ny;
            
            nx = newn.x - go.x;
            ny = newn.y - go.y;
            
            newn.hcost = sqrt(nx*(double)nx + (double)ny*ny);
            
            
            newn.fcost = newn.gcost + newn.hcost;
            
            
            int preID = -1;
            for (int i = 0; i<nodes->size(); i++) {
                if((*nodes)[i].x == newn.x && (*nodes)[i].y == newn.y){
                    preID = i;
                }
            }
            

            if(preID == -1){
                // the new x and y is not a node
                // meaning we can add this after setting fcost hcost parent and open
                newn.open = true;
                nodes->push_back(newn);
                
            }else{
                // there is already a node that is on that x and y
                // meaning we haave to check whether this is shorter to get there or the other one is
                
                
                node pre = (*nodes)[preID];
                
                if(pre.gcost > newn.gcost){
                    
                    pre.parent = id; // it is shorter so this one should replace it
                    pre.gcost = newn.gcost;
                }
                
                
            }
        }else{
            
        }
    }
    
    // start another openNode
    double lowestCost = __DBL_MAX__;
    int curID = 0;
    bool picked = false;
    
    
    int i = 1;
    while (picked == false && i < nodes->size()) {
        if((*nodes)[i].open){
            lowestCost = (*nodes)[i].fcost;
            curID = i;
            picked = true;
        }else{
            // if nothing is open then we return the lowest hcost node
            if((*nodes)[i].hcost < lowestCost){
                lowestCost = (*nodes)[i].hcost;
                curID = i;
            }
            i++;
        }
    }
    
    if(!picked){
        
        return curID;
    }
    
    for (int i = 0; i<(*nodes).size(); i++) {
        if((*nodes)[i].fcost < lowestCost && (*nodes)[i].open){
            lowestCost = (*nodes)[i].fcost;
            curID = i;
        }
    }
    

    
    return openNode(curID, go, nodes);
    
}

std::vector<eManager::node> eManager::pathFindAi(entity*ent,sf::Vector2i go){
    std::vector<node> nodes;
    
    std::vector<eManager::node> returnThis;
    
    node n;
    //add this to open node
    n.x = ent->getX();
    n.y = ent->getY();
    n.open = true;
    n.fNode = true;
    
    nodes.push_back(n);
    
    int current = openNodeFirstAi(0, go, &nodes, ent->getX(), ent->getY());
    
    while (current) {
        returnThis.push_back(nodes[current]);
        current = nodes[current].parent;
    }
    
    return returnThis;
}

int eManager::openNodeFirstAi (int id, sf::Vector2i go,std::vector<node>*nodes,int entX, int entY){
    //open all surroudning stuff
    (*nodes)[id].open = false;
    
    for (int i = 0; i<9; i++) {
        if(i == 4){
            continue; // go to 5
        }
        node newn;
        newn.x = (*nodes)[id].x-1+i/3;
        newn.y = (*nodes)[id].y-1+i%3;
        newn.parent = id;
        
        if( moveIn(newn.x, newn.y) && !rm->gHasEnt(newn.x, newn.y)&&abs(newn.x-entX)<12&&abs(newn.y-entY)<12){ // not solid
            if ( newn.x == go.x && newn.y == go.y) {
                (*nodes).push_back(newn);
                return nodes->size()-1;// we have reached the end
            }
            if(newn.x == (*nodes)[id].x || newn.y == (*nodes)[id].y){
                newn.gcost = (*nodes)[id].gcost + 1;
            }else{
                newn.gcost = (*nodes)[id].gcost + 1.4;
            }
            int nx,ny;
            nx = newn.x - go.x;
            ny = newn.y - go.y;
            newn.hcost = sqrt(nx*(double)nx + (double)ny*ny);
            newn.fcost = newn.gcost + newn.hcost;
            int preID = -1;
            for (int i = 0; i<nodes->size(); i++) {
                if((*nodes)[i].x == newn.x && (*nodes)[i].y == newn.y){
                    preID = i;
                }
            }
            if(preID == -1){
                // the new x and y is not a node
                // meaning we can add this after setting fcost hcost parent and open
                newn.open = true;
                nodes->push_back(newn);
            }else{
                // there is already a node that is on that x and y
                // meaning we haave to check whether this is shorter to get there or the other one is
                node pre = (*nodes)[preID];
                if(pre.gcost > newn.gcost){
                    pre.parent = id; // it is shorter so this one should replace it
                    pre.gcost = newn.gcost;
                }
            }
        }
    }
    double lowestCost = __DBL_MAX__;
    int curID = 0;
    bool picked = false;
    int i = 1;
    while (picked == false && i < nodes->size()) {
        if((*nodes)[i].open){
            lowestCost = (*nodes)[i].fcost;
            curID = i;
            picked = true;
        }else{
            // if nothing is open then we return the lowest hcost node
            if((*nodes)[i].hcost < lowestCost){
                lowestCost = (*nodes)[i].hcost;
                curID = i;
            }
            i++;
        }
    }
    if(!picked){
        return curID;
    }
    for (int i = 0; i<(*nodes).size(); i++) {
        if((*nodes)[i].fcost < lowestCost && (*nodes)[i].open){
            lowestCost = (*nodes)[i].fcost;
            curID = i;
        }
    }
    return openNodeAi(curID, go, nodes, entX, entY);
}

int eManager::openNodeAi(int id, sf::Vector2i go, std::vector<node>*nodes, int x, int y){
    
    //open all surroudning stuff
    //
    
    (*nodes)[id].open = false;
    
    for (int i = 0; i<9; i++) {
        if(i == 4){
            continue; // go to 5
        }
        node newn;
        newn.x = (*nodes)[id].x-1+i/3;
        newn.y = (*nodes)[id].y-1+i%3;
        newn.parent = id;
        
        
        //printf("%d, %d\n",newn.x,newn.y);
        if( moveIn(newn.x, newn.y)&&abs(newn.x-x)<12&&abs(newn.y-y)<12){ // not solid
            if ( newn.x == go.x && newn.y == go.y) {
                (*nodes).push_back(newn);
                return nodes->size()-1;// we have reached the end
            }
            if(newn.x == (*nodes)[id].x || newn.y == (*nodes)[id].y){
                newn.gcost = (*nodes)[id].gcost + 1;
            }else{
                newn.gcost = (*nodes)[id].gcost + 1.4;
            }
            
            int nx,ny;
            
            nx = newn.x - go.x;
            ny = newn.y - go.y;
            
            newn.hcost = sqrt(nx*(double)nx + (double)ny*ny);
            
            
            newn.fcost = newn.gcost + newn.hcost;
            
            
            int preID = -1;
            for (int i = 0; i<nodes->size(); i++) {
                if((*nodes)[i].x == newn.x && (*nodes)[i].y == newn.y){
                    preID = i;
                }
            }
            
            
            if(preID == -1){
                // the new x and y is not a node
                // meaning we can add this after setting fcost hcost parent and open
                newn.open = true;
                nodes->push_back(newn);
                
            }else{
                // there is already a node that is on that x and y
                // meaning we haave to check whether this is shorter to get there or the other one is
                
                
                node pre = (*nodes)[preID];
                
                if(pre.gcost > newn.gcost){
                    
                    pre.parent = id; // it is shorter so this one should replace it
                    pre.gcost = newn.gcost;
                }
                
                
            }
        }else{
            
        }
    }
    
    // start another openNode
    double lowestCost = __DBL_MAX__;
    int curID = 0;
    bool picked = false;
    
    
    int i = 1;
    while (picked == false && i < nodes->size()) {
        if((*nodes)[i].open){
            lowestCost = (*nodes)[i].fcost;
            curID = i;
            picked = true;
        }else{
            // if nothing is open then we return the lowest hcost node
            if((*nodes)[i].hcost < lowestCost){
                lowestCost = (*nodes)[i].hcost;
                curID = i;
            }
            i++;
        }
    }
    
    if(!picked){
        
        return curID;
    }
    
    for (int i = 0; i<(*nodes).size(); i++) {
        if((*nodes)[i].fcost < lowestCost && (*nodes)[i].open){
            lowestCost = (*nodes)[i].fcost;
            curID = i;
        }
    }
    
    
    
    return openNodeAi(curID, go, nodes, x, y);
    
}

void eManager::addSeenEnt(int x, int y){
    
    for (int i = 0; i < ents.size(); i++) {
        entity*e = ents[i];
        if(e->getX()+e->getXMove() == x && e->getY()+e->getYMove() == y){
            e->setSee(true);
            return;
        }
    }
}

void eManager::setVision(int x, int y){
    rm->setVision(x, y, this);
}



entity*eManager::checkArea(int x, int y){
    for (int i = 0; i < ents.size(); i++) {
        if(ents[i]->getX()==x&&ents[i]->getY()==y){
            //if on the x and y then we return it
            return ents[i];
        }
    }
    return nullptr;
    
}

bool eManager::moveIn(uint x, uint y){
    
    //can i move into this square x,y
    return !rm->isSolid(x, y);
    

}


sf::View*eManager::getView(){
    return rm->getView();
}
sf::Vector2f eManager::getMousePos(sf::RenderWindow *window){
    return window->mapPixelToCoords(sf::Mouse::getPosition(*window));
}

void eManager::generateMap(sf::RenderWindow * window){
    
    double xOffset = rm->renderMiniMap(window);
    for (int i = 0; i < ents.size(); i++ ) {
        
        if(ents[i]->getSee()){
            
            dots[ents[i]->getGoodGuy()].setPosition(window->mapPixelToCoords(sf::Vector2i((ents[i]->getX()+1)*rm->getMapPixelSize(), (ents[i]->getY()-1)*rm->getMapPixelSize()+xOffset)));
            window->draw(dots[ents[i]->getGoodGuy()]);
        }
        
    }
    
}

void eManager::sHasEnt(int x, int y,bool t){
    rm->sHasEnt(x, y, t);
}

bool eManager::gHasEnt(int x, int y){
    rm->gHasEnt(x, y);
}

void eManager::clearEnts(){
    for (int i =0; i <ents.size(); i++) {
        ents[i]->setSee(false);
    }
}
entity*eManager::getEnt(int x, int y){
    for (int i = 0; i < ents.size(); i++) {
        if(ents[i]->getX()+ents[i]->getXMove()==x&&ents[i]->getY()+ents[i]->getYMove()==y){
            return ents[i];
        }
    }
    return nullptr;
}
entity*eManager::getEnt(int id){
    return ents[id];
}
entity * eManager::getIntersect(int ix, int iy, int ex, int ey) {
    
    for (int i = 0; i < ents.size(); i++) {
        if(ents[i]->checkInterSect(ix, iy, ex, ey)){
            return ents[i];
        }
    }
    return nullptr;
}


//void eManager::addEnemy(enemy * e){
    //enes.push_back(e);
    //ents.push_back(e);
//}


