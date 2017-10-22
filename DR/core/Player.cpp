//
//  Player.cpp
//  DR
//
//  Created by Alan Yuan on 2017-04-28.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//

#include "Player.hpp"

Player::Player(eManager*eman,hero*h1,hero*h2,hero*h3,hero*h4){
    h.push_back(h1);h.push_back(h2);h.push_back(h3);h.push_back(h4);
    
    this->eman = eman;
    guiTexture.loadFromFile(resourcePath()+"gui.png");
    selectSprite.setTexture(guiTexture);
    selectSpriteInner.setTexture(guiTexture);
    potionSprite.setTexture(guiTexture);
    feildSprite.setTexture(guiTexture);
    mapsprite.setTexture(guiTexture);
    backGround.setFillColor(sf::Color::Black);
    potionBar.setFillColor(sf::Color::Red);
    potionBar.setSize(sf::Vector2f(4,8));
    backGround.setSize(sf::Vector2f(32,10));
    inv_select.setFillColor(sf::Color(255,255,255,255));
    wep_select.setFillColor(sf::Color(255,255,255,255));
    
    inv_select.setSize(sf::Vector2f(42,44));
    wep_select.setSize(sf::Vector2f(44,42));
    
    
    
}

void Player::addToInv(int id){
    for (int i = 0; i <inventoryMaxSpace; i++) {
        if(wepMan.compare(inventory[i], NONE())){
            inventory[i] = wepMan.getWep(id);
        }
    }
}

void Player::addToSelected(hero*e){
    bool add = true;
    for (int i = 0; i < selected.size(); i++) {
        if(selected[i]==e){
            add = false;
            i+=100;
        }
    }
    if(add){
        selected.push_back(e);
    }
    
}
void Player::init(){
    initX=0;
    initY=0;
    
    int mapSpriteSize = 64;
    mapsprite.setTextureRect(sf::IntRect(0,0,64,64));
    float mapTextureScale = (eman->getRoom()->getRoomSize()*eman->getRoom()->getMapPixelSize()+2)/(float)mapSpriteSize;
    mapsprite.setScale(mapTextureScale, mapTextureScale);
    
    potionSprite.setTextureRect(sf::IntRect(64,32,32,32));
    
    
    selectSprite.setTextureRect(sf::IntRect(64+32,32,32,32));
    feildSprite.setTextureRect(sf::IntRect(64,0,32,32));
    selectSpriteInner.setTextureRect(sf::IntRect(96,0,32,32));
    
    feildSprite.setScale((float)eman->getTileSizeX()/32,(float)eman->getTileSizeY()/32);
    
}

