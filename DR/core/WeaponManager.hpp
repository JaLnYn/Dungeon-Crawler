//
//  WeaponManager.hpp
//  DR
//
//  Created by Alan Yuan on 2017-10-01.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#ifndef WeaponManager_hpp
#define WeaponManager_hpp
#include <weapon_type.hpp>
#include <SFML/Graphics.hpp>
#include <ResourcePath.hpp>

#include <stdio.h>
class weaponManager{
public:
    
    weaponManager(){
        spriteSheet.loadFromFile(resourcePath()+"weapons.jpg");
        sprite.setTexture(spriteSheet);
        
        
    }
    
    void renderWep(sf::RenderWindow*window, int imgX,int imgY, float x, float y){
        sprite.setTextureRect(sf::IntRect(imgX*picsize,imgY*picsize,picsize,picsize));
        sprite.setPosition(window->mapPixelToCoords(sf::Vector2i(x, y)));
        window->draw(sprite);
    }

    
    bool compare(weapon w1, weapon w2){
        return w1.getID() == w2.getID();
    }
    
    weapon getWep(int id){
        
    }
    
private:
    weapon weps[2] = {NONE(),Knife()};
    sf::Texture spriteSheet;
    sf::Sprite sprite;
    int spriteXsize = 9;
    int spriteYsize = 6;
    int picsize = 32;
    
};
#endif /* WeaponManager_hpp */
