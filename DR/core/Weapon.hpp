//
//  Weapon.hpp
//  DR
//
//  Created by Alan Yuan on 2017-09-16.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#ifndef Weapon_hpp
#define Weapon_hpp


#include <stdio.h>
class hero;
class entity;

class weapon{
public:
    weapon(int IMGX,int IMGY,int id,int attackReset, int attackDamage,int totalDur);
    void attack(entity*, int hsPercent);

    int getATK();
    int getTime();
    int getDur();
    int getCurDur();
    int getRange();
    int getAmmo();
    int getID();
    int getIMGX();
    int getIMGY();
    
    
private:
    int attackResetTime;
    int imgX, imgY;
    int attackDamage;
    int durability;
    int current_durability;
    int range;
    int id;
    
    
    
};

#endif /* Weapon_hpp */
