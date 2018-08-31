//
//  main.cpp
//  BlockGame
//
//  Created by Diego Gonçalves on 30/08/18.
//  Copyright © 2018 Diego Gonçalves. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int board[4][4];

struct Position
{
    Position(int x,int y) : x(x), y(y) {};
    int x;
    int y;
};

std::vector<Position> listOfPositions;

/*
 values : in hex
 1 - connects to the top
 2 - connects to the bottom
 4 - connects to the right
 8 - connects to the left
 10 - end pos
 20 - start pos
 
 1 2 4 8 16 32 in decimal
 */


//use to find start and end
Position FindEdgePosition(int edge)
{
    Position edgePosition(-1,-1);
    
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if((board[i][j] & edge) == edge){
                edgePosition.x = i;
                edgePosition.y = j;
                break;
            }
        }
    }
    
    return edgePosition;
}

int currentDirection;
int currentRow;
int currentColumn;

bool PathIsValid()
{
    int value;
    
    switch (currentDirection) {
        case 1:
            if(currentRow ==0) //cant connect up from the top!
                return false;
            else{
                value = board[currentRow-1][currentColumn];
                if((value & 2) != 2) //needs to connect to bottom
                    return false;
                else{
                    currentDirection = value ^ 2;
                    currentRow--;
                }
            }
            break;
            
        case 2:
            if(currentRow ==3) //cant connect down from the bottom!
                return false;
            else{
                value = board[currentRow+1][currentColumn];
                if((value & 1) != 1) //needs to connect to top
                    return false;
                else{
                    currentDirection = value ^ 1;
                    currentRow++;
                }
            }
            break;

        case 4:
            if(currentColumn ==3) //cant connect right from ... the right
                return false;
            else{
                value = board[currentRow][currentColumn+1];
                if((value & 8 ) != 8) //needs to connect to left
                    return false;
                else{
                    currentDirection = value ^ 8;
                    currentColumn++;
                }
            }
            break;
            
        case 8:
            if(currentColumn == 0) //cant connect left from the left
                return false;
            else{
                value = board[currentRow][currentColumn-1];
                if((value & 4) != 4) //needs to connect to right
                    return false;
                else{
                    currentDirection = value ^ 4;
                    currentColumn--;
                }
            }
            break;
            
    }
    return true;
}

bool PositionValid(Position currentPosition)
{
    if(currentPosition.x > 3 || currentPosition.x <0)
        return false;
    
    if(currentPosition.y > 3 || currentPosition.y <0)
        return false;
    
    return true;
}

int main(int argc, const char * argv[]) {
    
    std::string fileName = argv[1];
    std::fstream fileStream;
    std::string line;
    
    
    fileStream.open(fileName,std::fstream::in);
    if(fileStream.is_open()){
        int j =0;
        
        while(getline(fileStream,line)){
            std::istringstream stream(line);
            for(int i =0; i < 4; i++){//hardcoding this
                stream >> board[j][i];
            }
            j++;
        }
    }
    
    Position start = FindEdgePosition(32);
    Position end =   FindEdgePosition(16);
    
    if(!PositionValid(start) || !PositionValid(end)){
        std::cout << "PATH IS NOT VALID - MISSING START OR END POINT" <<  "\n";
        return 0;
    }
    
    int startDirection = board[start.x][start.y];
    startDirection = startDirection ^ 32; //because we know the code for the start
    
    listOfPositions.push_back(start);
    
    currentDirection = startDirection;
    
    currentRow = start.x;
    currentColumn = start.y;
    
    bool pathValid = false;
    
    while(PathIsValid()){
        listOfPositions.push_back(Position(currentRow,currentColumn));
        if(currentRow == end.x && currentColumn == end.y){
            pathValid = true;
            break;
        }
    }
    
    if(pathValid){
        std::cout << "PATH IS VALID" <<  "\n";
    }
    else{
        std::cout << "PATH IS NOT VALID" <<  "\n";
        listOfPositions.clear();
    }
    
    for(auto pos : listOfPositions){
        std::cout << pos.x << " " << pos.y << "\n";
    }
    
    return 0;
}
