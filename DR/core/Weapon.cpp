//
//  Weapon.cpp
//  DR
//
//  Created by Alan Yuan on 2017-09-16.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#include "Weapon.hpp"
#include <ResourcePath.hpp>
weapon::weapon(int imgX,int imgY,int id,int attackReset, int attackDamage,int totalDur){
    this->id = id;
    attackResetTime = attackReset;
    this->attackDamage = attackDamage;
    durability = totalDur;
    this->imgX = imgX;
    this->imgY = imgY;
}



int weapon::getTime(){
    return attackResetTime;
}

int weapon::getATK(){
    return attackDamage;
}

int weapon::getDur(){
    return durability;
}
int weapon::getCurDur(){
    return current_durability;
}

int weapon::getID(){
    return id;
}
int weapon::getIMGX(){
    return imgX;
}
int weapon::getIMGY(){
    return imgX;
}

