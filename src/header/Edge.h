#pragma once

#include "Point.h"
class Edge
{
public:
    Point* A;
    Point* B;
    bool is_bad = false;
    Edge& operator=(const Edge&) = default;
    Edge(Point* A_,Point* B_):
    A(A_), B(B_)
    {
        std::cout << "Edge ctor: id " << A->id << " -> " << B->id << " (" << A->x << ", " << A->y << ") -> (" << B->original_x << ", " << B->original_y << ")" << std::endl;
    }
    ~Edge()
    {
        std::cout << "Edge dtor: id " << A->id << " -> " << B->id << std::endl;
    };
};