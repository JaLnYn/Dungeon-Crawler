//
//  Rooms.hpp
//  Dungeons
//
//  Created by Alan Yuan on 2017-01-11.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#ifndef Rooms_hpp
#define Rooms_hpp
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include <vector>
#include "eManager.hpp"
#include <algorithm>
#include <math.h>

typedef unsigned int uint;

static const int multipliers[4][8] = {
    {1, 0, 0, -1, -1, 0, 0, 1},
    {0, 1, -1, 0, 0, -1, 1, 0},
    {0, 1, 1, 0, 0, -1, -1, 0},
    {1, 0, 0, 1, -1, 0, 0, -1}
};

class room{
public:
    sf::String textureDIR = "brick_tiles_1.png";
    sf::Texture spriteSheet;
    sf::Sprite sprite;
    std::vector<sf::Vector2i> camps;
    room(sf::View*view){
        cam = view;
        
        
        yOffSet = sf::VideoMode::getDesktopMode().height-roomSize*mapPixelSize;
        
        spriteSheet.loadFromFile(resourcePath()+textureDIR);
        
        sprite.setTexture(spriteSheet);
        double scaleX = (double)renderSizeX/tileSize;
        double scaleY = (double)renderSizeY/tileSize;
        sprite.setScale(scaleX, scaleY);
        
        rects[0].setSize(sf::Vector2f(mapPixelSize,mapPixelSize));
        rects[1].setSize(sf::Vector2f(mapPixelSize,mapPixelSize));
        rects[0].setFillColor(sf::Color(191,191,191,100));
        rects[1].setFillColor(sf::Color(250,222,173,100));
        
        rects[2] = rects[0];
        rects[3] = rects[1];
        rects[4] = rects[1];
        rects[5] = rects[1];
        
        for (int y = 0;  y < getRoomSize(); y++) {
            for (int x = 0; x < getRoomSize(); x++) {
                Room[x][y] = new tile(x,y,0);
            }
        }
        squareForGround.setFillColor(sf::Color::Black);
        squareBackGround.setFillColor(sf::Color::White);
        squareForGround.setSize(sf::Vector2f(64*3.5,64*3.5));
        squareBackGround.setSize(sf::Vector2f(64*3.5+5,64*3.5 +2));

    }
    
    void genorate(int seed);
    void render(sf::RenderWindow*);
    int getArea(int x,int y);
    sf::View*getView();
    int getRenderSizeX();
    int getRenderSizeY();
    void setVision(int, int, eManager*eman);
    
    
    
    
    void setVisible(uint x, uint y,uint startX,uint startY, eManager*eman) {
        // Set the visibility of the cell at the given position.
        int distanceFromOrigin = sqrt((startX - x)*(startX - x) + (startY - y)*(startY - y));
        eman->addSeenEnt(x, y);
        if(Room[x][y]->see < visionRange-distanceFromOrigin){
            Room[x][y]->see = visionRange - distanceFromOrigin;
        }
        
        Room[x][y]->seen = true;
    }
    
    /**
    sf::Vector2f toISO(sf::Vector2i pos){
        sf::Vector2f newPos;
        newPos.x = (2.0*pos.y+pos.x)/2.0;
        newPos.y = (2.0*pos.y-pos.x)/2.0;
        return newPos;
    }
    **/
    
    uint getWidth() const {
        // Return the width of the map.
        return roomSize;
    }
    
    uint getHeight() const {
        // Return the height of the map.
        return roomSize;
    }
    
    bool isSolid(uint x, uint y) {
        if(x < roomSize && y < roomSize){
            if (getArea(x, y) == 0||getArea(x, y) == 2) {
                return true; // is solid
            }
        }else{
            return true;
        }
        return false;
    }
    
    
    void castLight(uint x, uint y, uint radius, uint row,
                   float startSlope, float end_slope,
                   uint xx, uint xy, uint yx,
                   uint yy, eManager*eman) {
        if (startSlope < end_slope) {
            return;
        }
        float nextSlope = startSlope;
        for (uint i = row; i <= radius; i++) {
            bool blocked = false;
            for (int dx = -i, dy = -i; dx <= 0; dx++) {
                float lSlope = (dx - 0.5) / (dy + 0.5);
                float rSlope = (dx + 0.5) / (dy - 0.5);
                if (startSlope < rSlope) {
                    continue;
                } else if (end_slope > lSlope) {
                    break;
                }
                
                int sax = dx * xx + dy * xy;
                int say = dx * yx + dy * yy;
                if ((sax < 0 && (uint)std::abs(sax) > x) ||
                    (say < 0 && (uint)std::abs(say) > y)) {
                    continue;
                }
                uint ax = x + sax;
                uint ay = y + say;
                if (ax >= getWidth() || ay >= getHeight()) {
                    continue;
                }
                
                uint radius2 = radius * radius;
                if ((uint)(dx * dx + dy * dy) < radius2) {
                    setVisible(ax, ay,x,y, eman);
                }
                
                if (blocked) {
                    if (isSolid(ax, ay)) {
                        nextSlope = rSlope;
                        continue;
                    } else {
                        blocked = false;
                        startSlope = nextSlope;
                    }
                } else if (isSolid(ax, ay)) {
                    blocked = true;
                    nextSlope = rSlope;
                    castLight(x, y, radius, i + 1, startSlope, lSlope, xx,
                               xy, yx, yy,eman);
                }
            }
            if (blocked) {
                break;
            }
        }
    }
    
