//
//  eManager.hpp
//  DR
//
//  Created by Alan Yuan on 2017-01-13.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//


#ifndef eManager_hpp
#define eManager_hpp

#include <stdio.h>

#include <vector>
#include <SFML/Graphics.hpp>

class entity;
class hero;
class room;
class Player;
class enemy;
class ene_boss;

class Line {
private:
    std::vector<sf::Vector2i> coord;
public:
    void addCoord(int x, int y){
        coord.push_back(sf::Vector2i(x,y));
    }
    sf::Vector2i getCoord(int i){
        return coord[i];
    }
    int getSize(){
        return coord.size();
    }
};


class eManager{
public:
    

    struct node{
        int x = -1;
        int y = -1;
        
        double fcost; //length of the shortest path h + g
        double gcost; //from start to here
        double hcost; //from here to finish
        
        bool fNode = false;
        bool open;
        int parent;
    };
    //sorts the order in which entities are rendered
    struct sorter;
    
    
    
    void setRoom(room*);
    
    /**
     Purpose:
     to add an entity to the vector that holds all the entities
     Parameter:
     an entity to add
     Return:
     void
     **/
    void addEnt(entity*);
    
    void removeEnt(entity*);
    /**
    Purpose:
     to initiate eManager
    Parameter:
     set the room of the eManager
    Return:
     void
    **/
    void init(room*,  sf::RenderWindow*window);
    
    /**
    Purpose:
     to render the entities and everything under it examples: gui on the ground
    Parameter:
     a window to render on
    Return:
     void
    **/
    void render(sf::RenderWindow*,sf::View*cam);
    
    /**
     Purpose:
        to allow all the entities to calcuate and make desisions and let the player give commands
     Parameter:
        window to get mouse and keyboard input from
     Return:
        void
     **/
    void tick(sf::RenderWindow*);
    
    /**
     Purpose:
        to go to the next turn and allow the next entity to tick
     Parameter:
        none
     Return:
        none
     **/
    void nextEnt();
    
    /**
     Purpose:
        check whether there is a entity in the square given
     Parameter: 
        (int,int) the x and y to check
     Return:
        &of the entity that is on the spot. If there is nothing nullptr will be returned
     **/
    entity * checkArea(int, int);
    
    /**
     Purpose:
     To get the width
     Parameter:
     none
     Return:
     return the width
     **/
    int getTileSizeX();
    
    /**
     Purpose:
     To get the length
     Parameter:
     none
     Return:
     return the length
     **/
    int getTileSizeY();
    
    /**
     Purpose:
        get the view that is in the rm adress
     Parameter:
        none
     Return:
        the adress of the view from the rm variable
     **/
    sf::View*getView();
    
    /**
     Purpose:
        to set vision based on the x and y position
     Parameter:
        x and y to start the calculations
     Return:
        void
     **/
    void setVision(int, int);
    
    /**
     Purpose:
        to check whether there is a entity on a certain area and add it to the seen ent vector
     Parameter:
        the x and y to check
     Return:
        void
     **/
    void addSeenEnt(int, int);
    
    
    //pathFinding (find the next move based on the poseition of the current ent and the place it wants to go)
    // returns the xMove and yMove;
    
    /**
     Purpose:
        to find the path from a entities x and y to go to a certain x and y
     Parameter:
        a entity and a place to go to
     Return:
        a vector of nodes
     **/    
    std::vector<node> pathFind(entity*,sf::Vector2i go);
    
    /**
     Purpose: 
        opens and lables all surrounding nodes, closes the current one and returns the next closest node
     Parameter:
        the id of the node to open, the final destination and the adress of the node aray
     Return:
        returns an id of a node
     **/
    int openNode (int id, sf::Vector2i go,std::vector<node>*nodes);
    
    int openNodeFirst (int id,entity* ent, sf::Vector2i go,std::vector<node>*nodes);
    
    std::vector<node> pathFindAi(entity*,sf::Vector2i go);
    
    /**
     Purpose:
     opens and lables all surrounding nodes, closes the current one and returns the next closest node
     Parameter:
     the id of the node to open, the final destination and the adress of the node aray
     Return:
     returns an id of a node
     **/
    int openNodeAi (int id, sf::Vector2i go,std::vector<node>*nodes, int x, int y);
    
    int openNodeFirstAi (int id, sf::Vector2i go,std::vector<node>*nodes,int x, int y);
    
    //TOOLS FOR ENTITIES
    // under this line is room and collision checking
    
    /**
     Purpose:
        check if a entity can move into a certain area
     Parameter:
        a x and y to check
     Return:
        whether or not something can move in
     **/
    virtual bool moveIn(uint x,uint y);
    
    /**
     Purpose:
        get the position of the mouse as a vector
     Parameter:
        window to get the mouse from
     Return:
        the mosue as a vector
     **/
    sf::Vector2f getMousePos(sf::RenderWindow*window);
    
    /**
     Purpose:
        return the render x offset
     Parameter:
        none
     Return:
        the render x offset
     **/
    int renderX(){
        return getTileSizeX()/2;
    }
    
    /**
     Purpose:
        return the render y offset
     Parameter:
        none
     Return:
        the render y offset
     **/
    int renderY(){
        return getTileSizeY()/2;
    }
    
    /**
     Purpose: get the room's adress
     Parameter: none
     Return: adress of the room
     **/
    room*getRoom(){
        return rm;
    }
    
    /**
     purpose: genorate and render the minimap for the player
     parameters: none
     return: none
     **/
    void generateMap(sf::RenderWindow * window);
    
    bool lineCollision(Line line);
    
    //void addEnemy(enemy* e);
    
    entity*getEnt(int x, int y);
    entity*getEnt(int id);
    
    Player*getPlr(){
        return plr;
    }
    
    void gainPotion();
    void addppl();
    void sHasEnt(int x, int y,bool t);
    bool gHasEnt(int x, int y);
    
    void clearEnts();
    
    entity*getIntersect(int ix, int ex, int iy, int ey);
    int getAmoutOfEnt();
    int getCurrentXp(){
        return plrXP;
    }
    int getMonsterLv(){
        return plrLv;
    }
    int getMonsterXp(){
        return monsterXp;
    }
    int getCurrentLv(){
        return monsterLv;
    }
    void addXp(){
        plrXP+=300;
        if(plrXP>=xpPerLv){
            plrXP = 0;
            plrLv++;
        }
    }
    void addTooBG(enemy*e);
private:
    
    
    //std::vector<node*> nodes;
    sf::RectangleShape tileShader;
    room*rm;
    std::vector<entity*> ents;
    std::vector<entity*> badGuys;
    
    
    int plrLv = 0;
    int plrXP = 0;
    
    int monsterLv = 10;
    int monsterXp = 0;
    
    
    const int xpPerLv = 3000;
    
    int turn = 0;
    short phase = 0;
    
    ene_boss*bossMan;
    std::vector<hero*> heros;
    
    
    Player*plr;
    sf::RectangleShape dots[3];

};

#endif /* eManager_hpp */
