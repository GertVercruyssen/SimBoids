//
//  Boid.cpp
//  SimBoids
//
//  Created by gert on 2021/04/16.
//  Copyright © 2021 gert. All rights reserved.
//

#include "Boid.hpp"
#include "math.h"
#include <algorithm>


static const int areaWidth = 80;
static const int areaHeight = 44;
static const int foresight = 5;
static const float cohesionStrength = 0.01f;
static const float sepperationStrength = 0.06f;
static const float alignmentStrength = 0.025f;

Boid::Boid(float startX, float startY, float speedx, float speedy, std::vector<Boid*>* Boids)
{
    position.first = startX;
    position.second = startY;
    velocity.first = speedx;
    velocity.second = speedy;
    Normalize(velocity.first, velocity.second);
    otherBoids = Boids;
}
char Boid::GetRepresentation()
{
    float slope = velocity.second/velocity.first;
    if(slope > 2.41421 or slope < -2.41421)
    return '|';
    if(slope > -0.414214 and slope < 0.414214)
    return '-';
    if(slope > 0)
    return '/';
    else
    return '\\';
}
std::pair<int,int> Boid::GetPosition()
{
    return std::pair<int,int>(floor(position.first),floor(position.second));
}
bool Boid::AvoidWalls()
{
    /*//exclusion zone near the edge
    if (position.first < exclusionwidth or position.first > areaWidth-exclusionwidth or position.second < exclusionwidth or position.second> areaHeight-exclusionwidth)
    {
        if(IsLeftofBoid(areaWidth/2, areaHeight/2))
        {
            Rotate(0.5);
            return true;
        }
        else
        {
            Rotate(-0.5);
            return true;
        }
    }
    else
        return false;
     */
    
    //rotate if we are approaching a wall
    std::pair<float,float> futurepos(position.first+(foresight*velocity.first),position.second+(foresight*velocity.second));
    
    std::pair<float,float> left(futurepos.first+(foresight*velocity.second*-1),futurepos.second+(foresight*velocity.first));
    std::pair<float,float> right(futurepos.first+(foresight*velocity.second),futurepos.second+(foresight*velocity.first*-1));
    if(CheckIfOutsideBounds(left.first, left.second)) //rotate clockwise if our front left is inside a wall
    {
         Rotate(-0.5);
        return true;
    }
    else if(CheckIfOutsideBounds(right.first, right.second))//otherwise turn right
    {
        Rotate(0.5);
        return true;
    }
    else
        return false;
     
}

void Boid::Tick()
{
    //order of boid behavior is important
    if(!AvoidWalls())
    {
        //GoTowardsCenter();
        Cohesion();
        Alignment();
        Seperation();
    }
    
    position.first += velocity.first/2;
    position.second += velocity.second/2;
}
void Boid::Normalize(float& x, float& y, float desiredlength)
{
    float length = sqrt(x*x+y*y);
    x /= (length*desiredlength);
    y /= (length*desiredlength);
}

void Boid::Rotate(float angle)
{
    std::pair<float,float> newVelocity;
    newVelocity.first = cos(angle)*velocity.first - sin(angle)*velocity.second;
    newVelocity.second = sin(angle)*velocity.first + cos(angle)*velocity.second;
    velocity = newVelocity;
}

bool Boid::CheckIfOutsideBounds(float x, float y)
{
    return (x > areaWidth or x <0 or y > areaHeight or y <0);
}

void Boid::Seperation()
{
    std::vector<Boid*> closeBoids = GetAllInDistance(0.5);
    if(closeBoids.size() > 0)
    {
        //get center of mass
        std::pair<float,float> center = GetAverage(closeBoids);
        //steer away strongly
        float turnrate = sepperationStrength;//CalcDistance(center.first, center.second, position.first, position.second);
        if(IsLeftofBoid(center.first, center.second))
            Rotate(-1*turnrate);
        else
            Rotate(turnrate);
    }
}
void Boid::Alignment()
{
    std::vector<Boid*> closeBoids = GetAllInDistance(5);
    //get average direction
    float totalx = 0;
    float totaly = 0;
    for(Boid* b : closeBoids)
    {
        totalx += b->velocity.first;
        totaly += b->velocity.second;
    }
    totalx = (totalx/closeBoids.size())*alignmentStrength; //last number is strength of alignment
    totaly = (totaly/closeBoids.size())*alignmentStrength;
    
    velocity.first += totalx;
    velocity.second += totaly;
    Normalize(velocity.first, velocity.second);
}
void Boid::Cohesion()
{
    std::vector<Boid*> closeBoids = GetAllInDistance(30);
    //get center of mass
    std::pair<float,float> center = GetAverage(closeBoids);
    //steer closer gently
    float turnrate = CalcDistance(center.first, center.second, position.first, position.second)*cohesionStrength;
    if(IsLeftofBoid(center.first, center.second))
        Rotate(turnrate);
    else
        Rotate(turnrate*-1);
}
std::vector<Boid*> Boid::GetAllInDistance(float distance)
{
    std::vector<Boid*> closeBoids {};
    for(Boid* b : *otherBoids)
        if(CalcDistance(position.first, position.second,b->position.first, b->position.second) < distance)
            closeBoids.push_back(b);
    return closeBoids;
}
float Boid::CalcDistance(float x1, float y1, float x2, float y2)
{
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}
std::pair<float,float> Boid::GetAverage(std::vector<Boid*>& boids)
{
    float totalx = 0;
    float totaly = 0;
    for(Boid* b : boids)
    {
        totalx += b->position.first;
        totaly += b->position.second;
    }
    std::pair<float, float> average(totalx/boids.size(),totaly/boids.size());
    return average;
}
bool Boid::IsLeftofBoid(float x, float y)
{
    float point2x = position.first+velocity.first;
    float point2y = position.second+velocity.second;
    return (((point2x - position.first)*(y - position.second) - (point2y - position.second)*(x - position.first)) >0);
}
void Boid::GoTowardsCenter()
{
    float distance = CalcDistance(position.first, position.second, areaWidth/2, areaHeight/2);
    if(IsLeftofBoid(areaWidth/2, areaHeight/2))
        Rotate(0.001*distance);
    else
        Rotate(-0.001*distance);
}