void Player::tick(sf::RenderWindow*window){
    
    sf::View * view = eman->getView();
    
    mouseX = eman->getMousePos(window).x;
    mouseY = eman->getMousePos(window).y;
    
    //wepSelect
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        if(!arrowDown){
            arrowDown = true;
            if(cur_sel_equ+1<h.size()){
                cur_sel_equ++;
            }
        }
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        if(!arrowDown){
            arrowDown = true;
            if(cur_sel_equ>0){
                cur_sel_equ--;
            }
        }
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        if(!arrowDown){
            arrowDown = true;
            if(cur_sel_inv>0){
                cur_sel_inv--;
            }
        }
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        if(!arrowDown){
            arrowDown = true;
            if(cur_sel_inv<inventoryMaxSpace){
                cur_sel_inv++;
            }
        }
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
        if(!arrowDown){
            arrowDown = true;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
                inventory[cur_sel_inv] = NONE();
            }else{
                weapon wep = inventory[cur_sel_inv];
                inventory[cur_sel_inv] = *(h[cur_sel_equ]->getCurrentWeapon());
                h[cur_sel_equ]->setWeapon(wep);
                
            }
        }
        
    }else{
        arrowDown = false;
    }
    //window->mapPixelToCoords(sf::Vector2i(0, eman->getRoom()->getYOFF()-64-32*4-(renderSizeWep+2)*2*i))
    //scrolling
    if(sf::Mouse::getPosition().x > sf::VideoMode::getDesktopMode().width-scrollSense){
        view->move(scrollSpd, 0);
    }else if(sf::Mouse::getPosition().x < scrollSense){
        view->move(-scrollSpd, 0);
    }
    
    if(sf::Mouse::getPosition().y-sf::VideoMode::getDesktopMode().height > sf::VideoMode::getDesktopMode().height-scrollSense){
        
        view->move(0, scrollSpd);
    }else if(sf::Mouse::getPosition().y-sf::VideoMode::getDesktopMode().height < scrollSense){
        view->move(0, -scrollSpd);
    }
    
    
    //control groups
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            groups[0].removeAll();
            for (int i = 0; i < selected.size(); i++) {
                groups[0].add(selected[i]);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            for (int i = 0; i < selected.size(); i++) {
                groups[0].add(selected[i]);
            }
        }else{
            if(previousSelectNumber == 0&&groups[0].getSize()>0){
                if(!keyDown){view->setCenter(selected[0]->getX()*eman->getTileSizeX(), selected[0]->getY()*eman->getTileSizeY());}
            }else{
                selected.clear(); keyDown = true;
                for (int i = 0; i < groups[0].getSize(); i++) {
                    selected.push_back(groups[0].get(i));
                }
                previousSelectNumber = 0;
            }
        }
        
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            groups[1].removeAll();
            for (int i = 0; i < selected.size(); i++) {
                groups[1].add(selected[i]);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            for (int i = 0; i < selected.size(); i++) {
                groups[1].add(selected[i]);
            }
        }else{
            if(previousSelectNumber == 1&&groups[1].getSize()>0){
                if(!keyDown){view->setCenter(selected[0]->getX()*eman->getTileSizeX(), selected[0]->getY()*eman->getTileSizeY());}
            }else{
                selected.clear(); keyDown = true;
                for (int i = 0; i < groups[1].getSize(); i++) {
                    selected.push_back(groups[1].get(i));
                }
                previousSelectNumber = 1;
            }
        }
        
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            groups[2].removeAll();
            for (int i = 0; i < selected.size(); i++) {
                groups[2].add(selected[i]);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            for (int i = 0; i < selected.size(); i++) {
                groups[2].add(selected[i]);
            }
        }else{
            if(previousSelectNumber == 2&&groups[2].getSize()>0){
                if(!keyDown){view->setCenter(selected[0]->getX()*eman->getTileSizeX(), selected[0]->getY()*eman->getTileSizeY());}
            }else{
                selected.clear(); keyDown = true;
                for (int i = 0; i < groups[2].getSize(); i++) {
                    selected.push_back(groups[2].get(i));
                }
                previousSelectNumber = 2;
            }
        }
        

    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            groups[3].removeAll();
            for (int i = 0; i < selected.size(); i++) {
                groups[3].add(selected[i]);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            for (int i = 0; i < selected.size(); i++) {
                groups[3].add(selected[i]);
            }
        }else{
            if(previousSelectNumber == 3&&groups[3].getSize()>0){
                if(!keyDown){view->setCenter(selected[0]->getX()*eman->getTileSizeX(), selected[0]->getY()*eman->getTileSizeY());}
            }else{
                selected.clear(); 
                keyDown = true;
                for (int i = 0; i < groups[3].getSize(); i++) {
                    selected.push_back(groups[3].get(i));
                }
                previousSelectNumber = 3;
            }
        }
        
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            groups[4].removeAll();
            for (int i = 0; i < selected.size(); i++) {
                groups[4].add(selected[i]);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            for (int i = 0; i < selected.size(); i++) {
                groups[4].add(selected[i]);
            }
        }else{
            if(previousSelectNumber == 4&&groups[4].getSize()>0){
                if(!keyDown){view->setCenter(selected[0]->getX()*eman->getTileSizeX(), selected[0]->getY()*eman->getTileSizeY());}
            }else{
                selected.clear(); keyDown = true;
                for (int i = 0; i < groups[4].getSize(); i++) {
                    selected.push_back(groups[4].get(i));
                }
                previousSelectNumber = 4;
            }
        }
        
        
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            groups[5].removeAll();
            for (int i = 0; i < selected.size(); i++) {
                groups[5].add(selected[i]);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            for (int i = 0; i < selected.size(); i++) {
                groups[5].add(selected[i]);
            }
        }else{
            if(previousSelectNumber == 5&&groups[5].getSize()>0){
                if(!keyDown){view->setCenter(selected[0]->getX()*eman->getTileSizeX(), selected[0]->getY()*eman->getTileSizeY());}
            }else{
                selected.clear(); keyDown = true;
                for (int i = 0; i < groups[5].getSize(); i++) {
                    selected.push_back(groups[5].get(i));
                }
                previousSelectNumber = 5;
            }
        }
        

    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            groups[6].removeAll();
            for (int i = 0; i < selected.size(); i++) {
                groups[6].add(selected[i]);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            for (int i = 0; i < selected.size(); i++) {
                groups[6].add(selected[i]);
            }
        }else{
            if(previousSelectNumber == 6&&groups[6].getSize()>0){
                if(!keyDown){view->setCenter(selected[0]->getX()*eman->getTileSizeX(), selected[0]->getY()*eman->getTileSizeY());}
            }else{
                selected.clear(); keyDown = true;
                for (int i = 0; i < groups[6].getSize(); i++) {
                    selected.push_back(groups[6].get(i));
                }
                previousSelectNumber = 6;
            }
        }
        
        

    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            groups[7].removeAll();
            for (int i = 0; i < selected.size(); i++) {
                groups[7].add(selected[i]);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            for (int i = 0; i < selected.size(); i++) {
                groups[7].add(selected[i]);
            }
        }else{
            if(previousSelectNumber == 7&&groups[7].getSize()>0){
                if(!keyDown){view->setCenter(selected[0]->getX()*eman->getTileSizeX(), selected[0]->getY()*eman->getTileSizeY());}
            }else{
                selected.clear(); keyDown = true;
                for (int i = 0; i < groups[7].getSize(); i++) {
                    selected.push_back(groups[7].get(i));
                }
                previousSelectNumber = 7;
            }
        }
        

    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            groups[8].removeAll();
            for (int i = 0; i < selected.size(); i++) {
                groups[8].add(selected[i]);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            for (int i = 0; i < selected.size(); i++) {
                groups[8].add(selected[i]);
            }
        }else{
            if(previousSelectNumber == 8&&groups[8].getSize()>0){
                if(!keyDown){view->setCenter(selected[0]->getX()*eman->getTileSizeX(), selected[0]->getY()*eman->getTileSizeY());}
            }else{
                selected.clear(); keyDown = true;
                for (int i = 0; i < groups[8].getSize(); i++) {
                    selected.push_back(groups[8].get(i));
                }
                previousSelectNumber = 8;
            }
        }
        
        

    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            groups[9].removeAll();
            for (int i = 0; i < selected.size(); i++) {
                groups[9].add(selected[i]);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            for (int i = 0; i < selected.size(); i++) {
                groups[9].add(selected[i]);
            }
        }else{
            if(previousSelectNumber == 9&&groups[9].getSize()>0){
                if(!keyDown){view->setCenter(selected[0]->getX()*eman->getTileSizeX(), selected[0]->getY()*eman->getTileSizeY());}
            }else{
                selected.clear(); keyDown = true;
                for (int i = 0; i < groups[9].getSize(); i++) {
                    selected.push_back(groups[9].get(i));
                }
                previousSelectNumber = 9;
            }
        }
        
        
    }else{
        keyDown = false;
    }
    
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        cmd = 5;
    }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        for (int i = 0; i< selected.size();i++) {
            entity*e = selected[i];
            //&&eman->moveIn(goToX, goToY)
            e->fpath(sf::Vector2i(e->getX(),e->getY()));
            e->initMove();
            e->setCMD(0);
            e->setLock(nullptr);
        }
    }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        //holD
        for (int i = 0; i< selected.size();i++) {
            entity*e = selected[i];
            //&&eman->moveIn(goToX, goToY)
            e->fpath(sf::Vector2i(e->getX(),e->getY()));
            e->initMove();
            e->setCMD(4);
            e->setLock(nullptr);
        }
    }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
        //drink Potion
        if(!potionClicked){
            potionClicked = true;
            for (int i = 0; i< selected.size();i++) {
                hero*e = selected[i];
                //&&eman->moveIn(goToX, goToY)
                if(amountOfPotions>1){
                    amountOfPotions--;
                    e->heal(6);
                }
            }
        }
        
    }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
        //use ability
        for (int i = 0; i< selected.size();i++) {
            hero*e = selected[i];
            //&&eman->moveIn(goToX, goToY)
            e->useAbil();
        }
    }else{
        potionClicked = false;
    }
