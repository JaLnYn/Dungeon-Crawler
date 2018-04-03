//
//  objects.hpp
//  spaceCruiser
//
//  Created by Alan Yuan on 2016-12-01.
//  Copyright © 2016 Alan Yuan. All rights reserved.
//

#ifndef objects_hpp
#define objects_hpp


#include "ResourcePath.hpp"
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "eManager.hpp"
#include <iostream>

class object{
public:
    
    object(sf::String path, int x, int y, int team, int bodySizeX,int bodySizeY){
        goodGuy = team;
        hp = HP_PT;
        spriteSheet.loadFromFile(resourcePath()+path);
        this->x = x;
        this->y = y;
        sprite.setTexture(spriteSheet);
        double scale = (double)renderSize/picSize;
        sprite.setScale(scale, scale);
        
        sprite.setTextureRect(sf::IntRect(0,0,picSize,picSize));
        
        hpBackGround.setFillColor(sf::Color::White);
        hpBar.setSize(sf::Vector2f(hpBarSizeX,hpBarSizeY));
        
        hpBlackGround.setSize(sf::Vector2f(hpBarSizeX,hpBarSizeY));
        hpBlackGround.setFillColor(sf::Color::Black);
        hpBackGround.setSize(sf::Vector2f(hpBarSizeX+2,hpBarSizeY+2));
        
        if(goodGuy==1){
            hpBar.setFillColor(sf::Color::Green);
        }else{
            hpBar.setFillColor(sf::Color::Red);
        }
        
        bodyHitBox.setOutlineColor(sf::Color::Green);
        bodyHitBox.setFillColor(sf::Color(255,255,255,150));
        bodyHitBox.setSize(sf::Vector2f(bodySizeX, bodySizeY));
        bodyHitBox.setOrigin(bodySizeX/2, bodySizeY);
        
        
        sprite.setOrigin(picSize/2, (float)picSize*3/4);
        bodyHitBox.setOrigin(bodySizeX/2, (float)bodySizeY*3/4);
    }
    
    int getX(){
        return x;
    }
    
    int getY(){
        return y;
    }
    
    // drawing onto the display
    
    virtual void render(sf::RenderWindow*window){
        
        hpBar.setSize(sf::Vector2f(hpBarSizeX*hp/HP_PT,hpBarSizeY));
        //hpBar
        hpBackGround.setPosition(x*eman->getTileSizeX()+eman->getTileSizeX()/2+xOffSet-hpBarSizeX/2-1, y*eman->getTileSizeY()+eman->getTileSizeY()/2+yOffSet+3-1+barOffSet);
        hpBlackGround.setPosition(x*eman->getTileSizeX()+eman->getTileSizeX()/2+xOffSet-hpBarSizeX/2, y*eman->getTileSizeY()+eman->getTileSizeY()/2+yOffSet+3+barOffSet);
        hpBar.setPosition(x*eman->getTileSizeX()+eman->getTileSizeX()/2+xOffSet-hpBarSizeX/2, y*eman->getTileSizeY()+eman->getTileSizeY()/2+yOffSet+3+barOffSet);
        
        
        
        sprite.setPosition(x*eman->getTileSizeX()+eman->getTileSizeX()/2+xOffSet, y*eman->getTileSizeY()+eman->getTileSizeX()/2+yOffSet);
        
        
        
        bodyHitBox.setPosition(x*eman->getTileSizeX()+eman->getTileSizeX()/2+xOffSet, y*eman->getTileSizeY()+eman->getTileSizeY()/2+yOffSet);
        window->draw(sprite);
        
        

        window->draw(hpBackGround);
        
        window->draw(hpBlackGround);
        window->draw(hpBar);
        
        
        window->draw(bodyHitBox);
        
        
    }
    
    double abs(double i){
        if (i>0) {
            return i;
        }else{
            return -i;
        }
    }
    
    double xOff(){
        return xOffSet;
    }
    double yOff(){
        return yOffSet;
    }
    void setSee(bool see){
        this->see = see;
        
    }
    
    bool getSee(){
        return this->see;
    }
    
    
    
    bool checkInterSect(int ix, int iy, int ex, int ey){
        int xStart = x*eman->getTileSizeX() + eman->getTileSizeX()/2 - bodyHitBox.getSize().x/2 + xOffSet;
        int xEnd = x*eman->getTileSizeX() + eman->getTileSizeX()/2 + bodyHitBox.getSize().x/2 + xOffSet;
        
        int yStart = y*eman->getTileSizeY() + eman->getTileSizeY()/2 - bodyHitBox.getSize().y + yOffSet;
        int yEnd = y*eman->getTileSizeY() + eman->getTileSizeY()/2 + yOffSet;
        
        if(xStart < ex && xEnd > ix){
            if(yStart < ey && yEnd > iy){
                return true;
            }
        }
        return false;
    }
    
    bool checkPointInterSect(int nx, int ny){
        int xStart = x*eman->getTileSizeX() + eman->getTileSizeX()/2 - bodyHitBox.getSize().x/2 + xOffSet;
        int xEnd = x*eman->getTileSizeX() + eman->getTileSizeX()/2 + bodyHitBox.getSize().x/2 + xOffSet;
        
        int yStart = y*eman->getTileSizeY() + eman->getTileSizeY()/2 - bodyHitBox.getSize().y + yOffSet;
        int yEnd = y*eman->getTileSizeY() + eman->getTileSizeY()/2 + yOffSet;
        
        if(xStart < nx && xEnd > nx){
            if(yStart < ny && yEnd > ny){
                
                return true;
            }
        }
        return false;
    }
    
    int getRenderSize(){
        return renderSize;
    }
protected:
    int x = 1;
    int y = 1;
    const int picSize = 32;
    const int renderSize = 64;
    
    const int picOffsetX = 16;
    const int picOffsetY = 16;
    int hpBarSizeX = 32;
    int hpBarSizeY = 5;
    
    sf::RectangleShape hpBlackGround;
    sf::RectangleShape hpBackGround;
    sf::RectangleShape hpBar;
    int goodGuy;
    // 0 is nutral
    // 1 is good
    // 2 is bad
    
    //animation
    sf::Texture spriteSheet;
    sf::Sprite sprite;
    
    
    eManager*eman;
    int barOffSet = 10;
    int headOffSet = 0;
    double xOffSet = 0;
    double yOffSet = 0;
    int phase = 0;
    bool see = 0;
    bool dead = false;
    // health mx health is /2
    // Sheilds mx sheild is /3
    int HP_PT = 200;
    int hp = 200;
    // stanima is your erg
    int ERG_PT = 6;
    int erg = 6;
    // atk is the dmg you do
    int atk = 3;
    // xMove and yMove
    int xMove = 0;
    int yMove = 0;
    
    
    sf::RectangleShape bodyHitBox;
    
private:
    
        
};

#endif /* objects_hpp */
