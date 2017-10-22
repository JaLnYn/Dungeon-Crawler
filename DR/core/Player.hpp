//
//  Player.hpp
//  DR
//
//  Created by Alan Yuan on 2017-04-28.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Player.hpp"
#include <vector>
#include "eManager.hpp"
#include <SFML/Graphics.hpp>
#include "hero.hpp"
#include <math.h>
#include <vector>
#include <WeaponManager.hpp>


class Player{
private:
    struct Group{
        std::vector<hero*> contains;
        void add(hero*h){
            bool add = true;
            for (int i = 0; i < contains.size(); i++) {
                if(contains[i]==h){
                    add = false;
                    i+=100;
                }
            }
            if(add){
                contains.push_back(h);
            }
        }
        void removeAll(){
            contains.clear();
        }
        hero* get(int id){
            return contains[id];
        }
        int getSize(){
            return contains.size();
        }
    };
    
    eManager * eman;
    int mouseX = 100;
    int mouseY = 100;
    int goToX = 0;
    int goToY = 0;
    int initX;
    int initY;
    
    int startX;
    int startY;
    int endX;
    int endY;
    
    bool haveBeenClicked = false;
    bool cmdClick = false;
    std::vector<hero*> h;
    int guiStart;
    int mapGuiEnd;
    const int scrollSpd = 30;
    const int scrollSense = 5;
    std::vector<hero*> selected;
    Group groups[10];
    int cmd = false;
    int amountOfPotions = 5;
    int previousSelectNumber = 0;
    sf::Texture guiTexture;
    sf::Sprite mapsprite;
    sf::Sprite feildSprite;
    sf::Sprite selectSprite;
    sf::Sprite selectSpriteInner;
    sf::RectangleShape potionBar;
    weaponManager wepMan;
    
    sf::RectangleShape backGround;
    
    //list of weapons
    
    std::vector<weapon> wea_list;
    
    sf::RectangleShape inv_background;
    
    sf::RectangleShape inv_select;
    sf::RectangleShape wep_select;
    
public:
    
    
    bool potionClicked = false;
    int maxPotions = 5;
    
    
    
    Player(eManager*eman, hero*h1, hero*h2,hero*h3,hero*h4);
    void setVision();
    void addToSelected(hero*e);
    void init();
    void tick(sf::RenderWindow*);
    void renderGui(sf::RenderWindow*);
    void SetGuiStart();
    int getSize();
    void removeH(entity*h);
    void lvUp();
    void renderGroundGui(sf::RenderWindow*);
    void addToInv( int id);
    void plusPotion();
    bool keyDown = false;
    
    entity * getHero(int i);
    
    weapon inventory[5] = {NONE(),NONE(),NONE(),NONE(),NONE()};
    const int inventoryMaxSpace = 5;
    
    int cur_sel_inv = 0;
    int cur_sel_equ = 0;
    bool arrowDown = false;
    
    sf::Sprite potionSprite;
};
#endif /* Player_hpp */
