//
//  weapon_type.hpp
//  DR
//
//  Created by Alan Yuan on 2017-09-16.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#ifndef weapon_type_hpp
#define weapon_type_hpp

#include <stdio.h>
#include <Weapon.hpp>



class Knife:public weapon {
    
    
public:
    Knife():weapon(1,0,1,50,1,30){
        
    }
};

class NONE:public weapon {
    
    
public:
    NONE():weapon(0,0,0,50,1,30){
        
    }
};
#endif /* weapon_type_hpp */
