#pragma once

#include "Point.h"
class Edge
{
public:
    Point* A;
    Point* B;
    Edge(Point* A_, Point* B_)
    {
        A = A_;
        B = B_;
    }
    ~Edge() = default;
};