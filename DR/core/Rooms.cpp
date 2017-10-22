//
//  Rooms.cpp
//  Dungeons
//
//  Created by Alan Yuan on 2017-01-11.
//  Copyright © 2017 Alan Yuan. All rights reserved.
//
#include <algorithm>
#include <iostream>
#include "Rooms.hpp"

int room::getArea(int x, int y){
    return Room[x][y]->id;
}



void room::render(sf::RenderWindow* window){
    int startX = (cam->getCenter().x - (20 * renderSizeX) )/renderSizeX;
    int endX = (cam->getCenter().x + (20 * renderSizeX) )/renderSizeX;
    
    int startY = (cam->getCenter().y - (14 * renderSizeY) )/renderSizeY;
    int endY = (cam->getCenter().y + (14 * renderSizeY) )/renderSizeY;
    
    startX = std::max(startX, 0);
    startY = std::max(startY, 0);
    endX = std::min(endX, roomSize);
    endY = std::min(endY, roomSize);
    
    /* Same with the X values so that we don't access values outside our array. */
    
    // when changin this remember to change eman render shaders too
    for(int y = startY; y < endY; y++){
        for(int x = startX; x < endX; x++){
            Room[x][y]->render(window,this);
        }
    }
}

int room::getRenderSizeX(){
    return renderSizeX;
}
int room::getRenderSizeY(){
    return renderSizeY;
}