//    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)){
//        for (int i = 0; i< selected.size();i++) {
//            entity*e = selected[i];
//            //&&eman->moveIn(goToX, goToY)
//            e->fpath(sf::Vector2i(e->getX(),e->getY()));
//            e->initMove();
//            e->setCMD(4);
//            e->setLock(nullptr);
//        }
//    }
    
    if(sf::Mouse::getPosition().y-sf::VideoMode::getDesktopMode().height > guiStart&&sf::Mouse::getPosition().x < mapGuiEnd && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        
        //clicking on the minimap
        view->setCenter((sf::Mouse::getPosition().x)/(double)eman->getRoom()->getMapPixelSize()*eman->getTileSizeX(), (sf::Mouse::getPosition().y-sf::VideoMode::getDesktopMode().height-sf::VideoMode::getDesktopMode().height+mapGuiEnd)/(double)eman->getRoom()->getMapPixelSize()*eman->getTileSizeY());
        
        
    }else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        bool hasSelection = false;
        for (int i = 0; i<eman->getAmoutOfEnt(); i++) {
            entity*e = eman->getEnt(mouseX/eman->getTileSizeX(),mouseY/eman->getTileSizeY());
            
            if(e!=nullptr){
                if(e->getGoodGuy()==2){
                    hasSelection = true;
                    i+=eman->getAmoutOfEnt();
                    for (int i = 0; i< selected.size();i++) {
                        entity*a = selected[i];
                        //&&eman->moveIn(goToX, goToY)
                        a->fpath(sf::Vector2i(mouseX/eman->getTileSizeX(),mouseY/eman->getTileSizeY()));
                        a->initMove();
                        a->setCMD(3);
                        a->setLockOn(e);
                    }
                }
            }
        }
        if(!hasSelection){
            
//            double avX = 0;
//            double avY = 0;
//            
//            for (int i = 0; i < selected.size(); i++) {
//                avX+=selected[i]->getX();
//                avY+=selected[i]->getY();
//            }
//            
//            avX = avX/(selected.size());
//            avY = avY/(selected.size());
//            
            for (int i = 0; i< selected.size();i++) {
                entity*e = selected[i];
                //&&eman->moveIn(goToX, goToY)
                e->fpath(sf::Vector2i(mouseX/eman->getTileSizeX(),mouseY/eman->getTileSizeY()));
                e->initMove();
                e->setCMD(2);
                e->setLockOn(nullptr);
            } 
        }
        
        
    }else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        
        
        if(cmd == 5){
            cmdClick = true;
            //attack
            cmd = 0;
            bool hasSelection = false;
            
            entity*e = eman->getEnt(mouseX/eman->getTileSizeX(),mouseY/eman->getTileSizeY());
            if(e!=nullptr){
                
                hasSelection = true;
                
                for (int i = 0; i< selected.size();i++) {
                    entity*a = selected[i];
                    //&&eman->moveIn(goToX, goToY)
                    
                    a->setCMD(3);
                    a->setLockOn(e);
                }
                
            }
            
            if(!hasSelection){
                
                for (int i = 0; i< selected.size();i++) {
                    entity*e = selected[i];
                    //&&eman->moveIn(goToX, goToY)
                    e->fpath(sf::Vector2i(mouseX/eman->getTileSizeX(),mouseY/eman->getTileSizeY()));
                    e->initMove();
                    e->setCMD(5);
                    
                }
            }
            
            
        }else if(!haveBeenClicked){
            // for selection of units
            haveBeenClicked = true;
            initX = mouseX;
            initY = mouseY;
            
        }
        if (initX<mouseX) {
            startX = initX;
            endX = mouseX;
        }else{
            startX = mouseX;
            endX = initX;
        }
        if (initY<mouseY) {
            startY = initY;
            endY = mouseY;
        }else{
            startY = mouseY;
            endY = initY;
        }
        
        
        
    }else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && haveBeenClicked){
        previousSelectNumber = 10;
        
        if(!cmdClick){
            haveBeenClicked = false;
            
            if(selected.size()>0&&!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
                selected.clear();
            }
            
            //        if (abs(initX - mouseX) < eman->getTileSizeX() && abs(initY - mouseY) < eman->getTileSizeY()) {
            //
            //            // make clicking latter
            //
            //            entity*e = eman->getIntersect(startX, startY, endX, endY);
            //            if (e != nullptr) {
            //                hero * h;
            //                if((h = dynamic_cast<hero * >(e))){
            //                    addToSelected(h);
            //                }
            //
            //            }
            //
            //        }else {
            
            
            for (int y = startY/eman->getTileSizeY(); y < endY/eman->getTileSizeY()+1; y++) {
                for (int x = startX/eman->getTileSizeX(); x < endX/eman->getTileSizeX()+1; x++) {
                    for (int i = 0; i < h.size(); i++) {
                        
                        
                        int entX = ((h[i]->getX())+(h[i]->getXMove()));
                        int entY = ((h[i]->getY())+(h[i]->getYMove()));
                        if(entX==x && entY==y){
                            
                            addToSelected(h[i]);
                            
                        }
                    }
                }
            }
            //}
            
            
        }else{
            cmdClick = false;
            haveBeenClicked = false;
        }
    }
    

}

