#pragma once

#include "Point.h"
class Edge
{
public:
    Point* A;
    Point* B;
    Edge () = default;
    Edge(Point* A_, Point* B_)
    {
        // Let's define an edge as having the largest ID as the first point
        if (A_->id > B_->id)
        {
            A = A_;
            B = B_;
        }
        else
        {
            B = A_;
            A = B_;
        }
        std::cout << "Edge ctor: id " << A->id << " -> " << B->id << " (" << A->x << ", " << A->y << ") -> (" << B->original_x << ", " << B->original_y << ")" << std::endl;
    }
    ~Edge() = default;
};