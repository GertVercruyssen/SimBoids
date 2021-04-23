//
//  main.cpp
//  SimBoids
//
//  Created by gert on 2021/04/16.
//  Copyright © 2021 gert. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "Boid.hpp"
#include <random>

static const int areaWidth = 80;
static const int areaHeight = 44;

void ClearScreen()
{
    //system("clear");
    std::cout << " \n";
    std::cout << " \n";
    std::cout << " \n";
}

float GenerateRandom(float min, float max)
{
    float test =static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return ((test)*(max-min)) +min;
}

void Draw(const std::vector<Boid*>& boids)
{
    std::string output = "";
    char field[areaWidth][areaHeight];
    //clear the field
    for(int i =0;i<areaWidth;++i)
        for(int j =0;j<areaHeight;++j)
            field[i][j] = ' ';
    
    //put the boids in the field
    for(Boid* b : boids )
    {
        std::pair<int,int> pos = b->GetPosition();
        if(pos.first<areaWidth and pos.first >= 0 and pos.second < areaHeight and pos.second>=0)
            field[b->GetPosition().first][b->GetPosition().second] = b->GetRepresentation();
    }
    field[0][0] = 'x';
    field[areaWidth-1][0] = 'x';
    field[0][areaHeight-1] = 'x';
    field[areaWidth-1][areaHeight-1] = 'x';
    
    //put it into a string that we can push to output (y axis is reversed!)
    for(int tellery = areaHeight-1; tellery >=0 ; --tellery)
    {
        for(int tellerx = 0; tellerx< areaWidth; ++tellerx)
        {
            output = output + field[tellerx][tellery];
        }
        output += "\n";
    }
    std::cout << output;
}

int main(int argc, const char * argv[])
{
    srand (static_cast <unsigned> (time(0)));
    std::vector<Boid*>* boids = new std::vector<Boid*>();
    for(int i = 0;i<32;++i)
        boids->push_back(new Boid(GenerateRandom(0,areaWidth), GenerateRandom(0, areaHeight), GenerateRandom(-1, 1), GenerateRandom(-1, 1), boids));
    
    for(int teller = 0; teller <2000; teller++)
    {
        Draw(*boids);
        for(Boid* b : *boids )
            b->Tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        //ClearScreen();
    }
    return 0;
}
