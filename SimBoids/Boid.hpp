//
//  Boid.hpp
//  SimBoids
//
//  Created by gert on 2021/04/16.
//  Copyright © 2021 gert. All rights reserved.
//

#ifndef Boid_hpp
#define Boid_hpp

#include <stdio.h>
#include <vector>
#include <utility>

#endif /* Boid_hpp */
class Boid
{
public:
    Boid(float startX, float startY, float speedx, float speedy, std::vector<Boid*>* Boids);
    char GetRepresentation();
    std::pair<int,int> GetPosition();
    std::pair<float,float> velocity;
    std::pair<float,float> position;
    void Tick();
private:
    std::vector<Boid*> GetAllInDistance(float distance);
    static void Normalize(float& x, float& y, float desiredlength = 1.0f);
    bool AvoidWalls();
    void Seperation();
    void Alignment();
    void Cohesion();
    void GoTowardsCenter();
    void Rotate(float angle);
    bool CheckIfOutsideBounds(float x, float y);
    std::vector<Boid*>* otherBoids;
    static float CalcDistance(float x1, float y1, float x2, float y2);
    static std::pair<float,float> GetAverage(std::vector<Boid*>& boids);
    bool IsLeftofBoid(float x, float y);
};