void Player::renderGui(sf::RenderWindow * window ){
    
    backGround.setPosition(window->mapPixelToCoords(sf::Vector2i(0, eman->getRoom()->getYOFF()-64-32+46)));
    window->draw(backGround);
    for (int i = 0; i < amountOfPotions; i++) {
        potionBar.setPosition(window->mapPixelToCoords(sf::Vector2i(12*i+2, eman->getRoom()->getYOFF()-64-32+60)));
        window->draw(potionBar);
    }
    
    potionSprite.setPosition(window->mapPixelToCoords(sf::Vector2i(0, eman->getRoom()->getYOFF()-64-32)));
    window->draw(potionSprite);
    wep_select.setPosition(window->mapPixelToCoords(sf::Vector2i(0, eman->getRoom()->getYOFF()-64-32*4-(32+2)*2*cur_sel_equ-8)));
    window->draw(wep_select);
    for (int i = 0; i < h.size(); i++) {
        h[i]->renderWepEquip(window, i);
    }
    
    inv_select.setPosition(window->mapPixelToCoords(sf::Vector2i(0, eman->getRoom()->getYOFF()-64-32*(4+inventoryMaxSpace+1)*2-(32+2)*2*cur_sel_inv-8)));
    window->draw(inv_select);
    
    for (int i = 0; i < inventoryMaxSpace; i++) {
        wepMan.renderWep(window, inventory[i].getIMGX(), inventory[i].getIMGY(),0, eman->getRoom()->getYOFF()-64-32*(4+inventoryMaxSpace+1)*2-(32+2)*2*i-8);
    }
    
    for (int i = 0; i < h.size(); i++) {
        wepMan.renderWep(window, h[i]->getCurrentWeapon()->getIMGX(), h[i]->getCurrentWeapon()->getIMGX(),0, eman->getRoom()->getYOFF()-64-32*4-(32+2)*2*i-8);
    }
}