    int getSeeableTile(int x, int y){
        
            return Room[x][y]->see;
        
        
    }
    
    bool getSeenTile(int x, int y){
        return Room[x][y]->seen;
    }
    
    double renderMiniMap(sf::RenderWindow * window){
        
        squareForGround.setPosition(window->mapPixelToCoords(sf::Vector2i(mapPixelSize, -mapPixelSize+yOffSet)));
        squareBackGround.setPosition(window->mapPixelToCoords(sf::Vector2i(0, -mapPixelSize*2+yOffSet)));
        window->draw(squareBackGround);
        window->draw(squareForGround);
        for (int y = 0; y < roomSize; y++) {
            for (int x = 0; x < roomSize; x++) {
                if (Room[x][y]->seen) {
                    rects[Room[x][y]->id].setPosition(window->mapPixelToCoords(sf::Vector2i(x*mapPixelSize+mapPixelSize, y*mapPixelSize+yOffSet-mapPixelSize)));
                    window->draw(rects[Room[x][y]->id]);
                }
            }
        }
        
        return yOffSet;
    }
    
    void clearVision(){
        for (int x = 0; x < roomSize; x++) {
            for (int y = 0; y < roomSize; y++) {
                Room[x][y]->see = false;
            }
        }
    }
    
    //getters and setters
    void sHasEnt(int x, int y,bool t){
        Room[x][y]->hasEnt = t;
    }
    
    bool gHasEnt(int x, int y){
        return Room[x][y]->hasEnt;
    }
    int getSpwnX() { return this->spwnX; }
    int getSpwnY() { return this->spwnY; }
    int getEndX() { return this->endX; }
    int getEndY() { return this->endY; }

    int getExitX() { return this->exitX; }
   
    int getExitY() { return this->exitY; }
    int getRoomSize() { return this->roomSize; }
    int getMapPixelSize() { return this->mapPixelSize; }
    
    int getCampSize(){
        return camps.size();
    }
    
    sf::Vector2i getCampLoc(int i){
        return camps[i];
    }
    
    ~room(){
        for (int x = 0; x < roomSize; x++) {
            for (int y= 0 ; y < roomSize; y++) {
                delete Room[x][y];
            }
        }
    }
    double getYOFF(){
        return yOffSet;
    }
    
private:
    uint visionRange = 7;
    
    class tile{
    public:
        tile(int x, int y, int id){
            this->x = x;
            this->y = y;
            this->id = id;
            see = 0; //0-10 0 = not vis 10 = fully vis
            seen = false;
            hasEnt = false;
        }
        int x;
        int y;
        int see;
        bool seen;
        bool hasEnt;
        int id;
        virtual void render(sf::RenderWindow*window, room*Room){
            
            if(seen){
                
                Room->sprite.setTextureRect(sf::IntRect(Room->tileSize * id, 0, Room->tileSize,Room->tileSize));
                Room->sprite.setPosition(x*Room->renderSizeX, y*Room->renderSizeY);
                window->draw(Room->sprite);
                
            }
        }
    };
    
    
    class wallTile:public tile{
        
    public:
        wallTile(int x, int y):room::tile(x,y,2){
            
        }
        virtual void render(sf::RenderWindow*window, room*Room){
            
            if(Room->getSeenTile(x, y + 1)){
                if(seen){
                    
                    Room->sprite.setTextureRect(sf::IntRect(Room->tileSize * 2, 0, Room->tileSize,Room->tileSize));
                    Room->sprite.setPosition(x*Room->renderSizeX, y*Room->renderSizeY);
                    window->draw(Room->sprite);
                    
                }
            }else{
                if(seen){
                    
                    Room->sprite.setTextureRect(sf::IntRect(0, 0, Room->tileSize,Room->tileSize));
                    Room->sprite.setPosition(x*Room->renderSizeX, y*Room->renderSizeY);
                    window->draw(Room->sprite);
                    
                }
            }
        }
    };
    
    
    
    const int tileSize = 64;
    const int renderSizeX = 64;
    const int renderSizeY = 56;
    
    tile*Room[64][64];
    const int roomSize = 64;
    const int min_leaf_size = 5;
    const int max_offset = 2;
    int spwnX = 0;
    int spwnY = 0;
    int endX = 0;
    int endY = 0;
    int exitX = 0;
    int exitY = 0;
    const int mapPixelSize = 6;
    double yOffSet;
    sf::RectangleShape squareBackGround;
    sf::RectangleShape squareForGround;
    const sf::Vector2i goToTiles[8];
    //std::vector<sf::Vector2i> seeableTiles;
    
    
    sf::RectangleShape rects[6];
    sf::View*cam;
};



#endif /* Rooms_hpp */