void room::genorate(int seed){
    srand(seed);
    int Room[roomSize][roomSize];
    struct roomRect{
        int x,y,w,h;
        bool deleted;
    };
    struct lineConnect{
        int id1, id2;
        double distance;
    };
    
    const int numberOfRooms = 50;
    const int maxRoomSize = 10;
    const int minRoomSize = 5;
    
    roomRect roomList[numberOfRooms];
    
    for (int y = 0 ; y < roomSize; y++) {
        for (int x = 0; x < roomSize; x++) {
            Room[x][y] = 0;
        }
    }
    
    //create rooms and spread them out
    
    // NOTE: must create the rooms near the middle to make sure the boxes don't go too far
    
    const int sBox = roomSize/3;
    const int eBox = roomSize*2/3-minRoomSize;
    
    for (int i = 0; i < numberOfRooms; i++) {
        roomList[i].x = rand()%(eBox-sBox)+sBox;
        roomList[i].y = rand()%(eBox-sBox)+sBox;
        roomList[i].w = rand()%(maxRoomSize-minRoomSize)+minRoomSize+1;
        roomList[i].h = rand()%(maxRoomSize-minRoomSize-2)+minRoomSize;
        roomList[i].deleted = false;
    }
    
    camps.clear();
    
    //spread them if they intersect
    bool moved = true; // if one room moved then we restart the spreading process
    while (moved) {
        moved = false;
        for (int r1 = 0; r1 < numberOfRooms; r1++) {
            if (!roomList[r1].deleted) {
                for (int r2 = r1+1; r2 < numberOfRooms; r2++) {
                    if(!roomList[r2].deleted){
                        //for every box, check with other boxes to see if they colide
                        //check collision
                        if (roomList[r1].x-1 < roomList[r2].x+roomList[r2].w && roomList[r1].x+roomList[r1].w +1 > roomList[r2].x &&
                            roomList[r1].y < roomList[r2].y+roomList[r2].h+1 && roomList[r1].y+roomList[r1].h+1 > roomList[r2].y){
                            
                            //is collide
                            moved = true;
                            
                            int moveX = 0;
                            int moveY = 0;
                            int rx = 0;
                            int ry = 0;
                            
                            if(roomList[r1].x+roomList[r1].w/2>=roomSize/2){
                                //moving right
                                if(roomList[r1].x>roomList[r2].x){
                                    // r1 is more right so r1 moves
                                    rx = r1;
                                    moveX = roomList[r2].x + roomList[r2].w - roomList[r1].x + 1;
                                }else{
                                    // r2 is moving right
                                    rx = r2;
                                    moveX = roomList[r1].x + roomList[r1].w - roomList[r2].x + 1;
                                }
                            }else{
                                //moving left
                                if(roomList[r1].x+roomList[r1].w<roomList[r2].x+roomList[r2].w){
                                    // r1 is more left so r1 moves
                                    rx = r1;
                                    
                                    moveX = roomList[r2].x - roomList[r1].x - roomList[r1].w - 1;
                                }else{
                                    rx = r2;
                                    // r2 is moving left
                                    moveX = roomList[r1].x - roomList[r2].x - roomList[r2].w - 1;
                                }
                            }
                            
                            if(roomList[r1].y+roomList[r1].h/2>=roomSize/2){
                                //moving down
                                if(roomList[r1].y>roomList[r2].y){
                                    ry = r1;
                                    // r1 is more down so r1 moves
                                    moveY = roomList[r2].y + roomList[r2].h - roomList[r1].y + 1;
                                }else{
                                    ry = r2;
                                    // r2 is moving right
                                    moveY = roomList[r1].y + roomList[r1].h - roomList[r2].y + 1;
                                }
                            }else{
                                //moving up
                                if(roomList[r1].y+roomList[r1].h<roomList[r2].y+roomList[r2].h){
                                    // r1 is more up so r1 moves
                                    ry = r1;
                                    moveY = roomList[r2].y - roomList[r1].y - roomList[r1].h - 1;
                                }else{
                                    // r2 is moving up
                                    ry = r2;
                                    moveY = roomList[r1].y - roomList[r2].y - roomList[r2].h - 1;
                                }
                            }
                            
                            if(abs(moveX)<=abs(moveY)){
                                //move x
                                roomList[rx].x+=moveX;
                            }else{
                                roomList[ry].y+=moveY;
                                //move y
                            }
                            
                            //deleting rooms that go out of bounds
                            if(roomList[r1].x < 1 || roomList[r1].y < 1 || roomList[r1].y+roomList[r1].h > roomSize - 1 || roomList[r1].x + roomList[r1].w > roomSize - 1){
                                roomList[r1].deleted = true;
                            }
                            if(roomList[r2].x < 1 || roomList[r2].y < 1 || roomList[r2].y+roomList[r2].h > roomSize - 1 || roomList[r2].x + roomList[r2].w > roomSize - 1){
                                roomList[r2].deleted = true;
                            }
                            
                            
                        }
                    }
                }
            }
        }
    }
    
    // set the room
    for (int i = 0; i < numberOfRooms; i++) {
        if (!roomList[i].deleted) {
            for (int y = roomList[i].y; y < roomList[i].y+roomList[i].h; y++) {
                for (int x = roomList[i].x; x < roomList[i].x+roomList[i].w; x++) {
                    Room[x][y] = 1;
                }
            }
        }
    }
    
    
    for (int i = 0; i < numberOfRooms; i++) {
        if(!roomList[i].deleted) {
            for (int x = roomList[i].x; x < roomList[i].w+roomList[i].x; x++) {
                Room[x][roomList[i].y] = 2;
            }
        }
    }
    
    for (int i = 0; i < numberOfRooms; i++) {
        if (roomList[i].deleted) {
            roomList[i].w = -10;
            roomList[i].x = -10;
            roomList[i].h = -10;
            roomList[i].y = -10;
        }
    }
    
    
    std::vector<std::vector<int>> groupsOfGroups;
    
    //find hallways
    for (int curID = 0; curID < numberOfRooms; curID++) {
        if(!roomList[curID].deleted){
            bool placeL = true, placeR = true;
            
            for (int y = roomList[curID].y; y < roomList[curID].y + roomList[curID].h; y++) {
                if(Room[roomList[curID].x-1][y] == 1){
                    placeL = false;
                }
                if(Room[roomList[curID].x+roomList[curID].w][y] == 1){
                    placeR = false;
                }
            }
            
            // NOTE: Proccess for connecting up and down
            // create new vector of ids that are connected to this room
            // find the id of the room this am connecting to named id2
            // search groups for id2's group
            // append them all ids to this group
            // append this group to groups
            // connect up and down
            
            // move right
            //new vector of ids
            std::vector<int> currentGroup;
            //add myself and my group
            bool ihaveBeenFound = false;
            for (int searchFor2 = 0; searchFor2 < groupsOfGroups.size()&&!ihaveBeenFound; searchFor2++) {
                // for all groups
                for (int inGroup = 0; inGroup < groupsOfGroups[searchFor2].size()&&!ihaveBeenFound; inGroup++) {
                    // search all for id2           ingroup index
                    if(groupsOfGroups[searchFor2][inGroup] == curID){
                        ihaveBeenFound = true;
                        // if found then add all of id2 into current group
                        for (int connectThese = 0; connectThese < groupsOfGroups[searchFor2].size(); connectThese++) {
                            currentGroup.push_back(groupsOfGroups[searchFor2][connectThese]);
                        }
                        // delete groupsOfGroups[searchFor2]
                        groupsOfGroups.erase(groupsOfGroups.begin()+searchFor2);
                    }
                }
            }
            if (!ihaveBeenFound) {
                currentGroup.push_back(curID);
            }
            if(placeL){
                std::vector<int> placeY; // the y will be between y to y + h - 2
                
                for (int ny = roomList[curID].y; ny < roomList[curID].y+roomList[curID].h - 2; ny++) {
                    int nx = roomList[curID].x - 1; // x starts right before the room's x
                    bool keepGoing = true;
                    
                    while (keepGoing) {
                        nx--;
                        if(nx >= 0){
                            if (Room[nx][ny + 1] == 1 && Room[nx][ny + 2] == 1){
                                keepGoing = false;
                                placeY.push_back(ny);
                                
                            }else if(Room[nx][ny] == 1 || Room[nx][ny+3] == 1){
                                keepGoing = false;
                            }
                        }else{
                            keepGoing = false;
                        }
                    }
                    
                }
                
                if(placeY.size()>0){
                    
                    int ny = placeY[rand() % placeY.size()];
                    int nx = roomList[curID].x-1;
                    while (Room[nx][ny + 1] == 0) {
                        Room[nx][ny] = 2;
                        Room[nx][ny + 1] = 1;
                        Room[nx][ny + 2] = 1;
                        nx--;
                    }
                    
                    
                    //find id of the room that is connected to this
                    bool stopLoop = false;
                    for (int id2 = 0; id2 < roomSize && !stopLoop; id2++) {
                        if(!roomList[id2].deleted){
                            if(roomList[id2].x <= nx - 1&& roomList[id2].x + roomList[id2].w-1 >= nx - 1
                               && roomList[id2].y <= ny+1&& roomList[id2].y + roomList[id2].h-1 >=ny+1){
                                
                                //id2 has been found
                                
                                bool id2HasBeenFound = false;
                                for (int searchFor2 = 0; searchFor2 < groupsOfGroups.size()&&!id2HasBeenFound; searchFor2++) {
                                    // for all groups
                                    for (int inGroup = 0; inGroup < groupsOfGroups[searchFor2].size()&&!id2HasBeenFound; inGroup++) {
                                        // search all for id2           ingroup index
                                        if(groupsOfGroups[searchFor2][inGroup] == id2){
                                            id2HasBeenFound = true;
                                            // if found then add all of id2 into current group
                                            for (int connectThese = 0; connectThese < groupsOfGroups[searchFor2].size(); connectThese++) {
                                                
                                                currentGroup.push_back(groupsOfGroups[searchFor2][connectThese]);
                                                
                                            }
                                            // delete groupsOfGroups[searchFor2]
                                            groupsOfGroups.erase(groupsOfGroups.begin()+searchFor2);
                                        }
                                    }
                                }
                                
                                if (!id2HasBeenFound) {
                                    currentGroup.push_back(id2);
                                }
                                stopLoop = true;
                            }
                        }
                    }
                    
                }
            }
            
            if (placeR) {
                std::vector<int> placeY; // the y will be between y to y + h - 2
                
                for (int ny = roomList[curID].y; ny < roomList[curID].y+roomList[curID].h - 2; ny++) {
                    int nx = roomList[curID].x + roomList[curID].w; // x starts right before the room's x
                    bool keepGoing = true;
                    while (keepGoing) {
                        nx++;
                        if(nx < roomSize-1){
                            if (Room[nx][ny + 1] == 1 && Room[nx][ny + 2] == 1){
                                keepGoing = false;
                                placeY.push_back(ny);
                            }else if(Room[nx][ny] == 1 || Room[nx][ny+3] == 1){
                                keepGoing = false;
                            }
                        }else{
                            keepGoing = false;
                        }
                    }
                }
                
                if(placeY.size()>0){
                    
                    int ny = placeY[rand() % placeY.size()];
                    int nx = roomList[curID].x+roomList[curID].w;
                    while (Room[nx][ny + 1] == 0) {
                        Room[nx][ny] = 2;
                        Room[nx][ny + 1] = 1;
                        Room[nx][ny + 2] = 1;
                        nx++;
                    }
                    
                    //find id of the room that is connected to this
                    bool stopLoop = false;
                    for (int id2 = 0; id2 < roomSize && !stopLoop; id2++) {
                        if(!roomList[id2].deleted){
                            if(roomList[id2].x <= nx + 1&& roomList[id2].x + roomList[id2].w-1 >= nx + 1
                               && roomList[id2].y <= ny+1&& roomList[id2].y + roomList[id2].h-1 >=ny+1){
                                
                                //id2 has been found
                                
                                bool id2HasBeenFound = false;
                                for (int searchFor2 = 0; searchFor2 < groupsOfGroups.size()&&!id2HasBeenFound; searchFor2++) {
                                    // for all groups
                                    for (int inGroup = 0; inGroup < groupsOfGroups[searchFor2].size()&&!id2HasBeenFound; inGroup++) {
                                        // search all for id2           ingroup index
                                        if(groupsOfGroups[searchFor2][inGroup] == id2){
                                            id2HasBeenFound = true;
                                            // if found then add all of id2 into current group
                                            for (int connectThese = 0; connectThese < groupsOfGroups[searchFor2].size(); connectThese++) {
                                                
                                                currentGroup.push_back(groupsOfGroups[searchFor2][connectThese]);
                                                
                                            }
                                            // delete groupsOfGroups[searchFor2]
                                            groupsOfGroups.erase(groupsOfGroups.begin()+searchFor2);
                                        }
                                    }
                                }
                                
                                if (!id2HasBeenFound) {
                                    currentGroup.push_back(id2);
                                }
                                stopLoop = true;
                            }
                        }
                    }
                    
                    //search for id2 in groups
                    
                }
            }
            groupsOfGroups.push_back(currentGroup);
        }
        
    }
    // make an array of y for every x
    // for every group
    // for every x
    // for every room in group find the lowest y for that x
    // while placed = false && attempts < 100
    // go to that lowest y and go down
    const int repeatThisManyTimes = 2;
    for (int times = 0; times < repeatThisManyTimes; times++) {
        for (int i = 0; i < groupsOfGroups.size(); i++) {
            {
                int y[roomSize];
                for (int x = 0; x < roomSize; x++) {
                    y[x] = 0;
                }
                // down first
                for (int x = 0; x < roomSize; x++) {
                    for (int curG = 0; curG < groupsOfGroups[i].size(); curG++) {
                        if(roomList[groupsOfGroups[i][curG]].x-1 < x && roomList[groupsOfGroups[i][curG]].x +roomList[groupsOfGroups[i][curG]].w > x){
                            if(roomList[groupsOfGroups[i][curG]].y+roomList[groupsOfGroups[i][curG]].h-1 > y[x]){
                                // set y + h as y[x]
                                y[x] = roomList[groupsOfGroups[i][curG]].y+roomList[groupsOfGroups[i][curG]].h-1;
                            }
                        }
                    }
                }
                
                
                
                bool placed = false;
                std::vector<int> possibleX;
                for (int x = 0; x < roomSize&&!placed; x++) {
                    if(y[x] == y[x+1]&&y[x]!=0){
                        int currentY = y[x]+1;
                        bool placed = false;
                        while (!placed) {
                            
                            if(Room[x+1][currentY]!=0&&Room[x][currentY] == Room[x+1][currentY]&&Room[x+2][currentY] != 1 && Room[x-1][currentY] != 1){
                                for (int currentGroup = 0; currentGroup < groupsOfGroups.size()&&!placed; currentGroup++) {
                                    for (int currentRoom = 0; currentRoom < groupsOfGroups[currentGroup].size()&&!placed; currentRoom++) {
                                        if(x >= roomList[groupsOfGroups[currentGroup][currentRoom]].x && x < roomList[groupsOfGroups[currentGroup][currentRoom]].x +roomList[groupsOfGroups[currentGroup][currentRoom]].w && currentY < roomList[groupsOfGroups[currentGroup][currentRoom]].y + roomList[groupsOfGroups[currentGroup][currentRoom]].h && currentY >= roomList[groupsOfGroups[currentGroup][currentRoom]].y){
                                            possibleX.push_back(x);
                                            
                                        }
                                    }
                                }
                                placed = true;
                            }else if(currentY+1 < roomSize){
                                currentY++;
                            }else{
                                placed = true;
                            }
                        }
                    }
                }
                
                
                if(possibleX.size()>0){
                    int x = possibleX[rand()%possibleX.size()];
                    int currentY = y[x]+1;
                    while (!placed) {
                        if(Room[x+1][currentY]!=0&&Room[x][currentY] == Room[x+1][currentY]){
                            Room[x][currentY] = 1;
                            Room[x+1][currentY] = 1;
                            placed = true;
                        }else if(currentY+1 < roomSize){
                            Room[x][currentY] = 1;
                            Room[x+1][currentY] = 1;
                            currentY++;
                        }else{
                            placed = true;
                        }
                    }
                }
            }
            
            {
                int y[roomSize];
                for (int x = 0; x < roomSize; x++) {
                    y[x] = roomSize;
                }
                // up next
                for (int x = 0; x < roomSize; x++) {
                    for (int curG = 0; curG < groupsOfGroups[i].size(); curG++) {
                        if(roomList[groupsOfGroups[i][curG]].x-1 < x && roomList[groupsOfGroups[i][curG]].x +roomList[groupsOfGroups[i][curG]].w > x){
                            if(roomList[groupsOfGroups[i][curG]].y-1 < y[x]){
                                // set y + h as y[x]
                                y[x] = roomList[groupsOfGroups[i][curG]].y-1;
                            }
                        }
                    }
                }
                
                bool placed = false;
                std::vector<int> possibleX;
                for (int x = 0; x < roomSize&&!placed; x++) {
                    if(y[x] == y[x+1]&&y[x]!=roomSize){
                        int currentY = y[x];
                        bool placed = false;
                        while (!placed) {
                            
                            if(Room[x+1][currentY]!=0&&Room[x][currentY] == Room[x+1][currentY]&&Room[x+2][currentY] != 1 && Room[x-1][currentY] != 1){
                                for (int currentGroup = 0; currentGroup < groupsOfGroups.size()&&!placed; currentGroup++) {
                                    for (int currentRoom = 0; currentRoom < groupsOfGroups[currentGroup].size()&&!placed; currentRoom++) {
                                        if(x >= roomList[groupsOfGroups[currentGroup][currentRoom]].x && x <= roomList[groupsOfGroups[currentGroup][currentRoom]].x +roomList[groupsOfGroups[currentGroup][currentRoom]].w && currentY <= roomList[groupsOfGroups[currentGroup][currentRoom]].y + roomList[groupsOfGroups[currentGroup][currentRoom]].h && currentY >= roomList[groupsOfGroups[currentGroup][currentRoom]].y){
                                            possibleX.push_back(x);
                                            
                                        }
                                    }
                                }
                                placed = true;
                            }else if(currentY-1 > 0){
                                currentY--;
                            }else{
                                placed = true;
                            }
                        }
                    }
                }
                
                
                if(possibleX.size()>0){
                    int x = possibleX[rand()%possibleX.size()];
                    int currentY = y[x];
                    Room[x][currentY+1] = 1;
                    Room[x+1][currentY+1] = 1;
                    while (!placed) {
                        if(Room[x+1][currentY]!=0&&Room[x][currentY] == Room[x+1][currentY]){
                            Room[x][currentY] = 1;
                            Room[x+1][currentY] = 1;
                            placed = true;
                        }else if(currentY-1 < roomSize){
                            Room[x][currentY] = 1;
                            Room[x+1][currentY] = 1;
                            currentY--;
                        }else{
                            placed = true;
                        }
                    }
                }
                //            for (int x = 0; x < possibleX.size(); x++) {
                //                Room[possibleX[x]][y[possibleX[x]]] = 7;
                //            }
                
            }
            
        }
    }
    
    
    //
    //    for (int i = 0; i < groupsOfGroups.size(); i++) {
    //        for (int b = 0; b < groupsOfGroups[i].size(); b++) {
    //
    //            Room[roomList[groupsOfGroups[i][b]].x+roomList[groupsOfGroups[i][b]].w/2][roomList[groupsOfGroups[i][b]].y + roomList[groupsOfGroups[i][b]].h/2] = i+1;
    //        }
    //    }
    //trace the outside of all rooms and if there is no path then remove the room
    
    for (int x = 0; x < roomSize; x++) {
        for (int y = 0; y < roomSize; y++) {
            delete this->Room[x][y];
            if (Room[x][y]==2) {
                this->Room[x][y] = new wallTile(x,y);
            }else{
                this->Room[x][y] = new tile(x,y,Room[x][y]);
            }
        }
    }
    
    //spwn room
    
    spwnX = roomList[0].x + roomList[0].w/2;
    spwnY = roomList[0].y + roomList[0].h/2;
    //for everyroom - spwn guysstea
    
    for (int i = numberOfRooms-1; i > 0; i--) {
        if(!roomList[i].deleted){
            endX = roomList[i].x + roomList[i].w/2;
            endY = roomList[i].y + roomList[i].h/2;
            i--;
            for (; i > 0; i--) {
                if(!roomList[i].deleted){
                    camps.push_back(sf::Vector2i(roomList[i].x + roomList[i].w/2,roomList[i].y + roomList[i].h/2));
                }
            }
        }
        
    }
    
}


sf::View*room::getView(){
    return cam;
}

void room::setVision(int sx, int sy, eManager*eman){
    
    //VISION HERE
    setVisible(sx, sy,sx,sy, eman);
    for (uint i = 0; i < 8; i++) {
        castLight(sx, sy, visionRange, 1, 1.0, 0.0, multipliers[0][i],
                  multipliers[1][i], multipliers[2][i], multipliers[3][i], eman);
    }
}