void Player::setVision(){
    eman->clearEnts();
    eman->getRoom()->clearVision();
    for (int i = 0; i<h.size(); i++) {
        if(h[i]!=nullptr){
            eman->setVision((h[i]->getX())+(h[i]->getXMove()),(h[i]->getY())+(h[i]->getYMove()));
        }
    }
    
}

void Player::SetGuiStart(){
    guiStart = sf::VideoMode::getDesktopMode().height-eman->getRoom()->getRoomSize()*eman->getRoom()->getMapPixelSize();
    mapGuiEnd = eman->getRoom()->getRoomSize()*eman->getRoom()->getMapPixelSize();
    
}

int Player::getSize(){
    return h.size();
}

entity * Player::getHero(int i){
    return h[i];
}

void Player::removeH(entity *hero){
    //10 is size of groups
    for (int i = 0; i < 10; i++) {
        for (int i2 = 0; i2 < groups[i].getSize(); i2++) {
            if(groups[i].get(i2)==hero){
                groups[i].removeAll();
            }
        }
    }
    for (int i = 0; i < selected.size(); i++) {
        if(selected[i] == hero){
            selected.erase(selected.begin()+i);
            
        }
        
    }
    for (int i = 0; i< h.size(); i++) {
        if(hero == h[i]){
            h.erase(h.begin()+i);
        }
    }
}
void Player::lvUp(){
    for (int i = 0; i< selected.size();i++) {
        hero*e = selected[i];
        e->lvlUP();
    }
}
void Player::renderGroundGui(sf::RenderWindow*window){
    
    for (int i = 0; i < h.size(); i++) {
        selectSpriteInner.setPosition(h[i]->getX()*eman->getTileSizeX()+h[i]->xOff()+eman->getTileSizeX()/2-16, h[i]->getY()*eman->getTileSizeY()+h[i]->yOff());
        window->draw(selectSpriteInner);
    }
    
    for (int i = 0; i < selected.size(); i++) {
        selectSprite.setPosition(selected[i]->getX()*eman->getTileSizeX()+selected[i]->xOff()+eman->getTileSizeX()/2-16, selected[i]->getY()*eman->getTileSizeY()+selected[i]->yOff());
        window->draw(selectSprite);
    }
    
    
    
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)&&(endX-startX)/eman->getTileSizeX()<15&&(endY-startY)/eman->getTileSizeY()<15){
        
        for (int x = startX/eman->getTileSizeX(); x<endX/eman->getTileSizeX()+1; x++) {
            for (int y = startY/eman->getTileSizeY(); y<endY/eman->getTileSizeY()+1; y++) {
                feildSprite.setPosition(x*eman->getTileSizeX(), y*eman->getTileSizeY());
                window->draw(feildSprite);
            }
        }
    }
    
    
    
}
void Player::plusPotion(){
    if(amountOfPotions<maxPotions){
        amountOfPotions++;
    }
}

